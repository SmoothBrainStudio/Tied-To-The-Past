#include "../Definitions.hpp"
#include "BigBooMiniGame.hpp"

BigBooMiniGame::BigBooMiniGame(void)
{
	InitSounds();
}

BigBooMiniGame::~BigBooMiniGame(void)
{
	DeleteSounds();
}

void BigBooMiniGame::InitMiniGame(std::shared_ptr<Player>& _player, std::shared_ptr<Enemy>& _enemy, FightingState* _fightingState)
{
	initEnemyScale = enemy->getScale();
	rotateAngle = 0.f;
	playerTakeDamage = false;
	playerGiveDamage = false;

	if (player->IsAlive())
	{
		if (turnBeforeSpecialAtk > 0)
		{
			InitNormalAttack();
		}
		else
		{
			InitSpecialAttack();
		}
	}

	enemy->GetCurAnim()->Reset();
	enemy->GetCurAnim()->Play();

	attackTimer = Maths::PreciseRand(BIG_BOO_ATACK_TIMER_MAX, BIG_BOO_ATACK_TIMER_MIN);
	shakeTime = 0.f;

	if (enemy->IsStun())
	{
		End();
	}
}

void BigBooMiniGame::UpdateMiniGame(FightingState* _fightingState)
{
	const float& dt = GameClock::Get().GetDeltaTime();

	UpdatePlayerDefense();

	switch (bigBooState)
	{
	case BigBooMiniGame::State::NORMAL_MOVE:
		UpdateNormalMove(dt);
		break;
	case BigBooMiniGame::State::NORMAL_ATK:
		UpdateNormalAttack(dt);
		break;
	case BigBooMiniGame::State::SPECIAL_MOVE:
		UpdateSpecialMove(dt);
		break;
	case BigBooMiniGame::State::SPECIAL_ATK:
		UpdateSpecialAttack(dt);
		break;
	case BigBooMiniGame::State::NORMAL_SHAKE:
		UpdateNormalShake(dt);
		break;
	case BigBooMiniGame::State::SPECIAL_SHAKE:
		UpdateSpecialShake(dt);
		break;
	case BigBooMiniGame::State::NORMAL_REPLACE:
		UpdateNormalReplace(dt);
		break;
	case BigBooMiniGame::State::SPECIAL_REPLACE:
		UpdateSpecialReplace(dt);
		break;
	case BigBooMiniGame::State::COUNTER_REPLACE:
		UpdateCounterReplace(dt);
		break;
	default:
		Debug.warning("Problem with BigBoo Update");
		break;
	}
}

void BigBooMiniGame::PollEventMiniGame(sf::Event& _event, FightingState* _fightingState)
{
	EventPlayerDefense(_event);
}

void BigBooMiniGame::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(textResult, states);
}

void BigBooMiniGame::InitSounds(void)
{
	audioMng.AddBuffer("Contents/Assets/Audio/Sounds/Enemies/BooNormalHit.wav", "BOO_NORMAL_HIT");
	audioMng.AddBuffer("Contents/Assets/Audio/Sounds/Enemies/BooSpecialHit.wav", "BOO_SPECIAL_HIT");
	audioMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Enemies/BooNormalMove.wav", "BOO_NORMAL_MOVE", 30.f);
	audioMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Enemies/BooNormalReplace.wav", "BOO_NORMAL_REPLACE", 30.f);
	audioMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Enemies/BooBreath.wav", "BOO_BREATH", 50.f);
	audioMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Enemies/BooBounce.wav", "BOO_BOUNCE", 90.f);
	audioMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Enemies/BooSpecialMove.wav", "BOO_SPECIAL_MOVE");
}

void BigBooMiniGame::DeleteSounds(void)
{
	audioMng.DeleteBuffer("BOO_NORMAL_HIT");
	audioMng.DeleteBuffer("BOO_SPECIAL_HIT");
	audioMng.DeleteSoundAndBuffer("BOO_NORMAL_MOVE");
	audioMng.DeleteSoundAndBuffer("BOO_NORMAL_REPLACE");
	audioMng.DeleteSoundAndBuffer("BOO_BREATH");
	audioMng.DeleteSoundAndBuffer("BOO_BOUNCE");
	audioMng.DeleteSoundAndBuffer("BOO_SPECIAL_MOVE");
}

void BigBooMiniGame::InitNormalAttack(void)
{
	turnBeforeSpecialAtk--;
	bigBooState = State::NORMAL_MOVE;
	enemy->SetAnimationState(Enemy::AnimationState::MOVE);
	target = sf::Vector2f(playerInitPos.x, playerInitPos.y - player->GetSprite().getGlobalBounds().height / 2.f);
	vectorMove = Maths::NormalizeVector(target - enemy->getPosition());
}

