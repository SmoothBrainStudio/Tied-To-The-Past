#include "FightingMiniGames.hpp"
#include "../Player.hpp"
#include "../Enemy.hpp"

MiniGame::MiniGame(void)
{
	Screen& windowManager = Screen::Get();

	textResult.setTexture(TexturesManager::Get()["MISS_TEXT_FIGHTING"]);

	sf::FloatRect rect = textResult.getGlobalBounds();
	textResult.setOrigin(rect.width / 2.f, rect.height / 2.f);
	textResult.setPosition(WORKING_WIDTH / 2.f, 256.f);
	textResult.setColor(sf::Color::Transparent);

	DrawManager::Get().AddObject("FIGHT_HUD", *this);

	for (int i = 0; i < PLAYER_CLONE_SIZE; i++)
	{
		//playerClone[i] = player->GetCurAnim()->GetSprite();
		//playerClone[i].setPosition(player->getPosition());

		DrawManager::Get().AddObject("FIGHT", playerClone[i]);
		DrawManager::Get().SetObjectVisible("FIGHT", &playerClone[i], false);
	}

	warning.setTexture(TexturesManager::Get()["WARNING"], true);
	warning.setScale(0.5f, 0.5f);
	sf::FloatRect rectWarning = warning.getGlobalBounds();
	warning.setOrigin(rectWarning.width / 2.f, rectWarning.height);

	ShadersManager::Get()["VOID_OUT"].setUniform("resolution", sf::Vector2f(Screen::Get().GetWidth(), Screen::Get().GetHeight()));

	counteredOneTime = false;
	DrawManager::Get().AddObject("FIGHT_HUD", warning, false);
}

MiniGame::~MiniGame(void)
{
	for (int i = 0; i < PLAYER_CLONE_SIZE; i++)
	{
		DrawManager::Get().DeleteObject("FIGHT", &playerClone[i]);
	}

	DrawManager::Get().DeleteObject("FIGHT_HUD", &warning);

	DrawManager::Get().DeleteObject("FIGHT_HUD", this);
}

void MiniGame::Init(std::shared_ptr<Player>& _player, std::shared_ptr<Enemy>& _enemy, FightingState* _fightingState)
{
	totalTime = 0.f;
	for (int i = 0; i < PLAYER_CLONE_SIZE; i++)
	{
		playerCloneActionTime[i] = 0.f;
	}
	over = false;
	playerActiveDef = false;

	enemy = _enemy;
	if (enemy != nullptr)
	{
		enemyInitPos = enemy->getPosition();
	}

	player = _player;
	if (player != nullptr)
	{
		playerInitPos = player->getPosition();
		playerActionTime = 0.f;
		playerCoolDown = 0.f;
	}

	textResultTime = FLT_MAX;

	ShadersManager::Get()["VOID_OUT"].setUniform("resolution", sf::Vector2f(Screen::Get().GetWidth(), Screen::Get().GetHeight()));

	InitMiniGame(_player, _enemy, _fightingState);
}

void MiniGame::PollEvent(sf::Event& _event, FightingState* _fightingState)
{
	if (!over)
	{
		PollEventMiniGame(_event, _fightingState);
	}

	if (playerActiveDef)
	{
		EventPlayerDefense(_event);
	}
}

void MiniGame::Update(FightingState* _fightingState)
{
	const float& dt = GameClock::Get().GetDeltaTime();
	totalTime += dt;
	voidTime += dt * VOID_SPEED;
	textResultTime += dt;

	sf::Color result = sf::Color::White;
	result.a = (sf::Uint8)(255 - Maths::Clamp(textResultTime / TEXT_RESULT_TIME, 0.f, 1.f) * 255);
	textResult.setColor(result);

	DrawManager::Get().SetObjectVisible("FIGHT_HUD", &warning, (!BoolManager::Get()["FightAid"] && activeWarning));
	if (counteredOneTime)
	{
		shaderTotalTime += GameClock::Get().GetDeltaTime();

		ShadersManager::Get()["VOID_OUT"].setUniform("time", shaderTotalTime);
		ShadersManager::Get()["VOID_OUT"].setUniform("mousepos", Screen::Get().Pos(sf::Vector2f(player->getPosition().x + 20.f, player->getPosition().y - 400.f)));
		ShadersManager::Get()["VOID_OUT"].setUniform("factor1", voidTime / VOID_SPEED);
	}
	if (shaderTotalTime > 2.f)
	{
		counteredOneTime = false;
	}
	

	if (!over)
	{
		UpdateMiniGame(_fightingState);
	}

	if (playerActiveDef)
	{
		UpdatePlayerDefense();
	}
}

