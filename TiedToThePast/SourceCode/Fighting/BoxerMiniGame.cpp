#include "BoxerMiniGame.hpp"
#include "../Player.hpp"
#include "../Enemy.hpp"
#include "TextDamage.hpp"

BoxerMiniGame::BoxerMiniGame(void)
{
	// Sounds 
	audio.AddBuffer("Contents/Assets/Audio/Sounds/Player/PunchMiss.wav", "FIGHT_PUNCH_MISS");
	audio.AddBuffer("Contents/Assets/Audio/Sounds/Player/PunchOk.wav", "FIGHT_PUNCH_OK");
	audio.AddBuffer("Contents/Assets/Audio/Sounds/Player/PunchGreat.wav", "FIGHT_PUNCH_GREAT");
	audio.AddSoundWithBuffer("FIGHT_PUNCH_OK", "FIGHT_PUNCH_HIT");

	DrawManager::Get().AddObject("FIGHT_HUD", keyInteract);

	particlePressedTouch = new ParticleSystem("Contents/Data/Particles/PressedTouch.json");
	particlePressedTouch->setPosition(-500.f, -500.f);
	particlePressedTouch->SetStatue(ParticleSystem::Statue::STOPPED);

	DrawManager::Get().AddObject("FIGHT_HUD", *particlePressedTouch);
}

BoxerMiniGame::~BoxerMiniGame(void)
{
	audio.DeleteSound("FIGHT_PUNCH_HIT");
	audio.DeleteBuffer("FIGHT_PUNCH_MISS");
	audio.DeleteBuffer("FIGHT_PUNCH_OK");
	audio.DeleteBuffer("FIGHT_PUNCH_GREAT");

	DrawManager::Get().DeleteObject("FIGHT_HUD", &keyInteract);

	if (particlePressedTouch != nullptr)
	{
		delete particlePressedTouch;
		particlePressedTouch = nullptr;

		DrawManager::Get().DeleteObject("FIGHT_HUD", particlePressedTouch);
	}
}

void BoxerMiniGame::InitMiniGame(std::shared_ptr<Player>& _player, std::shared_ptr<Enemy>& _enemy, FightingState* _fightingState)
{
	clickCount = 0;
	state = static_cast<StateFunc>(&BoxerMiniGame::Start);

	initPlayerDirection = player->GetXDir();

	keyInteract.setTexture(ControlsManager::Get().GetTexture("INTERACT"));
	keyInteract.setOrigin(keyInteract.getGlobalBounds().width / 2, keyInteract.getGlobalBounds().height / 2);
	keyInteract.setScale(1, 1);
	keyInteract.setPosition(-500, -500);
	player->SetXDir(initPlayerDirection);
	


	burnOut = false;
}

void BoxerMiniGame::UpdateMiniGame(FightingState* _fightingState)
{
	if (this->state)
	{
		(this->*state)(_fightingState);
	}
}

void BoxerMiniGame::PollEventMiniGame(sf::Event& _event, FightingState* _fightingState)
{
	if (stateEvent == StateEvent::PLAYER_INTERACTION)
	{
		if (ControlsManager::Get().IsPressedInput(_event, "INTERACT") || ControlsManager::Get().IsPressedInput(_event, "INTERACT_2"))
		{
			clickCount++;

			// Player shake
			sf::Vector2f shakePos = (enemyInitPos.x - ATTACK_OFFSET_TO_ENEMY.x > player->getPosition().x ? sf::Vector2f(1.f, 0) : sf::Vector2f(-1.f, 0));
			player->setPosition(enemyInitPos - ATTACK_OFFSET_TO_ENEMY + shakePos);
		}
	}
}

void BoxerMiniGame::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	//if (state == &BoxerMiniGame::On && stateEvent == StateEvent::PLAYER_INTERACTION)
	//{
	//	target.draw(*particlePressedTouch, states);
	//	target.draw(keyInteract, states);
	//}
	target.draw(textResult, states);
}

