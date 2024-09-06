#include "FireBallMiniGame.hpp"
#include "../Player.hpp"
#include "../Enemy.hpp"

FireBallMiniGame::FireBallMiniGame(void)
{
	
}

void FireBallMiniGame::InitMiniGame(std::shared_ptr<Player>& _player, std::shared_ptr<Enemy>& _enemy, FightingState* _fightingState)
{
	SetActiveDef();

	shakeTime = 0.f;
	timeAtk = Maths::PreciseRand(TIME_MAX_ATTACK, TIME_MIN_ATTACK);
	timeBeforeAtk = Maths::PreciseRand(TIME_MAX_BEFORE_ATK, TIME_MIN_BEFORE_ATK);
	state = static_cast<StateFunc>(&FireBallMiniGame::UpdateBegin);

	if (player->IsAlive())
	{
		enemy->SetAnimationState(Enemy::AnimationState::MOVE);
	}

	if (enemy->IsStun())
	{
		End();
	}
}

void FireBallMiniGame::UpdateMiniGame(FightingState* _fightingState)
{
	// State update
	if (this->state) (this->*state)(_fightingState);
}

void FireBallMiniGame::PollEventMiniGame(sf::Event& _event, FightingState* _fightingState)
{

}

void FireBallMiniGame::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(textResult, states);
}

void FireBallMiniGame::UpdateBegin(FightingState* _fightingState)
{
	// Calcul position by time between 2 positions
	float factor = Easings::InOutCubic(Maths::Clamp(totalTime / TIME_START, 0.f, 1.f));
	sf::Vector2f newPosition = Maths::InterpolateVector(enemyInitPos, playerInitPos + offsetBetween, factor);
	enemy->setPosition(newPosition);

	// If begin is end
	if (totalTime >= TIME_START) NextToBeforeAttack();
}

void FireBallMiniGame::UpdateBeforeAttack(FightingState* _fightingState)
{
	// Shake
	if (totalTime < timeBeforeAtk - 0.2f)
	{
		shakeTime += glMng.GetDeltaTime();

		if (shakeTime >= SHAKE_TIME)
		{
			shakeTime -= SHAKE_TIME;

			sf::Vector2f newPosition = GetShakingPos(enemy->getPosition(), playerInitPos + offsetBetween);
			enemy->setPosition(newPosition);
		}
	}
	else
	{
		activeWarning = true;
		warning.setPosition(enemy->getPosition() - sf::Vector2f(0.f, enemy->GetGlobalAABB().height + 50));
	}

	// If begin is end
	if (totalTime >= timeBeforeAtk) NextToAttack();
}

void FireBallMiniGame::UpdateAttack(FightingState* _fightingState)
{
	// Calcul position by time between 2 positions
	const float factor = Easings::InCubic(Maths::Clamp(totalTime / timeAtk, 0, 1));
	sf::Vector2f newPosition = Maths::InterpolateVector(playerInitPos + offsetBetween, sf::Vector2f(-159.f, playerInitPos.y), factor);
	enemy->setPosition(newPosition);

	// Dodge minigame
	if (player->GetGlobalAABB().intersects(enemy->GetGlobalAABB()) &&
		player->GetState() != PlayerState::DODGE &&
		enemy->GetAnimationState() == Enemy::AnimationState::ATTACKING)
	{
		player->TakeDamage(enemy->GetDamage(), enemy->GetAttackType());
		enemy->ApplyEffect(player);

		// Death enemy
		enemy->SetAnimationState(Enemy::AnimationState::SPECIAL);
		enemy->GetCurAnim()->Reset();
		enemy->GetCurAnim()->Play();
		enemy->SetHP(0);
		enemy->setScale(1.25f, 1.25f);

		state = static_cast<StateFunc>(&FireBallMiniGame::EndDead);
	}

	// If attack is end
	if (totalTime >= timeAtk) NextToReturnStart();
}

void FireBallMiniGame::UpdateReturnStart(FightingState* _fightingState)
{
	// Calcul position by time between 2 positions
	const float factor = Easings::InCubicInvert(Maths::Clamp(totalTime / TIME_RETURN_START, 0, 1));
	sf::Vector2f newPosition = Maths::InterpolateVector(enemyInitPos, sf::Vector2f(WORKING_WIDTH + 159, playerInitPos.y), factor);
	enemy->setPosition(newPosition);

	// If return start is end
	if (totalTime >= TIME_RETURN_START && textResultTime >= TEXT_RESULT_TIME) End();
}

void FireBallMiniGame::EndDead(FightingState* _fightingState)
{
	// Security to make sure the fireball is dead
	// Whitout this, the next enemies is stop current his minigame
	if (!enemy->IsAlive()) End();
}

void FireBallMiniGame::NextToBeforeAttack(void)
{
	totalTime = 0.f;
	state = static_cast<StateFunc>(&FireBallMiniGame::UpdateBeforeAttack);
	enemy->SetAnimationState(Enemy::AnimationState::IDLE);
}

void FireBallMiniGame::NextToAttack(void)
{
	totalTime = 0.f;
	activeWarning = false;
	state = static_cast<StateFunc>(&FireBallMiniGame::UpdateAttack);
	enemy->SetAnimationState(Enemy::AnimationState::ATTACKING);
}

void FireBallMiniGame::NextToReturnStart()
{
	totalTime = 0.f;
	state = static_cast<StateFunc>(&FireBallMiniGame::UpdateReturnStart);
	enemy->SetAnimationState(Enemy::AnimationState::MOVE);
	CallResult(ResultDodge::DODGE);
}

void FireBallMiniGame::End(void)
{
	enemy->setPosition(enemyInitPos);
	enemy->SetAnimationState(Enemy::AnimationState::IDLE);
	FinishMiniGame();
}