void MiniGame::FinishMiniGame(void)
{
	over = true;

	for (int i = 0; i < PLAYER_CLONE_SIZE; i++)
	{
		DrawManager::Get().SetObjectVisible("FIGHT", &playerClone[i], false);
		playerCloneActionTime[i] = 0.f;
	}

	for (unsigned int i = 0; i < player->GetParticles().size(); ++i)
	{
		if (i != (int)Player::ParticleTypes::DAMAGE && player->GetParticles()[i]->GetStatue() == ParticleSystem::Statue::PLAYING)
		{
			player->GetParticles()[i]->Update();
			if (player->GetParticles()[i]->GetStatue() == ParticleSystem::Statue::STOPPED)
			{
				player->GetParticles()[i]->Reset();
			}
		}
	}

	if (player != nullptr && player->IsAlive())
	{
		player->setPosition(playerInitPos);
	}
	if (enemy != nullptr && !enemy->IsAlive())
	{
		enemy->setPosition(enemyInitPos);
	}
}

bool MiniGame::IsOver(void)
{
	const bool enemyDead = enemy == nullptr ? false : enemy->GetHP() <= 0;
	const bool playerIdle = player == nullptr ? false : player->GetState() == PlayerState::IDLE;

	return (enemyDead || over) && playerIdle && textResultTime >= TEXT_RESULT_TIME && !counteredOneTime;
}

void MiniGame::EventPlayerDefense(sf::Event& _event)
{
	ControlsManager& controlsMng = ControlsManager::Get();

	PlayerState playerState = player->GetState();
	if (playerState == PlayerState::IDLE && !player->IsStun() && playerCoolDown < 0.f && !over)
	{
		if ((controlsMng.IsPressedInput(_event, "INTERACT") || controlsMng.IsPressedInput(_event, "INTERACT_2")))
		{
			Audio::Get().PlaySound("DODGE");
			SetPlayerInteractState(PlayerState::DODGE);
		}
		else if (controlsMng.IsPressedInput(_event, "HIT"))
		{
			SetPlayerInteractState(PlayerState::COUNTER);
			if (player->GetCostumeState() == CostumeState::MUSIC)
			{
				DrawManager::Get().SetRenderTextureShader("FIGHT_BG", &ShadersManager::Get()["VOID_OUT"]);
				voidTime = 0;
				shaderTotalTime = 0;

				counteredOneTime = true;
			}
		}
	}
}

void MiniGame::UpdatePlayerDefense(void)
{
	float factor;
	playerCoolDown -= GameClock::Get().GetDeltaTime();

	switch (player->GetState())
	{
	case(PlayerState::DODGE):
		playerActionTime += GameClock::Get().GetDeltaTime();
		factor = 1.f - fabsf(Easings::OutQuart(Maths::Clamp(playerActionTime / PLAYER_DODGE_DURATION, 0.f, 1.f)) * 2.f - 1.f);
		player->setPosition(playerInitPos.x - factor * PLAYER_DODGE_OFFSET, playerInitPos.y);

		for (int i = 0; i < PLAYER_CLONE_SIZE; i++)
		{
			playerCloneActionTime[i] += GameClock::Get().GetDeltaTime();
			playerClone[i] = player->GetSprite();
			playerClone[i].setOrigin(player->getOrigin());
			playerClone[i].setScale(player->getScale());
			factor = 1.f - fabsf(Easings::OutQuart(Maths::Clamp((playerActionTime - 0.6f / i + playerCloneActionTime[i]) / PLAYER_DODGE_DURATION, 0.f, 1.f)) * 2.f - 1.f);
			playerClone[i].setColor(sf::Color(255, 255, 255, PLAYER_DODGE_TRANSPARENCY));
			playerClone[i].setPosition(playerInitPos.x - factor * PLAYER_DODGE_OFFSET, playerInitPos.y);
		}

		if (playerActionTime > 0.6f)
		{
			playerActionTime = 0.f;
			SetPlayerInteractState(PlayerState::IDLE);
			playerCoolDown = PLAYER_COOLDOWN_DODGE;
		}
		break;

	case(PlayerState::COUNTER):
		if (player->GetCurAnim()->GetCurrentFrame() == player->GetCurAnim()->GetFrameCount() - 1)
		{
			SetPlayerInteractState(PlayerState::IDLE);
			playerCoolDown = PLAYER_COOLDOWN_COUNTER;
		}
		break;

	default:
		break;
	}
}