void BoxerMiniGame::Start(FightingState* _fightingState)
{
	player->SetState(PlayerState::MOVE);

	// Calculate the factor for the ease-in ease-out animation
	float factor = (float)Easings::InOutCubic(Maths::Clamp(totalTime / START_TIME, 0.f, 1.f));

	// Set the new position for the player
	sf::Vector2f newPosition = playerInitPos + Maths::CreateVector(playerInitPos, enemyInitPos - ATTACK_OFFSET_TO_ENEMY) * factor;

	// Apply new player position
	player->setPosition(newPosition);

	if (totalTime >= START_TIME) StartOver(_fightingState);
}

void BoxerMiniGame::StartOver(FightingState* _fightingState)
{
	player->SetState(PlayerState::CHARGING_ATTACK);
	player->GetCurAnim()->Pause();
	
	totalTime = 0;
	state = static_cast<StateFunc>(&BoxerMiniGame::On);

	stateEvent = StateEvent::PLAYER_INTERACTION;
}

void BoxerMiniGame::On(FightingState* _fightingState)
{
	DrawManager::Get().SetObjectVisible("FIGHT_HUD", particlePressedTouch, true);
	DrawManager::Get().SetObjectVisible("FIGHT_HUD", &keyInteract, true);
	float dt = GameClock::Get().GetDeltaTime();
	keyInteract.setPosition(player->getPosition().x, player->getPosition().y - player->GetGlobalAABB().height - 50);
	particlePressedTouch->setPosition(keyInteract.getPosition());

	particlePressedTouch->Update();
	if (particlePressedTouch->GetStatue() == ParticleSystem::Statue::STOPPED)
	{
		particlePressedTouch->Reset();
	}

	if (keyInteract.getScale().x == PRESSED_SCALE_BOXER)
	{
		timerResetScale -= dt;
		if (timerResetScale <= 0.f)
		{
			particlePressedTouch->Play();
			keyInteract.setScale(1.f, 1.f);
			timerResetScale = TIMER_SCALE_MAX_BOXER;
		}
	}
	else
	{
		timerResetScale -= dt;
		if (timerResetScale <= 0.f)
		{
			keyInteract.setScale(PRESSED_SCALE_BOXER, PRESSED_SCALE_BOXER);

			timerResetScale = TIMER_SCALE_MAX_BOXER;
		}
	}

	if (stateEvent == StateEvent::PLAYER_INTERACTION)
	{
		if (clickCount >= ((float)MAX_BAR_SCORE * BURNOUT_WIDTH / MAX_BAR_WIDTH))
		{
			player->GetCurAnim()->SetCurrentFrame(4);
			player->SetBoxerAttackAnimation(1);
			burnOut = true;
		}
		else if (clickCount >= ((float)MAX_BAR_SCORE * EXCELLENT_WIDTH / MAX_BAR_WIDTH))
		{
			player->GetCurAnim()->SetCurrentFrame(3);
		}
		else if (clickCount >= ((float)MAX_BAR_SCORE * GREAT_WIDTH / MAX_BAR_WIDTH))
		{
			player->GetCurAnim()->SetCurrentFrame(2);
		}
		else if (clickCount >= ((float)MAX_BAR_SCORE * OK_WIDTH / MAX_BAR_WIDTH))
		{
			player->GetCurAnim()->SetCurrentFrame(1);
		}
		if (totalTime >= MAX_BAR_TIME)
		{
			player->GetCurAnim()->Reset();
			stateEvent = StateEvent::NOT_INTERACTION;
			player->SetState(PlayerState::ATTACK);
			player->GetCurAnim()->Play();
			totalTime = 0.f;
		}
	}
	else if (stateEvent == StateEvent::NOT_INTERACTION && clickCount >= ((float)MAX_BAR_SCORE * OK_WIDTH / MAX_BAR_WIDTH) && clickCount <= ((float)MAX_BAR_SCORE * BURNOUT_WIDTH / MAX_BAR_WIDTH))
	{
		// Calculate the factor for the ease-in ease-out animation
		float factor = (float)Easings::Square(Maths::Clamp(totalTime / MAX_ATTACK_BOXER_TIME, 0.f, 1.f));

		// Set the new position for the player
		sf::Vector2f newPosition = enemyInitPos - ATTACK_OFFSET_TO_ENEMY + Maths::CreateVector(enemyInitPos - ATTACK_OFFSET_TO_ENEMY, enemyInitPos - sf::Vector2f(32.f, 0.f)) * factor;

		// Apply new player position
		player->setPosition(newPosition);

		if (totalTime >= MAX_ATTACK_BOXER_TIME)
		{
			totalTime = 0.f;
			OnOver(_fightingState);
		}
	}
	else
	{
		if (totalTime >= MAX_ATTACK_BOXER_TIME)
		{
			totalTime = 0.f;
			OnOver(_fightingState);
		}
	}
}

