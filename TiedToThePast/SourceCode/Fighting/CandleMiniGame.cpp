#include "CandleMiniGame.hpp"
#include "../Player.hpp"
#include "../Enemy.hpp"

CandleMiniGame::CandleMiniGame()
{
	fireballAnim = AnimatedSprite("Contents/Data/Animations/Other/fireballFighting.json");
	fireballAnim.GetSprite().setPosition(-500.f, -500.f);
}

CandleMiniGame::~CandleMiniGame()
{
}

void CandleMiniGame::InitMiniGame(std::shared_ptr<Player>& _player, std::shared_ptr<Enemy>& _enemy, FightingState* _fightingState)
{
	indexFireball = 0;
	state = static_cast<StateFunc>(&CandleMiniGame::Start);

	keyInteract.setTexture(ControlsManager::Get().GetTexture("INTERACT"));
	keyInteract.setOrigin(keyInteract.getGlobalBounds().width / 2, keyInteract.getGlobalBounds().height / 2.f);
	keyInteract.setScale(1, 1);
	keyInteract.setPosition(player->getPosition().x - player->GetGlobalAABB().width, player->getPosition().y - player->GetGlobalAABB().height/2.f);

	sf::FloatRect playerGB = player->GetSprite().getGlobalBounds();
	sf::Vector2f plyPos = player->getPosition();
	interactKeyTime = 0;
	fireballAnim.GetSprite().setScale(-1,1);
	fireballAnim.Reset();
	levelFireball = 1;
	isThrowing = false;
	isThrowingPerfect = false;
	fireballLaunched = false;
	keyIsAppearing = true;
	currentDamage = 0;
	startPosition = sf::Vector2f(plyPos.x, plyPos.y - playerGB.height);

	if (enemy->GetName() != "BIG BOO")
	{
		endPosition = { enemy->getPosition().x, enemy->getPosition().y - enemy->GetGlobalAABB().height / 2 };
	}
	else
	{
		endPosition = { enemy->getPosition().x, enemy->getPosition().y - enemy->GetGlobalAABB().height * 0.75f };
	}
}


void CandleMiniGame::UpdateMiniGame(FightingState* _fightingState)
{
	if (this->state)
	{
		(this->*state)(_fightingState);
	}

	const float& dt = GameClock::Get().GetDeltaTime();

	if (keyIsAppearing)
	{
		interactKeyTime += dt;
	}
	else
	{
		interactKeyTime -= dt;
	}
	
	if (interactKeyTime >= ANIM_FIREBALL_DURATION / fireballAnim.GetSpeed())
	{
		interactKeyTime = ANIM_FIREBALL_DURATION / fireballAnim.GetSpeed() - (interactKeyTime - ANIM_FIREBALL_DURATION / fireballAnim.GetSpeed());
		keyIsAppearing = false;
	}
	else if (interactKeyTime <= 0.f)
	{
		keyIsAppearing = true;
		interactKeyTime = -interactKeyTime;
	}

	if (stateEvent == StateEvent::PLAYER_INTERACTION)
	{
		fireballAnim.Update();
		indexFireball = fireballAnim.GetCurrentFrame() % 14;
	}
	if (state == &CandleMiniGame::On)
	{
		float factor = Maths::Clamp(interactKeyTime / (ANIM_FIREBALL_DURATION / fireballAnim.GetSpeed()), 0.f, 1.f);
		sf::Color keyAlpha;
		keyAlpha.r = 255;
		keyAlpha.g = 255;
		keyAlpha.b = 255;
		keyAlpha.a = sf::Uint8(255 * factor);
		keyInteract.setColor(keyAlpha);
	}

	fireballAnim.SetSpeed(levelFireball * 1.65f);

	sf::FloatRect fireballRect = fireballAnim.GetSprite().getGlobalBounds();
	fireballAnim.GetSprite().setOrigin(fireballRect.width / 2.f, fireballRect.height / 2.f);
}

void CandleMiniGame::PollEventMiniGame(sf::Event& _event, FightingState* _fightingState)
{
	if (stateEvent == StateEvent::PLAYER_INTERACTION)
	{
		if (ControlsManager::Get().IsPressedInput(_event, "INTERACT") || ControlsManager::Get().IsPressedInput(_event, "INTERACT_2"))
		{
			audio.PlaySound("FIREBALL_LAUNCH");

			if (indexFireball == 7 && player->GetCurTie()->GetLevel() > 1)
			{
				this->levelFireball++;
				this->currentDamage = this->GetDamage();
				isThrowingPerfect = true;
			}
			else
			{
				this->currentDamage = this->GetDamage();
			}

			if (indexFireball <= 7)
			{
				player->SetCandleAttackAnimation(indexFireball);
			}
			else
			{
				player->SetCandleAttackAnimation(14 - indexFireball);
			}

			isThrowing = true;

		}
	}
}