void MiniGame::SetActiveDef(const bool& _bool)
{
	playerActiveDef = _bool;
}

void MiniGame::SetPlayerInteractState(const PlayerState& _playerState)
{
	const PlayerState lastState = player->GetState();
	player->SetState(_playerState);
	player->GetCurAnim()->Reset();
	player->GetCurAnim()->Play();

	// reset last state
	switch (lastState)
	{
	case PlayerState::DODGE:
		for (int i = 0; i < PLAYER_CLONE_SIZE; i++)
		{
			DrawManager::Get().SetObjectVisible("FIGHT", &playerClone[i], false);
			playerCloneActionTime[i] = 0.f;
		}
		player->setPosition(playerInitPos);
		break;
	default:
		break;
	}

	// init new state
	switch (_playerState)
	{
	case PlayerState::DODGE:
		for (int i = 0; i < PLAYER_CLONE_SIZE; i++)
		{
			playerClone[i] = player->GetCurAnim()->GetSprite();
			playerClone[i].setPosition(player->getPosition());
			DrawManager::Get().SetObjectVisible("FIGHT", &playerClone[i]);
		}
		break;
	default:
		break;
	}
}

void MiniGame::CallResult(const ResultAttack& _result)
{
	Screen& windowManager = Screen::Get();
	TexturesManager& textureManager = TexturesManager::Get();
	textResultTime = 0.f;
	switch (_result)
	{
	case MiniGame::ResultAttack::MISS:
		textResult.setTexture(textureManager["MISS_TEXT_FIGHTING"], true);
		break;
	case MiniGame::ResultAttack::OK:
		textResult.setTexture(textureManager["OK_TEXT_FIGHTING"], true);
		break;
	case MiniGame::ResultAttack::GREAT:
		textResult.setTexture(textureManager["GOOD_TEXT_FIGHTING"], true);
		break;
	case MiniGame::ResultAttack::EXCELENT:
		textResult.setTexture(textureManager["EXCELENT_TEXT_FIGHTING"], true);
		break;
	case MiniGame::ResultAttack::PERFECT:
		textResult.setTexture(textureManager["PERFECT_TEXT_FIGHTING"], true);
		break;
	default:
		break;
	}

	sf::FloatRect rect = textResult.getGlobalBounds();
	textResult.setOrigin(rect.width / 2.f, rect.height / 2.f);
	textResult.setPosition(WORKING_WIDTH / 2.f, 256.f);

	textResult.setRotation(Maths::PreciseRand(25, -25));
	textResult.setColor(sf::Color::White);
}

void MiniGame::CallResult(const ResultDodge& _result)
{
	Screen& windowManager = Screen::Get();
	TexturesManager& textureManager = TexturesManager::Get();
	textResultTime = 0.f;
	switch (_result)
	{
	case MiniGame::ResultDodge::ATTACKED:
		break;
	case MiniGame::ResultDodge::BLOCKED:
		textResult.setTexture(textureManager["BLOCKED_TEXT_FIGHTING"], true);
		break;
	case MiniGame::ResultDodge::DODGE:
		textResult.setTexture(textureManager["DODGED_TEXT_FIGHTING"], true);
		break;
	default:
		break;
	}

	sf::FloatRect rect = textResult.getGlobalBounds();
	textResult.setOrigin(rect.width / 2.f, rect.height / 2.f);
	textResult.setPosition(WORKING_WIDTH / 2.f, 256.f);

	textResult.setRotation(Maths::PreciseRand(25, -25));
	textResult.setColor(sf::Color::White);
}

const sf::Vector2f MiniGame::GetShakingPos(const sf::Vector2f& _currentPos, const sf::Vector2f& _center, const float& _offset)
{
	sf::Vector2f newPosition = _currentPos;
	newPosition.x = (newPosition.x < _center.x ? _center.x + _offset : _center.x - _offset);
	return newPosition;
}