void BoxerMiniGame::OnOver(FightingState* _fightingState)
{
	DrawManager::Get().SetObjectVisible("FIGHT_HUD", particlePressedTouch, false);
	DrawManager::Get().SetObjectVisible("FIGHT_HUD", &keyInteract, false);

	player->SetXDir(initPlayerDirection * -1);
	player->SetState(PlayerState::MOVE);

	int damage = GetDamage();

	enemy->AddHP(-damage);

	if (damage > 0)
	{
		// Create a new TextDamage object and add it to the list of damage texts
		std::string toDisplay = std::to_string(damage);
		enemy->textDamages.push_back(std::make_shared<TextDamage>(toDisplay, enemyInitPos - sf::Vector2f(0.f, enemy->GetGlobalAABB().height / 4.f) + sf::Vector2f(Maths::PreciseRand(50.f, -50.f), -enemy->GetSprite().getGlobalBounds().height)));
		if (player->GetCurTie()->GetLevel() > 2 && clickCount >= ((float)MAX_BAR_SCORE * EXCELLENT_WIDTH / MAX_BAR_WIDTH))
		{
			player->ApplyEffect(enemy);
		}
		player->GetApplicableEffects();

		std::shared_ptr<ParticleSystem> curParticle = enemy->GetParticles()[(int)Entity::ParticleTypes::DAMAGE];
		curParticle->setPosition(enemy->getPosition().x, enemy->getPosition().y - enemy->GetGlobalAABB().height / 2.f);
		curParticle->setRotation(270.f + Maths::PreciseRand(20.f, -20.f));
		curParticle->Reset();
		curParticle->Play();
		enemy->SetAnimationState(Enemy::AnimationState::DAMAGE);
		enemy->GetCurAnim()->Reset();
		enemy->GetCurAnim()->Play();
	}

	if (burnOut)
	{
		damage = 10;
		player->AddHP(-damage);
		std::string toDisplay = std::to_string(damage);
		player->textDamages.push_back(std::make_shared<TextDamage>(toDisplay, player->getPosition() - sf::Vector2f(50.f, player->GetGlobalAABB().height / 4.f) + sf::Vector2f(0.f, - player->GetSprite().getGlobalBounds().height)));
		std::shared_ptr<ParticleSystem> curParticle = enemy->GetParticles()[(int)Entity::ParticleTypes::DAMAGE];
		curParticle->setPosition(enemy->getPosition().x, enemy->getPosition().y - enemy->GetGlobalAABB().height / 2.f);
		curParticle->setRotation(90.f + Maths::PreciseRand(20.f, -20.f));
		curParticle->Reset();
		curParticle->Play();
	}
	else if(!burnOut && damage <= 0)
	{
		std::string toDisplay = std::to_string(damage);
		enemy->textDamages.push_back(std::make_shared<TextDamage>(toDisplay, enemyInitPos - sf::Vector2f(0.f, enemy->GetGlobalAABB().height / 4.f) + sf::Vector2f(Maths::PreciseRand(50.f, -50.f), -enemy->GetSprite().getGlobalBounds().height)));
	}

	totalTime = 0.f;
	state = static_cast<StateFunc>(&BoxerMiniGame::End);

	audio.PlaySound("FIGHT_PUNCH_HIT");
}

