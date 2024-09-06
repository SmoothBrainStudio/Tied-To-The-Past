#include "../Definitions.hpp"
#include "ZombieMiniGame.hpp"
#include "../Enemy.hpp"

ZombieMiniGame::ZombieMiniGame(void) :
	curUpdate(nullptr)
{
}

ZombieMiniGame::~ZombieMiniGame(void)
{

}

void ZombieMiniGame::InitMiniGame(std::shared_ptr<Player>& _player, std::shared_ptr<Enemy>& _enemy, FightingState* _fightingState)
{
	SetActiveDef();

	if (player->IsAlive())
	{
		enemy->SetAnimationState(Enemy::AnimationState::MOVE);
	}

	curUpdate = &ZombieMiniGame::UpdateBegin;

	shakeTime = 0.f;
	playerTakeDamage = false;
	playerGiveDamage = false;
	timeBeforeAtk = Maths::PreciseRand(TIME_MAX_BEFORE_ATK, TIME_MIN_BEFORE_ATK);
	InitEnemyScale = enemy->getScale();

	if (enemy->IsStun())
	{
		End();
	}
}

void ZombieMiniGame::PollEventMiniGame(sf::Event& _event, FightingState* _fightingState)
{

}

void ZombieMiniGame::UpdateMiniGame(FightingState* _fightingState)
{
	// If curUpdate is not init
	if (curUpdate == nullptr)
	{
		Debug.warning("Minigame zombie can't be update !");
		return;
	}

	// Update state
	(this->*curUpdate)(_fightingState);
}

void ZombieMiniGame::UpdateBegin(FightingState* _fightingState)
{
	float factor = Easings::InOutCubic(Maths::Clamp(totalTime / TIME_BEGIN, 0.f, 1.f));
	sf::Vector2f newPosition = Maths::InterpolateVector(enemyInitPos, playerInitPos + offsetBetween, factor);

	// Apply position
	enemy->setPosition(newPosition);

	// If begin is end
	if (totalTime >= TIME_BEGIN) NextToBeforeAttack();
}

void ZombieMiniGame::UpdateBeforeAttack(FightingState* _fightingState)
{
	// Animation
	if (enemy->GetCurAnim()->GetCurrentFrame() == PAUSE_FRAME) enemy->GetCurAnim()->Pause();

	if (enemy->GetCurAnim()->GetStatus() == AnimatedSprite::Status::PAUSED)
	{
		// Shake
		shakeTime += glMng.GetDeltaTime();

		if (shakeTime >= SHAKE_TIME && totalTime < timeBeforeAtk - 0.4f)
		{
			shakeTime -= SHAKE_TIME;

			sf::Vector2f newPosition = GetShakingPos(enemy->getPosition(), playerInitPos + offsetBetween);
			enemy->setPosition(newPosition);
		}
	}

	if (totalTime >= timeBeforeAtk - 0.4f)
	{
		activeWarning = true;
		warning.setPosition(enemy->getPosition() - sf::Vector2f(0.f, enemy->GetGlobalAABB().height + 25));
	}

	// If beforeAtack is end
	if (totalTime >= timeBeforeAtk) NextToAttack();
}

void ZombieMiniGame::UpdateAttack(FightingState* _fightingState)
{
	if (!playerGiveDamage)
	{
		float factor = Easings::Square(Maths::Clamp(totalTime / TIME_ATTACK, 0.f, 1.f));
		sf::Vector2f newPosition = Maths::InterpolateVector(playerInitPos + offsetBetween, playerInitPos + offsetBetween / 4.f, factor);
		enemy->setPosition(newPosition);
	}
	else
	{
		enemyCounteredPos = enemy->getPosition();
	}

	if (player->GetGlobalAABB().intersects(enemy->GetGlobalAABB()) &&
		player->GetState() != PlayerState::COUNTER &&
		player->GetState() != PlayerState::DODGE &&
		enemy->GetAnimationState() == Enemy::AnimationState::ATTACKING)
	{
		if (!playerTakeDamage)
		{
			audioMng.ChangeBuffer("ENEMY_HIT", "ZOMBIE_HIT");
			audioMng.PlaySound("ENEMY_HIT", 75.f);
			enemy->ApplyEffect(player);
			player->TakeDamage(enemy->GetDamage(), enemy->GetAttackType());
			playerTakeDamage = true;
		}
	}
	else if (player->GetState() == PlayerState::COUNTER &&
		enemy->GetAnimationState() == Enemy::AnimationState::ATTACKING)
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
		}
	}

	// if animation is end
	if (enemy->GetAnimationState() != Enemy::AnimationState::ATTACKING && enemy->GetAnimationState() != Enemy::AnimationState::DAMAGE && totalTime >= TIME_ATTACK) NextToEnd();
}

void ZombieMiniGame::UpdateEnd(FightingState* _fightingState)
{
	sf::Vector2f newPosition;

	if (!playerGiveDamage)
	{
		float factor = Easings::InOutCubic(Maths::Clamp(totalTime / TIME_END, 0, 1));
		newPosition = Maths::InterpolateVector(playerInitPos + offsetBetween, enemyInitPos, factor);
	}
	else
	{
		float factor = Easings::InOutCubic(Maths::Clamp(totalTime / TIME_END, 0, 1));
		newPosition = Maths::InterpolateVector(enemyCounteredPos, enemyInitPos, factor);
	}

	// Apply position
	enemy->setPosition(newPosition);

	// If begin is end
	if (totalTime >= TIME_END && (player->GetState() == PlayerState::IDLE || player->GetState() == PlayerState::DEATH)) End();
}

void ZombieMiniGame::NextToBeforeAttack(void)
{
	// Change update
	curUpdate = &ZombieMiniGame::UpdateBeforeAttack;
	// Reset members
	enemy->setPosition(playerInitPos + offsetBetween);
	totalTime = 0.f;
	// Change animation
	enemy->SetAnimationState(Enemy::AnimationState::ATTACKING);
	enemy->GetCurAnim()->Reset();
	enemy->GetCurAnim()->Play();
}

void ZombieMiniGame::NextToAttack(void)
{
	// Change update
	curUpdate = &ZombieMiniGame::UpdateAttack;
	// Reset members
	enemy->setPosition(playerInitPos + offsetBetween);
	activeWarning = false;
	totalTime = 0.f;
	// Start animation
	enemy->GetCurAnim()->Play();
}

void ZombieMiniGame::NextToEnd(void)
{
	// Change update
	curUpdate = &ZombieMiniGame::UpdateEnd;
	// Reset members
	enemy->setPosition(playerInitPos + offsetBetween);
	totalTime = 0.f;
	// Change animation
	enemy->SetAnimationState(Enemy::AnimationState::MOVE);
	enemy->setScale(sf::Vector2f(InitEnemyScale.x * -1, InitEnemyScale.y));
	// Dodge text
	if (!playerTakeDamage) CallResult(ResultDodge::DODGE);
	if (playerGiveDamage) CallResult(ResultDodge::BLOCKED);
}

void ZombieMiniGame::End(void)
{
	// Reset members
	FinishMiniGame();
	// Change animation
	enemy->SetAnimationState(Enemy::AnimationState::IDLE);
	enemy->setScale(InitEnemyScale);
}

void ZombieMiniGame::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
{
	_states.transform *= getTransform();

	_target.draw(textResult, _states);
}
