#include "../Definitions.hpp"
#include "BossCandleMiniGame.hpp"

BossCandleMiniGame::BossCandleMiniGame()
{
	fireballAnim = AnimatedSprite("Contents/Data/Animations/Other/fireballFighting.json");
	DrawManager::Get().AddObject("FIGHT_HUD", fireballAnim, false);

	const sf::FloatRect fireballRect = fireballAnim.GetSprite().getGlobalBounds();
	fireballAnim.GetSprite().setOrigin(fireballRect.width / 2.f, fireballRect.height);

	InitSounds();
}

BossCandleMiniGame::~BossCandleMiniGame()
{
	DrawManager::Get().DeleteObject("FIGHT_HUD", &fireballAnim);

	DeleteSounds();
}

void BossCandleMiniGame::InitMiniGame(std::shared_ptr<Player>& _player, std::shared_ptr<Enemy>& _enemy, FightingState* _fightingState)
{
	SetActiveDef();

	if (turnBeforAtkSpc-- > 0 || _fightingState->GetEnemiesSize() > 1)
	{
		enemy->SetAnimationState(Enemy::AnimationState::ATTACKING);
		enemyState = State::ATK_BASIQUE;
		timerThrow = Maths::PreciseRand(2.f, 1.f);

		fireballAnim.Reset();
		fireballAnim.setPosition(enemy->getPosition().x - enemy->GetGlobalAABB().width / 5.5f, enemy->getPosition().y - enemy->GetGlobalAABB().height + 25.f);
		fireballAnim.Play();
		DrawManager::Get().SetObjectVisible("FIGHT_HUD", &fireballAnim);
	}
	else
	{
		Audio::Get().PlaySound("CANDLE_SUMMON");
		turnBeforAtkSpc = TURN_BEFORE_SPECIAL_ATK;
		enemy->SetAnimationState(Enemy::AnimationState::SPECIAL);
		enemyState = State::ATK_SPECIAL;
		std::shared_ptr<Enemy> newEnemy1 = std::make_shared<Enemy>(*Enemy::prefabs["FIREBALL"]);
		std::shared_ptr<Enemy> newEnemy2 = std::make_shared<Enemy>(*Enemy::prefabs["FIREBALL"]);
		_fightingState->AddEnemy(newEnemy1);
		_fightingState->AddEnemy(newEnemy2);
	}

	enemy->GetCurAnim()->Reset();
	enemy->GetCurAnim()->Play();

	if (enemy->IsStun())
	{
		Finish();
	}
}

void BossCandleMiniGame::PollEventMiniGame(sf::Event& _event, FightingState* _fightingState)
{
}

void BossCandleMiniGame::UpdateMiniGame(FightingState* _fightingState)
{
	DrawManager::Get().SetObjectVisible("FIGHT_HUD", &fireballAnim, enemy->GetHP() > 0 && (enemyState == BossCandleMiniGame::ATK_BASIQUE_THROW || enemyState == BossCandleMiniGame::ATK_BASIQUE));
	fireballAnim.Update();

	switch (enemyState)
	{
	case BossCandleMiniGame::ATK_BASIQUE:
		UpdateAtkBasic(_fightingState);
		break;
	case BossCandleMiniGame::ATK_BASIQUE_THROW:
		UpdateAtkBasicThrow(_fightingState);
		break;
	case BossCandleMiniGame::ATK_SPECIAL:
		UpdateAtkSpecial(_fightingState);
		break;
	default:
		break;
	}
}

void BossCandleMiniGame::UpdateAtkBasic(FightingState* _fightingState)
{
	auto& audioMng = Audio::Get();

	timerThrow -= GameClock::Get().GetDeltaTime();
	enemy->SetAnimationState(Enemy::AnimationState::ATTACKING);
	enemy->GetCurAnim()->Play();

	audioMng.PlaySound("FIRE_NOISE");
	if (timerThrow < 0.3f && timerThrow > 0.f)
	{
		activeWarning = true;
		warning.setPosition(enemy->getPosition() - sf::Vector2f(40.f, enemy->GetCurAnim()->GetSprite().getGlobalBounds().height + 70));
	}
	else if (timerThrow < 0.f)
	{
		audioMng.StopSound("FIRE_NOISE");
		audioMng.PlaySound("FIREBALL_LAUNCH");
		target = sf::Vector2f(playerInitPos.x, playerInitPos.y - player->GetSprite().getGlobalBounds().height / 2.f);
		vectorFireBall = Maths::NormalizeVector(target - fireballAnim.getPosition());
		fireballAnim.Pause();
		enemyState = ATK_BASIQUE_THROW;
		activeWarning = false;
	}
}

