#include "Definitions.hpp"
#include "FrankenchefMiniGame.hpp"

FrankenChefFight::FrankenChefFight(void)
{
	auto& textureMng = TexturesManager::Get();

	textureMng.Add("Contents/Assets/Sprites/Entities/Enemies/Frankenchief/Object/Knife.png", "CHEF_KNIFE");
	textureMng.Add("Contents/Assets/Sprites/Entities/Enemies/Frankenchief/Object/Pan.png", "CHEF_PAN");
	textureMng.Add("Contents/Assets/Sprites/Entities/Enemies/Frankenchief/Object/Phone.png", "CHEF_PHONE?");
	textureMng.Add("Contents/Assets/Sprites/Entities/Enemies/Frankenchief/Object/Roll.png", "CHEF_ROLL");
	textureMng.Add("Contents/Assets/Sprites/Entities/Enemies/Frankenchief/Object/SaucePan.png", "CHEF_SAUCE_PAN");

	chefObject[KNIFE].setTexture(textureMng["CHEF_KNIFE"]);
	chefObject[PAN].setTexture(textureMng["CHEF_PAN"]);
	chefObject[ROLL].setTexture(textureMng["CHEF_ROLL"]);
	chefObject[SAUCE_PAN].setTexture(textureMng["CHEF_SAUCE_PAN"]);
	chefObject[SAUCE_PAN].scale(sf::Vector2f(1.5f, 1.5f));

	for (auto& object : chefObject)
	{
		object.setOrigin(object.getGlobalBounds().width / 2.f, object.getGlobalBounds().height / 2.f);
		DrawManager::Get().AddObject("FIGHT_HUD", object, false);
	}

	InitSounds();
}

FrankenChefFight::~FrankenChefFight(void)
{
	auto& textureMng = TexturesManager::Get();
	textureMng.Delete("CHEF_KNIFE");
	textureMng.Delete("CHEF_PAN");
	textureMng.Delete("CHEF_PHONE?");
	textureMng.Delete("CHEF_ROLL");
	textureMng.Delete("CHEF_SAUCE_PAN");

	for (auto& object : chefObject)
	{
		DrawManager::Get().DeleteObject("FIGHT_HUD", &object);
	}

	DeleteSounds();
}

void FrankenChefFight::InitMiniGame(std::shared_ptr<Player>& _player, std::shared_ptr<Enemy>& _enemy, FightingState* _fightingState)
{
	SetActiveDef();

	if (player->IsAlive())
	{
		enemy->SetAnimationState(Enemy::AnimationState::ATTACKING);
	}

	enemy->GetCurAnim()->Reset();
	enemy->GetCurAnim()->Play();
	throwTimer = Maths::PreciseRand(CHEF_THROW_TIMER_MAX, CHEF_THROW_TIMER_MIN);
	enemyState = State::THROW;

	if (enemy->IsStun())
	{
		Finish();
	}
}

void FrankenChefFight::PollEventMiniGame(sf::Event& _event, FightingState* _fightingState)
{

}

void FrankenChefFight::InitFlyingKnife(void)
{
	Audio::Get().PlaySound("FRANK_LAUNCH");
	DrawManager::Get().SetObjectVisible("FIGHT_HUD", &chefObject[KNIFE]);
	chefObject[KNIFE].setPosition(enemyInitPos.x - enemy->GetGlobalAABB().width / 2.f, enemyInitPos.y - enemy->GetGlobalAABB().height);
	target = sf::Vector2f(playerInitPos.x, playerInitPos.y - player->GetGlobalAABB().height / 2.f);
	vectorKnife = Maths::NormalizeVector(target - chefObject[KNIFE].getPosition());
	enemyState = State::FLYING_KNIFE;
	enemy->SetAnimationState(Enemy::AnimationState::IDLE);

	activeWarning = false;
}

void FrankenChefFight::InitSpecialAtk(void)
{
	Audio::Get().PlaySound("FRANK_LAUNCH");
	turnBeforeSpecialAtk = TURN_BEFORE_SPECIAL_ATK;
	DrawManager::Get().SetObjectVisible("FIGHT_HUD", &chefObject[KNIFE]);
	chefObject[KNIFE].setPosition(enemyInitPos.x - enemy->GetGlobalAABB().width / 2.f, enemyInitPos.y - enemy->GetGlobalAABB().height);
	target = sf::Vector2f(playerInitPos.x, -100.f);
	vectorKnife = Maths::NormalizeVector(target - chefObject[KNIFE].getPosition());
	enemyState = State::SPECIAL_ATK;

	activeWarning = false;
}

void FrankenChefFight::InitSpecialFlying(void)
{
	auto& audioMng = Audio::Get();

	audioMng.StopSound("FRANK_FLY");
	audioMng.PlaySound("FRANK_FALL"); 
	fallingObject = Objects(rand() % (OBJECTS_SIZE - 1) + 1);
	chefObject[fallingObject].setPosition(player->getPosition().x, -500.f);
	DrawManager::Get().SetObjectVisible("FIGHT_HUD", &chefObject[fallingObject]);
	enemyState = State::SPECIAL_FALLING;

	target = sf::Vector2f(0.f, 1.f);
	speed = 800.f;
}

void FrankenChefFight::UpdateMiniGame(FightingState* _fightingState)
{
	switch (enemyState)
	{
	case FrankenChefFight::State::THROW:
		UpdateThrow(_fightingState);
		break;
	case FrankenChefFight::State::FLYING_KNIFE:
		UpdateFlyingKnife(_fightingState);
		break;
	case FrankenChefFight::State::SPECIAL_ATK:
		UpdateSpeAtk(_fightingState);
		break;
	case FrankenChefFight::State::SPECIAL_FALLING:
		UpdateSpeFall(_fightingState);
		break;
	default:
		Debug.warning("ohoh le chef est dans un state chelou ??");
		break;
	}
}