void CandleMiniGame::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	if (stateEvent == StateEvent::PLAYER_INTERACTION || (stateEvent == StateEvent::FIREBALL_ANIMATION && fireballLaunched))
	{
		target.draw(fireballAnim, states);
	}
	if (this->state == &CandleMiniGame::On && stateEvent == StateEvent::PLAYER_INTERACTION)
	{
		target.draw(keyInteract, states);
	}
	target.draw(textResult, states);
}

void CandleMiniGame::Start(FightingState* _fightingState)
{
	if (totalTime >= START_TIME_CANDLE) StartOver(_fightingState);
}

void CandleMiniGame::StartOver(FightingState* _fightingState)
{
	totalTime = 0;
	state = static_cast<StateFunc>(&CandleMiniGame::On);

	stateEvent = StateEvent::PLAYER_INTERACTION;
	player->SetCandleIdleAnimation(0);
	player->SetState(PlayerState::IDLE);
}

void CandleMiniGame::On(FightingState* _fightingState)
{
	if (stateEvent == StateEvent::PLAYER_INTERACTION)
	{
		fireballAnim.GetSprite().setPosition(startPosition);
		if (isThrowing)
		{
			player->SetState(PlayerState::ATTACK);
			player->GetCurAnim()->Reset();
			player->GetCurAnim()->Play();
			stateEvent = StateEvent::FIREBALL_ANIMATION;
			totalTime = 0.f;
			player->SetCandleIdleAnimation(1);
		}
		audio.PlaySound("FIRE_NOISE", 40.f);
	}
	else if (stateEvent == StateEvent::FIREBALL_ANIMATION || stateEvent == StateEvent::NOT_INTERACTION)
	{
		audio.StopSound("FIRE_NOISE");
		// Calculate the factor for the ease-in ease-out animation


		// Set the new position for the fireball 


		// Apply new player position
		if (player->GetCurAnim()->GetCurrentFrame() >= 3 && !fireballLaunched)
		{
			totalTime = 0.f;
			fireballLaunched = true;
		}

		if (fireballLaunched)
		{
			float factor = (float)totalTime;
			sf::Vector2f newPosition = startPosition + Maths::CreateVector(startPosition, endPosition) * factor;
			fireballAnim.GetSprite().setPosition(newPosition);
		}

		sf::FloatRect playerGB = player->GetSprite().getGlobalBounds();
		if (isThrowingPerfect)
		{
			if (totalTime > END_TIME_FIREBALL)
			{
				stateEvent = StateEvent::NOT_INTERACTION;
				std::string toDisplay = std::to_string(currentDamage);
				interactKeyTime = 0;
				enemy->AddHP(-currentDamage);

				enemy->SetAnimationState(Enemy::AnimationState::DAMAGE);
				enemy->GetCurAnim()->Reset();
				enemy->GetCurAnim()->Play();

				enemy->textDamages.push_back(std::make_shared<TextDamage>(toDisplay, endPosition - sf::Vector2f(0.f, enemy->GetGlobalAABB().height / 4.f) + sf::Vector2f(Maths::PreciseRand(50.f, -50.f), -80.f)));
				player->ApplyEffect(enemy);

				enemy->GetParticles()[(int)Entity::ParticleTypes::DAMAGE]->setPosition(enemy->getPosition().x, enemy->getPosition().y - enemy->GetGlobalAABB().height / 2.f);
				enemy->GetParticles()[(int)Entity::ParticleTypes::DAMAGE]->setRotation(180.f + Maths::PreciseRand(20.f, -20.f));
				enemy->GetParticles()[(int)Entity::ParticleTypes::DAMAGE]->Reset();
				enemy->GetParticles()[(int)Entity::ParticleTypes::DAMAGE]->Play();

				if (enemy->GetHP() <= 0)
				{
					OnOver(_fightingState);
				}
				else
				{
					stateEvent = StateEvent::PLAYER_INTERACTION;
					player->SetCandleIdleAnimation(0);
					player->SetState(PlayerState::IDLE);
					isThrowingPerfect = false;
					isThrowing = false;
					fireballLaunched = false;
					totalTime = 0.f;
					this->fireballAnim.Reset();
				}
				fireballAnim.GetSprite().setPosition(startPosition);
				audio.PlaySound("FIRE_IMPACT");
			}

		}
		else if (totalTime > END_TIME_FIREBALL)
		{
			std::string toDisplay = std::to_string(currentDamage);
			interactKeyTime = 0;
			enemy->AddHP(-currentDamage);
			enemy->textDamages.push_back(std::make_shared<TextDamage>(toDisplay, endPosition - sf::Vector2f(0.f, enemy->GetGlobalAABB().height / 4.f) + sf::Vector2f(Maths::PreciseRand(50.f, -50.f), -80.f)));
			if (currentDamage > 0)
			{
				player->ApplyEffect(enemy);

				enemy->SetAnimationState(Enemy::AnimationState::DAMAGE);
				enemy->GetCurAnim()->Reset();
				enemy->GetCurAnim()->Play();

				enemy->GetParticles()[(int)Entity::ParticleTypes::DAMAGE]->setPosition(enemy->getPosition().x, enemy->getPosition().y - enemy->GetGlobalAABB().height / 2.f);
				enemy->GetParticles()[(int)Entity::ParticleTypes::DAMAGE]->setRotation(270.f + Maths::PreciseRand(20.f, -20.f));
				enemy->GetParticles()[(int)Entity::ParticleTypes::DAMAGE]->Reset();
				enemy->GetParticles()[(int)Entity::ParticleTypes::DAMAGE]->Play();
			}
			

			fireballAnim.GetSprite().setPosition(startPosition);
			stateEvent = StateEvent::NOT_INTERACTION;
			audio.PlaySound("FIRE_IMPACT");
			OnOver(_fightingState);
		}
	}
}