void BossCandleMiniGame::UpdateAtkBasicThrow(FightingState* _fightingState)
{
	enemy->SetAnimationState(Enemy::AnimationState::ATTACKING);
	enemy->GetCurAnim()->Play();
	fireballAnim.move(vectorFireBall * FIRE_BALL_SPEED * Maths::Clamp(fireballAnim.GetCurrentFrame() - 3.f, 1.f, 3.f) * GameClock::Get().GetDeltaTime());

	if (Maths::GetSquaredLength(fireballAnim.getPosition(), target) < 50.f * 50.f)
	{
		if (!(player->GetState() == PlayerState::DODGE))
		{
			PlayFireballFX();
			enemy->ApplyEffect(player);
			player->TakeDamage(enemy->GetDamage(), enemy->GetAttackType());
			Finish();
		}
	}

	if (fireballAnim.getPosition().x < -fireballAnim.GetSprite().getGlobalBounds().width && !(player->GetState() == PlayerState::DODGE) && !(player->GetState() == PlayerState::COUNTER))
	{
		CallResult(ResultDodge::DODGE);
		Finish();
	}
}

void BossCandleMiniGame::UpdateAtkSpecial(FightingState* _fightingState)
{
	auto enemyAnim = enemy->GetCurAnim();
	enemyAnim->Play();
	if (enemyAnim->GetCurrentFrame() == enemyAnim->GetFrameCount() - 1)
	{
		enemy->SetAnimationState(Enemy::IDLE);
	}

	if (enemy->GetAnimationState() == Enemy::IDLE)
	{
		Finish();
	}
}

void BossCandleMiniGame::Finish(void)
{
	DrawManager::Get().SetObjectVisible("FIGHT_HUD", &fireballAnim, false);

	FinishMiniGame();
}

void BossCandleMiniGame::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(textResult, states);
}

void BossCandleMiniGame::InitSounds(void)
{
	auto& audioMng = Audio::Get();

	audioMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Enemies/CandleBossSummon.wav", "CANDLE_SUMMON");
}

void BossCandleMiniGame::DeleteSounds(void)
{
	auto& audioMng = Audio::Get();

	audioMng.DeleteSoundAndBuffer("CANDLE_SUMMON");
}

void BossCandleMiniGame::PlayFireballFX(void)
{
	auto& audioMng = Audio::Get();
	int indexFireball = fireballAnim.GetCurrentFrame();

	if (indexFireball == 7)
	{
		audioMng.ChangeBuffer("FIRE_IMPACT", "FIRE_PERFECT");
		audioMng.SetSoundVolume("FIRE_IMPACT", 70.f);
	}
	else if (indexFireball == 6 || indexFireball == 8)
	{
		audioMng.ChangeBuffer("FIRE_IMPACT", "FIRE_EXCELENT");
		audioMng.SetSoundVolume("FIRE_IMPACT", 50.f);
	}
	else if (indexFireball == 4 || indexFireball == 5 || indexFireball == 9 || indexFireball == 10)
	{
		audioMng.ChangeBuffer("FIRE_IMPACT", "FIRE_GREAT");
		audioMng.SetSoundVolume("FIRE_IMPACT");
	}
	else if (indexFireball == 2 || indexFireball == 3 || indexFireball == 11 || indexFireball == 12)
	{
		audioMng.ChangeBuffer("FIRE_IMPACT", "FIRE_OK");
		audioMng.SetSoundVolume("FIRE_IMPACT", 85.f);
	}
	else if (indexFireball == 0 || indexFireball == 1 || indexFireball == 13)
	{
		audioMng.ChangeBuffer("FIRE_IMPACT", "FIRE_MISS");
		audioMng.SetSoundVolume("FIRE_IMPACT", 75.f);
	}

	audioMng.PlaySound("FIRE_IMPACT");
}