void FrankenChefFight::UpdateThrow(FightingState* _fightingState)
{
	if (enemy->GetCurAnim()->GetCurrentFrame() == enemy->GetCurAnim()->GetFrameCount() - 2)
	{
		enemy->GetCurAnim()->Pause();
		enemy->setPosition(enemyInitPos.x + Maths::PreciseRand(5.f, -5.f), enemyInitPos.y + Maths::PreciseRand(5.f, -5.f));
		throwTimer -= GameClock::Get().GetDeltaTime();
		if (throwTimer < 0.3f && throwTimer > 0.f)
		{
			activeWarning = true;
			warning.setPosition(enemy->getPosition() - sf::Vector2f(0.f, enemy->GetGlobalAABB().height + 100));
		}
		else if (throwTimer < 0.f)
		{

			enemy->GetCurAnim()->SetCurrentFrame(enemy->GetCurAnim()->GetFrameCount() - 1);

			if (turnBeforeSpecialAtk-- > 0)
			{
				InitFlyingKnife();
			}
			else
			{
				InitSpecialAtk();
			}
		}
	}
}

void FrankenChefFight::UpdateFlyingKnife(FightingState* _fightingState)
{
	chefObject[KNIFE].move(vectorKnife * 1200.f * GameClock::Get().GetDeltaTime());
	chefObject[KNIFE].rotate(-1200.f * GameClock::Get().GetDeltaTime());

	Audio::Get().PlaySound("FRANK_FLY");

	if (Maths::GetSquaredLength(chefObject[KNIFE].getPosition(), target) < 50.f * 50.f)
	{
		if (!(player->GetState() == PlayerState::DODGE))
		{
			Audio::Get().PlaySound("FRANK_HIT");
			enemy->ApplyEffect(player);
			player->TakeDamage(enemy->GetDamage(), enemy->GetAttackType());
			Finish();
		}
	}

	if (chefObject[KNIFE].getPosition().x < -chefObject[KNIFE].getGlobalBounds().width)
	{
		CallResult(ResultDodge::DODGE);
		Finish();
	}
}

void FrankenChefFight::UpdateSpeAtk(FightingState* _fightingState)
{
	chefObject[KNIFE].move(vectorKnife * 1200.f * GameClock::Get().GetDeltaTime());
	chefObject[KNIFE].rotate(-1200.f * GameClock::Get().GetDeltaTime());

	Audio::Get().PlaySound("FRANK_FLY");

	if (Maths::GetSquaredLength(chefObject[KNIFE].getPosition(), target) < 50.f * 50.f)
	{
		DrawManager::Get().SetObjectVisible("FIGHT_HUD", &chefObject[KNIFE], false);

		InitSpecialFlying();
	}
}

void FrankenChefFight::UpdateSpeFall(FightingState* _fightingState)
{
	chefObject[fallingObject].move(target * speed * ((fallingObject+1) / 3.f) * GameClock::Get().GetDeltaTime());
	chefObject[fallingObject].rotate((target.x + 0.1f) * speed * (fallingObject / 3.f) * GameClock::Get().GetDeltaTime());

	if (player->getPosition().y - 100.f < chefObject[fallingObject].getPosition().y && target.x == 0.f)
	{
		Audio::Get().StopSound("FRANK_FALL");
		if (player->GetState() != PlayerState::DODGE)
		{
			Audio::Get().PlaySound("FRANK_ITEM_TOUCH");
			enemy->ApplyEffect(player);
			player->TakeDamage(enemy->GetDamage(), enemy->GetAttackType());
			Finish();
		}
		else
		{
			Audio::Get().PlaySound("FRANK_ITEM_DROP");
			target = sf::Vector2f(1.f, -1.f);
			CallResult(ResultDodge::DODGE);
			speed = 1300.f;
		}
	}

	if (chefObject[fallingObject].getPosition().x > WORKING_WIDTH)
	{
		Finish();
	}
}

void FrankenChefFight::Finish(void)
{
	for (auto& object : chefObject)
	{
		DrawManager::Get().SetObjectVisible("FIGHT_HUD", &object, false);
	}

	enemy->GetCurAnim()->Play();

	FinishMiniGame();
}

void FrankenChefFight::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(textResult, states);
}

void FrankenChefFight::InitSounds(void)
{
	auto& audioMng = Audio::Get();

	audioMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Enemies/FrankenchiefLaunch.wav", "FRANK_LAUNCH", 80.f);
	audioMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Enemies/FrankenchiefFly.wav", "FRANK_FLY", 25.f);
	audioMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Enemies/FrankenchiefFall.wav", "FRANK_FALL", 30.f);
	audioMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Enemies/FrankenchiefItemTouch.wav", "FRANK_ITEM_TOUCH", 90.f);
	audioMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Enemies/FrankenchiefItemDrop.wav", "FRANK_ITEM_DROP");
	audioMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Enemies/FrankenchiefHit.wav", "FRANK_HIT", 95.f);
}

void FrankenChefFight::DeleteSounds(void)
{
	auto& audioMng = Audio::Get();

	audioMng.DeleteSoundAndBuffer("FRANK_LAUNCH");
	audioMng.DeleteSoundAndBuffer("FRANK_FLY");
	audioMng.DeleteSoundAndBuffer("FRANK_FALL");
	audioMng.DeleteSoundAndBuffer("FRANK_ITEM_TOUCH");
	audioMng.DeleteSoundAndBuffer("FRANK_ITEM_DROP");
	audioMng.DeleteSoundAndBuffer("FRANK_HIT");
}