void BigBooMiniGame::InitSpecialAttack(void)
{
	turnBeforeSpecialAtk = TURN_BEFORE_SPECIAL_ATK;
	bigBooState = State::SPECIAL_ATK;
	enemy->SetAnimationState(Enemy::AnimationState::SPECIAL);
	target = sf::Vector2f(playerInitPos.x, playerInitPos.y - player->GetSprite().getGlobalBounds().height / 2.f);
	vectorMove = Maths::NormalizeVector(target - enemy->getPosition());
	audioMng.PlaySound("BOO_BREATH");
}

void BigBooMiniGame::UpdateNormalMove(const float& _dt)
{
	const float factor = Easings::InOutCubic(Maths::Clamp(totalTime / TIME_MOVE, 0, 1));
	sf::Vector2f newPosition = Maths::InterpolateVector(enemyInitPos, playerInitPos + offsetBetween, factor);

	// Update shadow position
	enemy->SetShadowOffset(0.f, enemyInitPos.y - newPosition.y);

	// Apply position
	enemy->setPosition(newPosition);

	// Play sound
	audioMng.PlaySound("BOO_NORMAL_MOVE");

	// If begin is end
	if (totalTime >= TIME_MOVE)
	{
		lastState = bigBooState;
		bigBooState = State::NORMAL_SHAKE;
		enemy->GetCurAnim()->Pause();
		totalTime = 0.f;
	}
}

void BigBooMiniGame::UpdateNormalAttack(const float& _dt)
{
	// Active exclamation mark
	if (enemy->GetAnimationState() == Enemy::AnimationState::ATTACKING)
	{
		if (enemy->GetCurAnim()->GetCurrentFrame() > 3 && enemy->GetCurAnim()->GetCurrentFrame() < 6)
		{
			activeWarning = true;
		}
		else
		{
			activeWarning = false;
		}
	}

	// Hiting player test 
	if (enemy->GetCurAnim()->GetCurrentFrame() > 6 &&
		enemy->GetCurAnim()->GetCurrentFrame() < 10 &&
		player->GetState() != PlayerState::COUNTER &&
		player->GetState() != PlayerState::DODGE)
	{
		if (!playerTakeDamage)
		{
			audioMng.ChangeBuffer("ENEMY_HIT", "BOO_NORMAL_HIT");
			audioMng.PlaySound("ENEMY_HIT", 90.f);
			enemy->ApplyEffect(player);
			player->TakeDamage(enemy->GetDamage(), enemy->GetAttackType());
			playerTakeDamage = true;
			activeWarning = false;
		}
	}


	else if (player->GetState() == PlayerState::COUNTER &&
		enemy->GetCurAnim()->GetCurrentFrame() > 3 &&
		enemy->GetCurAnim()->GetCurrentFrame() < 6)
	{
		if (!playerGiveDamage)
		{
			if (player->GetCostumeState() == CostumeState::MUSIC)
			{
				if (rand() % 100 < 70)
				{
					player->ApplyParticularEffect(enemy, Entity::Effects::STUN, 2);
				}
			}
			else
			{
				if (player->GetCostumeState() == CostumeState::BOXER && player->GetCurTie()->GetLevel() > 2)
				{
					player->ApplyEffect(enemy);
				}
				else if (player->GetCostumeState() == CostumeState::CANDLE)
				{
					player->ApplyEffect(enemy);
				}
			}
			enemy->TakeDamage(*player);
			player->PlaySoundCounter();
			playerGiveDamage = true;
			activeWarning = false;

			enemy->SetAnimationState(Enemy::AnimationState::DAMAGE);
			enemy->GetCurAnim()->Reset();
			enemy->GetCurAnim()->Play();
		}
	}

	// if animation is end
	if (enemy->GetAnimationState() != Enemy::AnimationState::ATTACKING && enemy->GetAnimationState() != Enemy::AnimationState::DAMAGE
		&& totalTime >= attackTimer)
	{
		NextToEnd();
	}
}