void BoxerMiniGame::End(FightingState* _fightingState)
{
	// Calculate the factor for the ease-in ease-out animation
	float factor = (float)Easings::InOutCubic(Maths::Clamp(totalTime / END_TIME, 0.f, 1.f));

	// Calcul the new position for the player
	sf::Vector2f newPosition = enemyInitPos - ATTACK_OFFSET_TO_ENEMY + Maths::CreateVector(enemyInitPos - ATTACK_OFFSET_TO_ENEMY, playerInitPos) * factor;

	// Apply new player position
	player->setPosition(newPosition);

	if (totalTime >= END_TIME) EndOver(_fightingState);
}

void BoxerMiniGame::EndOver(FightingState* _fightingState)
{
	player->SetBoxerAttackAnimation(0);
	player->SetXDir(initPlayerDirection);
	player->SetState(PlayerState::IDLE);

	// Reset minigame
	clickCount = 0;

	DrawManager::Get().SetObjectVisible("FIGHT_HUD", particlePressedTouch, false);
	//DrawManager::Get().DeleteObject("FIGHT_HUD", particlePressedTouch);

	state = static_cast<StateFunc>(&BoxerMiniGame::Start);

	FinishMiniGame();
}

int BoxerMiniGame::GetDamage()
{
	const AttackType& attackType = player->GetCurTie()->GetAttackType();
	if (clickCount >= ((float)MAX_BAR_SCORE * BURNOUT_WIDTH / MAX_BAR_WIDTH))
	{
		CallResult(ResultAttack::MISS);
		audio.ChangeBuffer("FIGHT_PUNCH_HIT", "FIGHT_PUNCH_MISS");
		audio.SetSoundVolume("FIGHT_PUNCH_HIT", 60.f);
		return int(player->GetDamage() * BOXER_MISS_MULTIPLICATOR * enemy->GetResistances()[attackType] * player->GetCurTie()->GetLevelCoef());
	}
	else if (clickCount >= ((float)MAX_BAR_SCORE * EXCELLENT_WIDTH / MAX_BAR_WIDTH))
	{
		CallResult(ResultAttack::EXCELENT);
		audio.ChangeBuffer("FIGHT_PUNCH_HIT", "PUNCH_HIT");
		audio.SetSoundVolume("FIGHT_PUNCH_HIT");
		return int(player->GetDamage() * BOXER_PERFECT_EXCELLENT_MULTIPLICATOR * enemy->GetResistances()[attackType] * player->GetCurTie()->GetLevelCoef());
	}
	else if (clickCount >= ((float)MAX_BAR_SCORE * GREAT_WIDTH / MAX_BAR_WIDTH))
	{
		CallResult(ResultAttack::GREAT);
		audio.ChangeBuffer("FIGHT_PUNCH_HIT", "FIGHT_PUNCH_GREAT");
		audio.SetSoundVolume("FIGHT_PUNCH_HIT");
		return int(player->GetDamage() * BOXER_GREAT_MULTIPLICATOR * enemy->GetResistances()[attackType] * player->GetCurTie()->GetLevelCoef());
	}
	else if (clickCount >= ((float)MAX_BAR_SCORE * OK_WIDTH / MAX_BAR_WIDTH))
	{
		CallResult(ResultAttack::OK);
		audio.ChangeBuffer("FIGHT_PUNCH_HIT", "FIGHT_PUNCH_OK");
		audio.SetSoundVolume("FIGHT_PUNCH_HIT", 90.f);
		return int(player->GetDamage() * BOXER_OK_MULTIPLICATOR * enemy->GetResistances()[attackType] * player->GetCurTie()->GetLevelCoef());
	}
	else
	{
		CallResult(ResultAttack::MISS);
		audio.ChangeBuffer("FIGHT_PUNCH_HIT", "FIGHT_PUNCH_MISS");
		audio.SetSoundVolume("FIGHT_PUNCH_HIT", 60.f);
		return int(player->GetDamage() * BOXER_MISS_MULTIPLICATOR * enemy->GetResistances()[attackType] * player->GetCurTie()->GetLevelCoef());
	}
}