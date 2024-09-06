#include "Definitions.hpp"
#include "Encounter.hpp"

Encounter::Encounter(void)
{
}
Encounter::~Encounter(void)
{
}

void Encounter::Init(void)
{
	playerBg.setTexture(TexturesManager::Get()["BgPlayerEncounter"]);
	enemiesBg.setTexture(TexturesManager::Get()["BgEnemyEncounter"]);

	DrawManager::Get().AddObject("FIGHT_BG", playerBg, false);
	DrawManager::Get().AddObject("FIGHT_BG", enemiesBg, false);

	playerBg.setOrigin(playerBg.getGlobalBounds().width / 2.f, playerBg.getGlobalBounds().height / 2.f);
	enemiesBg.setOrigin(enemiesBg.getGlobalBounds().width / 2.f, enemiesBg.getGlobalBounds().height / 2.f);

	playerStartPos = sf::Vector2f(2.f * float(WORKING_WIDTH), 2.f * float(WORKING_HEIGHT));
	enemyStartPos = sf::Vector2f(-float(WORKING_WIDTH), -float(WORKING_HEIGHT));
}

void Encounter::Start(Player& _player, std::vector<std::shared_ptr<Enemy>> _enemies)
{
	timer.Restart();
	timer.SetNewTimerDuration(5.f);
	timer.Run();

	sf::FloatRect plyRect = _player.GetSprite().getGlobalBounds();
	_player.SetLastPosExplo(_player.getPosition());
	_player.setPosition(PLAYER_START_POS.x, PLAYER_START_POS.y + plyRect.height / 2.f);
	_player.setScale(-1.f, 1.f);
	_player.SetState(PlayerState::IDLE);

	playerBg.setPosition(0.f, WORKING_HEIGHT);
	enemiesBg.setPosition(WORKING_WIDTH, 0.f);

	_player.setPosition(PLAYER_START_POS);

	for (int i = 0; i < _enemies.size(); i++)
	{
		_enemies[i]->setPosition(ENEMIES_START_POS[i]);
	}

	sf::Color enemyBG = enemiesBg.getColor();
	sf::Color playerBG = playerBg.getColor();

	enemyBG.a = sf::Uint8(255);
	playerBG.a = sf::Uint8(255);

	enemiesBg.setColor(enemyBG);
	playerBg.setColor(playerBG);

	DrawManager::Get().SetObjectVisible("FIGHT_BG", &enemiesBg, true);
	DrawManager::Get().SetObjectVisible("FIGHT_BG", &playerBg, true);
}

void Encounter::Event(sf::Event& _event, Player& _player, std::vector<std::shared_ptr<Enemy>> _enemies)
{
	auto& ControlsMng = ControlsManager::Get();
	if (ControlsMng.IsPressedInput(_event, "INTERACT") || ControlsMng.IsPressedInput(_event, "INTERACT_2"))
	{
		timer.Stop();
		Audio::Get().StopSound("FIGHT_INTRODUCTION");

		End(_player, _enemies);
	}
}

void Encounter::Update(Player& _player, std::vector<std::shared_ptr<Enemy>> _enemies)
{
	timer.Update();

	// +0.5f to let animation enough time to finish & because even if that work good I want a delta before player & enemies apparition
	if (timer.GetTimePassed() <= Encounter::TIME_BG_CLASH + Encounter::DELTA_TIME_BG_CLASH)
	{
		const float factor = (float)Easings::InOutBack(Maths::Clamp(timer.GetTimePassed() / TIME_BG_CLASH, 0.f, 1.f));
		// Set the new position for the player
		const sf::Vector2f newPositionPlayerBG = playerStartPos + Maths::CreateVector(playerStartPos, END_POS_BG) * factor;
		const sf::Vector2f newPositionEnemyBG = enemyStartPos + Maths::CreateVector(enemyStartPos, END_POS_BG) * factor;

		playerBg.setPosition(newPositionPlayerBG);
		enemiesBg.setPosition(newPositionEnemyBG);
	}
	else if (timer.GetTimePassed() > TIME_BG_CLASH + DELTA_TIME_BG_CLASH)
	{
		const float factor = Easings::InQuad(Maths::Clamp((timer.GetTimePassed() - TIME_BG_CLASH - DELTA_TIME_BG_CLASH) / 0.5f, 0.f, 1.f));

		_player.setPosition(PLAYER_START_POS + Maths::CreateVector(PLAYER_START_POS, PLAYER_FIGHT_POS) * factor);

		for (int i = 0; i < _enemies.size(); i++)
		{
			_enemies[i]->setPosition(ENEMIES_START_POS[i] - Maths::CreateVector(ENEMIES_FIGHT_POS[i], ENEMIES_START_POS[i]) * factor);
		}
	}

	if (timer.GetTimeLeft() < 0.5f)
	{
		sf::Color enemyBG = enemiesBg.getColor();
		sf::Color playerBG = playerBg.getColor();

		const float factor = Easings::InQuad(Maths::Clamp(timer.GetTimeLeft() / 0.5f, 0.f, 1.f));
		enemyBG.a = sf::Uint8(255 * factor);
		playerBG.a = sf::Uint8(255 * factor);

		enemiesBg.setColor(enemyBG);
		playerBg.setColor(playerBG);
	}

	DrawManager::Get().SetObjectVisible("FIGHT", &_player, timer.GetTimePassed() > TIME_BG_CLASH + DELTA_TIME_BG_CLASH);
	for (auto& enemy : _enemies)
	{
		DrawManager::Get().SetObjectVisible("FIGHT", enemy.get(), timer.GetTimePassed() > TIME_BG_CLASH + DELTA_TIME_BG_CLASH);
	}

	if (timer.IsOver())
	{
		End(_player, _enemies);
	}
}

const bool Encounter::IsFinished(void)
{
	return timer.GetState() == Timer::State::STOPPED || timer.IsOver();
}

const Timer& Encounter::GetTimer(void) const
{
	return timer;
}

void Encounter::End(Player& _player, std::vector<std::shared_ptr<Enemy>> _enemies)
{
	_player.setPosition(PLAYER_FIGHT_POS);

	for (int i = 0; i < _enemies.size(); i++)
	{
		_enemies[i]->setPosition(ENEMIES_FIGHT_POS[i]);
	}

	DrawManager::Get().SetObjectVisible("FIGHT_BG", &enemiesBg, false);
	DrawManager::Get().SetObjectVisible("FIGHT_BG", &playerBg, false);
}