void BigBooMiniGame::UpdateSpecialMove(const float& _dt)
{
	// Active exclamation mark
	if (enemy->getPosition().x - player->getPosition().x > 400.f &&
		enemy->getPosition().x - player->getPosition().x < 1400.f)
	{
		warning.setPosition(enemy->getPosition() - sf::Vector2f(350.f, 1150.f));
		activeWarning = true;
	}
	else
	{
		activeWarning = false;
	}

	// BigBoo rotation
	rotateAngle -= ROTATE_SPEED * _dt;
	enemy->GetCurAnim()->setRotation(rotateAngle);

	// Calcul position by time between 2 positions
	float factor = Easings::InCubic(Maths::Clamp(totalTime / attackTimer, 0, 1));
	sf::Vector2f newPosition = Maths::InterpolateVector(initSpecialPos + offsetSpecial, sf::Vector2f(-OUT_SCREEN_OFFSET, playerInitPos.y + 800.f), factor);
	enemy->setPosition(newPosition);

	// Update shadow position
	enemy->SetShadowOffset(-offsetSpecial.x, enemyInitPos.y - newPosition.y);

	// Dodge minigame
	if (std::abs(player->getPosition().x - enemy->getPosition().x) < 400.f)
	{
		if (player->GetState() != PlayerState::DODGE &&
			player->GetState() != PlayerState::COUNTER &&
			!playerTakeDamage)
		{
			audioMng.ChangeBuffer("ENEMY_HIT", "BOO_SPECIAL_HIT");
			audioMng.PlaySound("ENEMY_HIT", 95.f);
			player->TakeDamage(enemy->GetDamage(), enemy->GetAttackType());
			enemy->ApplyEffect(player);
			playerTakeDamage = true;
			activeWarning = false;
		}
		else if (player->GetState() == PlayerState::COUNTER)
		{
			if (!playerGiveDamage)
			{
				if (player->GetCostumeState() == CostumeState::MUSIC)
				{
					if (rand() % 100 < 70)
					{
						player->ApplyParticularEffect(enemy, Entity::Effects::STUN, 2);
					}
				}
				else
				{
					if (player->GetCostumeState() == CostumeState::BOXER && player->GetCurTie()->GetLevel() > 2)
					{
						player->ApplyEffect(enemy);
					}
					else if (player->GetCostumeState() == CostumeState::CANDLE)
					{
						player->ApplyEffect(enemy);
					}
				}
				enemy->TakeDamage(*player);
				player->PlaySoundCounter();
				playerGiveDamage = true;
				activeWarning = false;
			}
		}
	}

	if (playerGiveDamage)
	{
		//ResetInitOrigin();
		bigBooState = State::COUNTER_REPLACE;
		rotateAngle = 0.f;
		initCounterPos = enemy->getPosition();
		totalTime = 0.f;
	}

	// If attack is end
	if (totalTime >= attackTimer)
	{
		ResetInitOrigin();
		NextToEnd();
	}
}

void BigBooMiniGame::UpdateSpecialAttack(const float& _dt)
{
	if (totalTime <= 1)
	{
		float factor = Easings::InOutCubic(Maths::Clamp(totalTime, 0, 1));
		float factorRadius = 0.5f + 0.35f * factor;
		enemy->SetShadowRadiusFactor(factorRadius);
	}
	else if (enemy->GetCurAnim()->GetCurrentFrame() == enemy->GetCurAnim()->GetFrameCount() - 1 &&
		enemy->GetCurAnim()->GetStatus() != AnimatedSprite::Status::PAUSED)
	{
		totalTime = 0;

		// Paused anim on last frame
		enemy->GetCurAnim()->Pause();
		enemy->GetCurAnim()->SetCurrentFrame(enemy->GetCurAnim()->GetFrameCount() - 1);

		lastState = bigBooState;
		bigBooState = State::SPECIAL_SHAKE;
	}
}

void BigBooMiniGame::UpdateNormalShake(const float& _dt)
{
	Shake(_dt, playerInitPos + offsetBetween);

	// If beforeAtack is end
	if (totalTime >= attackTimer)
	{
		bigBooState = State::NORMAL_ATK;
		enemy->SetAnimationState(Enemy::AnimationState::ATTACKING);
		enemy->GetCurAnim()->Reset();
		enemy->GetCurAnim()->Play();
		warning.setPosition(enemy->getPosition() - sf::Vector2f(0.f, enemy->GetGlobalAABB().height + 175.f));
		totalTime = 0.f;
	}
}

void BigBooMiniGame::UpdateSpecialShake(const float& _dt)
{
	Shake(_dt, enemyInitPos);

	// If beforeAtack is end
	if (totalTime >= attackTimer)
	{
		bigBooState = State::SPECIAL_MOVE;

		// Store Origin and position
		initEnemyOrigin = enemy->GetCurAnim()->getOrigin();
		initSpecialPos = enemy->getPosition();

		// Set origin to center for rotation
		enemy->GetCurAnim()->setOrigin(offsetSpecial);

		// Set new position according to new origin
		enemy->setPosition(enemy->getPosition() + offsetSpecial);

		// PLay sound
		audioMng.PlaySound("BOO_SPECIAL_MOVE");

		totalTime = 0.f;
	}
}

void BigBooMiniGame::UpdateNormalReplace(const float& _dt)
{
	sf::Vector2f newPosition;

	// Move to initial position
	float factor = Easings::InOutCubic(Maths::Clamp(totalTime / TIME_END, 0, 1));
	newPosition = Maths::InterpolateVector(playerInitPos + offsetBetween, enemyInitPos, factor);

	// Update shadow position
	enemy->SetShadowOffset(0.f, enemyInitPos.y - newPosition.y);

	// Apply position
	enemy->setPosition(newPosition);

	// Play sound
	audioMng.PlaySound("BOO_NORMAL_REPLACE");

	// If begin is end
	if (totalTime >= TIME_END && (player->GetState() == PlayerState::IDLE || player->GetState() == PlayerState::DEATH)
		&& textResultTime >= TEXT_RESULT_TIME)
	{
		End();
	}
}