void CandleMiniGame::OnOver(FightingState* _fightingState)
{
	totalTime = 0;
	state = static_cast<StateFunc>(&CandleMiniGame::End);
}

void CandleMiniGame::End(FightingState* _fightingState)
{
	if (currentDamage > 0 && totalTime >= END_TIME_CANDLE) EndOver(_fightingState);
	else if (currentDamage <= 0 && totalTime >= 1.f) EndOver(_fightingState);
}

void CandleMiniGame::EndOver(FightingState* _fightingState)
{
	// Make sure the position is correctly display
	this->levelFireball = 1;
	this->indexFireball = -1;
	stateEvent = StateEvent::NOT_INTERACTION;
	isThrowing = false;
	isThrowingPerfect = false;
	fireballLaunched = false;
	this->currentDamage = 0;
	this->fireballAnim.Reset();

	// Reset minigame
	totalTime = 0;
	over = true;

	state = static_cast<StateFunc>(&CandleMiniGame::Start);
}

int CandleMiniGame::GetDamage()
{
	const AttackType& attackType = player->GetCurTie()->GetAttackType();

	if (indexFireball == 7)
	{
		CallResult(ResultAttack::PERFECT);
		audio.ChangeBuffer("FIRE_IMPACT", "FIRE_PERFECT");
		audio.SetSoundVolume("FIRE_IMPACT", 70.f);
		return int(player->GetDamage() * CANDLE_PERFECT_EXCELLENT_MULTIPLICATOR * enemy->GetResistances()[attackType] * player->GetCurTie()->GetLevelCoef());
	}

	if (indexFireball == 6 || indexFireball == 8)
	{
		CallResult(ResultAttack::EXCELENT);
		audio.ChangeBuffer("FIRE_IMPACT", "FIRE_EXCELENT");
		audio.SetSoundVolume("FIRE_IMPACT", 50.f);
		return int(player->GetDamage() * CANDLE_PERFECT_EXCELLENT_MULTIPLICATOR * enemy->GetResistances()[attackType] * player->GetCurTie()->GetLevelCoef());
	}
	else if (indexFireball == 4 || indexFireball == 5 || indexFireball == 9 || indexFireball == 10)
	{
		CallResult(ResultAttack::GREAT);
		audio.ChangeBuffer("FIRE_IMPACT", "FIRE_GREAT");
		audio.SetSoundVolume("FIRE_IMPACT");
		return int(player->GetDamage() * CANDLE_GREAT_MULTIPLICATOR * enemy->GetResistances()[attackType] * player->GetCurTie()->GetLevelCoef());
	}
	else if (indexFireball == 2 || indexFireball == 3 || indexFireball == 11 || indexFireball == 12)
	{
		CallResult(ResultAttack::OK);
		audio.ChangeBuffer("FIRE_IMPACT", "FIRE_OK");
		audio.SetSoundVolume("FIRE_IMPACT", 85.f);
		return int(player->GetDamage() * CANDLE_OK_MULTIPLICATOR * enemy->GetResistances()[attackType] * player->GetCurTie()->GetLevelCoef());
	}
	else if (indexFireball == 0 || indexFireball == 1 || indexFireball == 13)
	{
		CallResult(ResultAttack::MISS);
		audio.ChangeBuffer("FIRE_IMPACT", "FIRE_MISS");
		audio.SetSoundVolume("FIRE_IMPACT", 75.f);
		return int(player->GetDamage() * CANDLE_MISS_MULTIPLICATOR * enemy->GetResistances()[attackType] * player->GetCurTie()->GetLevelCoef());
	}
	return -10000000;
}