void BigBooMiniGame::UpdateSpecialReplace(const float& _dt)
{
	// Calcul position by time between 2 positions
	const float factor = Easings::InCubicInvert(Maths::Clamp(totalTime / TIME_RETURN_START, 0, 1));
	sf::Vector2f newPosition = Maths::InterpolateVector(enemyInitPos, sf::Vector2f(WORKING_WIDTH + OUT_SCREEN_OFFSET, playerInitPos.y), factor);
	enemy->setPosition(newPosition);

	// If return start is end
	if (totalTime >= TIME_RETURN_START && textResultTime >= TEXT_RESULT_TIME)
	{
		End();
	}
}

void BigBooMiniGame::ResetInitOrigin(void)
{
	rotateAngle = 0.f;
	enemy->GetCurAnim()->setRotation(rotateAngle);
	enemy->GetCurAnim()->setOrigin(0.f, 0.f);
}

void BigBooMiniGame::UpdateCounterReplace(const float& _dt)
{
	// Big boo rotation
	rotateAngle += ROTATE_SPEED * _dt;
	enemy->GetCurAnim()->setRotation(rotateAngle);

	// Calcul position by time between 2 positions
	const float easingFactor = Easings::InBounce(Maths::Clamp(1 - totalTime / TIME_RETURN_START, 0, 1));
	const float timerFactor = totalTime / TIME_RETURN_START;
	sf::Vector2f newPosition = Easings::LerpY(initCounterPos, sf::Vector2f(WORKING_WIDTH + OUT_SCREEN_OFFSET * 2.f, enemyInitPos.y + offsetSpecial.y), timerFactor, easingFactor);
	enemy->setPosition(newPosition);

	// Play bounce sound
	if (newPosition.y >= initCounterPos.y - 1.f)
	{
		audioMng.PlaySound("BOO_BOUNCE");
	}

	// Update shadow position
	enemy->SetShadowOffset(-offsetSpecial.x, enemyInitPos.y - newPosition.y);

	if (totalTime >= TIME_COUNTER_MOVE && textResultTime >= TEXT_RESULT_TIME)
	{
		ResetInitOrigin();
		enemy->SetShadowOffset(0.f, 0.f);
		enemy->SetShadowRadiusFactor(0.5f);

		enemy->SetAnimationState(Enemy::AnimationState::IDLE);
		enemy->GetCurAnim()->Reset();
		enemy->GetCurAnim()->Play();

		totalTime = 0.f;
		bigBooState = State::SPECIAL_REPLACE;
	}
}

void BigBooMiniGame::Shake(const float& _dt, const sf::Vector2f _refPos)
{
	if (totalTime < attackTimer - 0.4f)
	{
		shakeTime += _dt;

		if (shakeTime >= SHAKE_TIME)
		{
			shakeTime -= SHAKE_TIME;

			sf::Vector2f newPosition = GetShakingPos(enemy->getPosition(), _refPos);
			enemy->setPosition(newPosition);
		}
	}
}

void BigBooMiniGame::NextToEnd(void)
{
	// Reset members
	totalTime = 0.f;

	// Change animation
	enemy->SetAnimationState(Enemy::AnimationState::MOVE);

	// Dodge text
	if (!playerTakeDamage)
	{
		CallResult(ResultDodge::DODGE);
	}
	if (playerGiveDamage)
	{
		CallResult(ResultDodge::BLOCKED);
	}

	// Change state
	if (bigBooState == State::NORMAL_ATK)
	{
		enemy->setPosition(playerInitPos + offsetBetween);
		enemy->setScale(sf::Vector2f(initEnemyScale.x * -1, initEnemyScale.y));
		enemy->SetAnimationState(Enemy::AnimationState::MOVE);
		enemy->GetCurAnim()->Reset();
		enemy->GetCurAnim()->Play();
		bigBooState = State::NORMAL_REPLACE;
	}
	else if (bigBooState == State::SPECIAL_MOVE)
	{
		// Update shadow position
		enemy->SetShadowOffset(0.f, 0.f);
		enemy->SetShadowRadiusFactor(0.5f);
		bigBooState = State::SPECIAL_REPLACE;
	}
}

void BigBooMiniGame::End()
{
	// Reset members
	FinishMiniGame();
	// Change animation
	enemy->setScale(initEnemyScale);
}

void BigBooMiniGame::Start(FightingState* _fightingState)
{
	over = true;
}