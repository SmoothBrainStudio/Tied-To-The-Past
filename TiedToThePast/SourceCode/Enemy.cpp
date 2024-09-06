#include "Burning.hpp"
#include "Bleeding.hpp"
#include "AttackBuff.hpp"
#include "BreakAttack.hpp"
#include "DefBuff.hpp"
#include "BreakDef.hpp"
#include "Poison.hpp"
#include "Immunity.hpp"
#include "AntiBuff.hpp"
#include "Stun.hpp"
#include "Enemy.hpp"
#include "Fighting/ZombieMiniGame.hpp"
#include "Fighting/KnightMiniGame.hpp"
#include "FrankenchefMiniGame.hpp"
#include "InGameDatas.hpp"
#include "Fighting/FireBallMiniGame.hpp"
#include "Fighting/BossCandleMiniGame.hpp"
#include "Fighting/BigBooMiniGame.hpp"
#include "Fighting/VampireMiniGame.hpp"

std::unordered_map<std::string, std::unique_ptr<Enemy>> Enemy::prefabs;

void Enemy::LoadPrefabs(const std::vector<std::string>& _filePaths)
{
	TexturesManager& texturesMng = TexturesManager::Get();

	for (unsigned int i = 0; i < _filePaths.size(); i++)
	{
		std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
		enemy->name = GetNameWithoutExtension(_filePaths[i]);
		enemy->LoadEnemyData(_filePaths[i]);
		prefabs.insert({ enemy->name, std::move(enemy) });
	}
}

void Enemy::UnloadPrefabs(void)
{
	for (auto& enemy : Enemy::prefabs)
	{
		enemy.second->~Enemy();
		enemy.second.reset();
	}

	Enemy::prefabs.clear();
}

Enemy::Enemy(const Json::Value& _mapEnemyJson) : Entity()
{
	walkSoundName = name + "_WALK_SOUND_";
	for (int i = 0; i < 10; i++)
	{
		char temp = rand() % 26 + 65;
		walkSoundName += temp;
	}

	LoadCopy(*Enemy::prefabs[_mapEnemyJson["type"].asString()]);
	LoadMapData(_mapEnemyJson);
	LoadSpecificData();

	// Shadow
	shadow.setFillColor(sf::Color(0, 0, 0, 50));
	shadow.setScale(1.f, 0.5f);

	DrawManager::Get().AddObject("Exploration", *this);
}

Enemy::Enemy(const Enemy& _enemy) : Entity()
{
	LoadCopy(_enemy);
	LoadSpecificData();
}

Enemy::~Enemy(void)
{
	if (DrawManager::Get().IsObjectInSection("Exploration", this))
	{
		DrawManager::Get().DeleteObject("Exploration", this);
	}

	ClearStatutes();

	if (!walkSoundName.empty())
	{
		audio.DeleteSound(walkSoundName);
		if (name == "ZOMBIE")
		{
			audio.DeleteSound(walkSpeedSoundName);
		}
	}

	delete fightingData.miniGame;
	fightingData.miniGame = nullptr;
}

void Enemy::LoadCopy(const Enemy& _enemy)
{
	//NEED TO BE FREQUENTLY UPDATED WHEN YOU ADD NEW VALUES INTO JSON
	hp = _enemy.hp;
	hpMax = hp;
	speed = _enemy.speed;
	damage = _enemy.damage;
	initialSpeed = _enemy.initialSpeed;
	speedMultiplier = _enemy.speedMultiplier;
	initialDamage = _enemy.initialDamage;
	name = _enemy.name;

	patrolRadius = _enemy.patrolRadius;
	timerWhileAttackingMax = _enemy.timerWhileAttackingMax;
	timerBeforeAttackMax = _enemy.timerBeforeAttackMax;
	timerWhileAttacking = timerWhileAttackingMax;
	scope = _enemy.scope;

	std::vector<Entity::ApplicableEffects> applicableStatutesCopy = {};
	for (auto& status : _enemy.applicableStatutes)
	{
		ApplicableEffects copy;
		copy.probability = status.probability;
		copy.status = status.status->Clone();
		applicableStatutesCopy.push_back(std::move(copy));
	}
	applicableStatutes = std::move(applicableStatutesCopy);

	resistances = _enemy.resistances;
	immunities = _enemy.immunities;
	initialImmunities = immunities;
	attackType = _enemy.attackType;
	collideBox = _enemy.collideBox;
	feetHitbox = _enemy.feetHitbox;
	itemsLootable = _enemy.itemsLootable;
	sweetsLoot = _enemy.sweetsLoot;
	setOrigin(_enemy.getOrigin());

	animations = _enemy.animations;

	curAnim = &animations[animState];
	curAnim->Reset();

	angle = 0.f;

	// Shadow
	shadow.setFillColor(sf::Color(0, 0, 0, 50));
	shadow.setScale(1.f, 0.5f);

	walkSoundName = name + "_WALK_SOUND_";
	for (int i = 0; i < 10; i++)
	{
		char temp = rand() % 26 + 65;
		walkSoundName += temp;
	}
}

void Enemy::LoadEnemyData(const std::string& _path)
{
	DrawManager& drawManager = DrawManager::Get();

	const auto& enemyJson = JsonManager::LoadFromFile(_path);

	hp = enemyJson["hp"].asInt();
	hpMax = hp;
	speed = enemyJson["speed"].asFloat();
	damage = enemyJson["attack"].asInt();
	initialSpeed = speed;
	initialDamage = damage;

	patrolRadius = enemyJson["patrolRadius"].asFloat();
	timerWhileAttackingMax = enemyJson["timerWhileAttackingMax"].asFloat();
	timerBeforeAttackMax = enemyJson["timerBeforeAttackMax"].asFloat();
	timerWhileAttacking = timerWhileAttackingMax;
	scope = enemyJson["scope"].asFloat();
	speedMultiplier = enemyJson["speedMultiplier"].asFloat();

	animations.resize(NB_ANIMATIONS);

	// Set sprite data
	for (unsigned int i = 0; i < enemyJson["animations"].size(); i++)
	{
		int state = enemyJson["animations"][i]["state"].asInt();
		std::string path = enemyJson["animations"][i]["path"].asString();

		animations[state] = AnimatedSprite(path);
	}

	for (unsigned int i = 0; i < enemyJson["effects"].size(); i++)
	{
		ApplicableEffects newEffet = {};
		switch (enemyJson["effects"][i]["id"].asInt())
		{
		case BURNING:
			newEffet.status = new Burning();
			break;
		case BLEEDING:
			newEffet.status = new Bleeding();
			break;
		case POISON:
			newEffet.status = new Poison();
			break;
		case ATK_BUFF:
			newEffet.status = new AttackBuff();
			break;
		case ATK_BREAK:
			newEffet.status = new BreakAttack();
			break;
		case DEF_BUFF:
			newEffet.status = new DefBuff();
			break;
		case DEF_BREAK:
			newEffet.status = new BreakDef();
			break;
		case IMMUNITY:
			newEffet.status = new Immunity();
			break;
		case ANTI_BUFF:
			newEffet.status = new AntiBuff();
			break;
		case STUN:
			newEffet.status = new Stun();
			break;
		default:
			break;
		}
		newEffet.probability = enemyJson["effects"][i]["probability"].asFloat();
		applicableStatutes.push_back(std::move(newEffet));
	}

	for (unsigned int i = 0; i < enemyJson["resistances"].size(); i++)
	{
		resistances[enemyJson["resistances"][i]["id"].asInt()] = enemyJson["resistances"][i]["coef"].asFloat();
	}

	for (unsigned int i = 0; i < enemyJson["immunities"].size(); i++)
	{
		immunities[enemyJson["immunities"][i]["id"].asInt()] = enemyJson["immunities"][i]["isImmune"].asBool();
	}

	initialImmunities = immunities;

	for (unsigned int i = 0; i < enemyJson["loot"].size(); i++)
	{
		std::shared_ptr<ItemLoot> tempItemLoot = std::make_shared<ItemLoot>();
		if (enemyJson["loot"][i]["type"].asString() == "TIE")
		{
			tempItemLoot->item = std::make_shared<Tie>((enemyJson["loot"][i]["id"].asString()));
		}
		else
		{
			tempItemLoot->item = std::make_shared<Item>((enemyJson["loot"][i]["id"].asString()));

		}

		tempItemLoot->probability = enemyJson["loot"][i]["probabilities"].asFloat();
		tempItemLoot->nbMax = enemyJson["loot"][i]["nbMax"].asInt();
		this->itemsLootable.push_back(std::move(tempItemLoot));
	}
	int sweetMin = enemyJson["CandiesLootMin"].asInt();
	int sweetMax = enemyJson["CandiesLootMax"].asInt();
	if (sweetMax - sweetMin == 0)
	{
		sweetsLoot = sweetMax;
	}
	else
	{
		sweetsLoot = rand() % (sweetMax - sweetMin) + sweetMin;
	}
	attackType = (AttackType)enemyJson["attackType"].asInt();

	curAnim = &animations[animState];
	curAnim->Reset();
	sf::FloatRect rect = curAnim->GetSprite().getGlobalBounds();
	setOrigin(rect.width / 2.f, rect.height);

	angle = 0.f;

	collideBox.setSize(sf::Vector2f(enemyJson["collideBox"]["width"].asFloat(), enemyJson["collideBox"]["height"].asFloat()));
	collideBox.setOrigin(sf::Vector2f(collideBox.getSize().x / 2.f, collideBox.getSize().y));

	feetHitbox.setSize(sf::Vector2f(GetFeetCollisionBox().width, GetFeetCollisionBox().height));
	feetHitbox.setOrigin(sf::Vector2f(feetHitbox.getSize().x / 2.f, feetHitbox.getSize().y));
}

void Enemy::LoadSpecificData(void)
{
	if (name == "KNIGHT")
	{
		audio.AddSoundWithBuffer("KNIGHT_WALK", walkSoundName, 90.f);
		audio.SetRelativeToListner(walkSoundName, false, 250.f, 1.7f);
		moveFunc = std::bind(&Enemy::MoveKnight, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		fightingData.miniGame = new KnightMiniGame();
	}
	else if (name == "ZOMBIE")
	{
		for (int i = 0; i < 10; i++)
		{
			char temp = rand() % 26 + 65;
			walkSpeedSoundName += temp;
		}

		audio.AddSoundWithBuffer("ZOMBIE_PATROL", walkSoundName);
		audio.AddSoundWithBuffer("ZOMBIE_PATROL_FAST", walkSpeedSoundName);
		audio.SetRelativeToListner(walkSoundName, false, 250.f, 1.7f);
		audio.SetRelativeToListner(walkSpeedSoundName, false, 250.f, 1.7f);
		moveFunc = std::bind(&Enemy::MoveZombie, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		fightingData.miniGame = new ZombieMiniGame();
	}
	else if (name == "FIREBALL")
	{
		audio.AddSoundWithBuffer("FIREBALL_PATROL", walkSoundName);
		audio.SetRelativeToListner(walkSoundName, false, 250.f, 1.7f);
		moveFunc = std::bind(&Enemy::MoveFireBall, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		fightingData.miniGame = new FireBallMiniGame();
	}
	else if (name == "FRANKENCHIEF")
	{
		audio.AddSoundWithBuffer("FRANKENCHIEF_CUT", walkSoundName, 50.f);
		audio.SetRelativeToListner(walkSoundName, false, 250.f, 1.7f);
		moveFunc = std::bind(&Enemy::MoveFrankenchief, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		fightingData.miniGame = new FrankenChefFight();
	}
	else if (name == "WAX GOLEM")
	{
		audio.AddSoundWithBuffer("CANDLE_BOSS_IDLE", walkSoundName, 50.f);
		audio.SetRelativeToListner(walkSoundName, false, 400.f, 1.2f);
		moveFunc = std::bind(&Enemy::MoveCandleBoss, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		fightingData.miniGame = new BossCandleMiniGame();
	}
	else if (name == "BIG BOO")
	{
		audio.AddSoundWithBuffer("BOO_WALK", walkSoundName);
		audio.SetRelativeToListner(walkSoundName, false, 400.f, 1.2f);
		moveFunc = std::bind(&Enemy::MoveCandleBoss, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		fightingData.miniGame = new BigBooMiniGame();
	}
	else if (name == "VAMPIRE")
	{
		//TODO : Changer le son du vampire
		audio.AddSoundWithBuffer("VAMPIRE_WALK", walkSoundName, 40.f);
		audio.SetRelativeToListner(walkSoundName, false, 400.f, 1.3f);
		moveFunc = std::bind(&Enemy::MoveCandleBoss, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		fightingData.miniGame = new VampireMiniGame();
	}

	for (auto& particle : particles)
	{
		particle->setPosition(getPosition().x + getOrigin().x, getPosition().y + getOrigin().y - GetGlobalAABB().height / 2.f);
	}
}

void Enemy::LoadMapData(const Json::Value& _mapData)
{
	name = _mapData["type"].asString();
	uniqueID = _mapData["uniqueID"].asString();
	unlockBool = _mapData["unlockBool"].asString();

	directionHorizontal = _mapData["horizontal"].asBool() ? RIGHT : LEFT;
	if (directionHorizontal == LEFT)
	{
		TurnLeft();
	}
	else
	{
		TurnRight();
	}

	const sf::Vector2f enemyPos(_mapData["x"].asFloat() + _mapData["width"].asFloat() / 2.f, _mapData["y"].asFloat() + _mapData["height"].asFloat());
	setPosition(enemyPos);
	posBasePatrol = enemyPos;
	collideBox.setPosition(enemyPos);
}

void Enemy::Update(InGameDatas& _inGameDatas)
{
	if (isInFight)
	{
		for (int i = 0; i < textDamages.size(); i++)
		{
			textDamages[i]->Update();
			if (textDamages[i]->IsDelete())
			{
				textDamages[i].reset();
				textDamages.erase(textDamages.begin() + i);
			}
		}

		nbIconX = 0;
		nbIconFloorY = 0;
		for (int i = 0; i < statutes.size(); i++)
		{
			float posX = getPosition().x + GetGlobalAABB().width / 2.f + 75 * nbIconX;
			//48 c'est la size d'une box
			if (posX > WORKING_WIDTH - 48)
			{
				nbIconX = 0;
				nbIconFloorY++;
				posX = getPosition().x + GetGlobalAABB().width / 2.f + 75 * nbIconX;
			}
			float posY = getPosition().y - GetGlobalAABB().height - 25.f + 75 * nbIconFloorY;
			statutes[i]->setPosition(posX, posY);
			nbIconX++;
			statutes[i]->Update();
		}

		if (hp <= 0)
		{
			if (deathTimer == 0)
			{
				if (name == "KNIGHT")
				{
					PlayDeathSound("KNIGHT_DEATH");
				}
				else if (name == "ZOMBIE")
				{
					PlayDeathSound("ZOMBIE_DEATH", 50.f);
				}
				else if (name == "FIREBALL")
				{
					PlayDeathSound("FIREBALL_DEATH", 90.f);
				}
				else if (name == "FRANKENCHIEF")
				{
					PlayDeathSound("FRANKENCHIEF_DEATH");
				}
				else if (name == "WAX GOLEM")
				{
					PlayDeathSound("CANDLEBOSS_DEATH", 70.f);
				}
				else if (name == "BIG BOO")
				{
					PlayDeathSound("BOO_DEATH", 70.f);
				}
				else if (name == "VAMPIRE")
				{
					//TODO: changer le son ici
					PlayDeathSound("VAMPIRE_DEATH", 100.f);
				}

				// on pourrait opti comme ça mais vu qu'on utilise un son différent pour chaque son dans le code...
				// audio.ChangeBuffer("ENEMY_DEATH", name + "_DEATH");
				// audio.PlaySound("ENEMY_DEATH", 70.f);
			}

			deathTimer += GameClock::Get().GetDeltaTime();
			deathTimer = Maths::Clamp(deathTimer, 0.f, DEATH_ANIM_TIME);

			sf::Color colorFightingEnemy = curAnim->GetSprite().getColor();
			colorFightingEnemy.a = (sf::Uint8)(255 - deathTimer / DEATH_ANIM_TIME * 255);

			curAnim->SetColor(colorFightingEnemy);
			for (auto& status : statutes)
			{
				status->statusDisplay.icon.setColor(colorFightingEnemy);
			}

			//Debug.log(float(colorFightingEnemy.a));

			if (deathTimer >= DEATH_ANIM_TIME)
			{
				this->ClearStatutes();
				isAlive = false;
			}
		}
	}
	else
	{
		lastPos = getPosition();
		moveFunc(GameClock::Get().GetDeltaTime(), _inGameDatas.player, _inGameDatas.map->GetCollisions());
	}

	UpdateAnimations();
	sf::FloatRect rect = curAnim->GetSprite().getGlobalBounds();
	setOrigin(rect.width / 2.f, rect.height);
	UpdateShadow();

	if (animState == AnimationState::DAMAGE)
	{
		particles[(int)ParticleTypes::DAMAGE]->Update();
	}

	for (unsigned int i = 0; i < particles.size(); ++i)
	{
		if (i != (int)ParticleTypes::DAMAGE && particles[i]->GetStatue() == ParticleSystem::Statue::PLAYING)
		{
			particles[i]->Update();
			if (particles[i]->GetStatue() == ParticleSystem::Statue::STOPPED)
			{
				particles[i]->Reset();
			}
		}
	}

	UpdateCollide();
}

void Enemy::SetTimerBeforeAttackMax(const float& _timerBeforeAttack)
{
	timerBeforeAttackMax = _timerBeforeAttack;
}

void Enemy::AddAlly(std::shared_ptr<Enemy>& _ally)
{
	fightingData.allies.push_back(_ally);
}

void Enemy::SetIsInFight(const bool& _bool)
{
	isInFight = _bool;
}

void Enemy::SetLastPosExplo(const sf::Vector2f& _pos)
{
	lastPosExplo = _pos;
}

void Enemy::SetAnimationState(const AnimationState& _state)
{
	animState = _state;
	curAnim = &animations[animState];

	const sf::FloatRect rect = curAnim->GetSprite().getGlobalBounds();
	setOrigin(rect.width / 2.f, rect.height);
	UpdateShadow();
}

void Enemy::SetReadyToFight(void)
{
	if (name == "KNIGHT")
	{
		SetAnimationState(AnimationState::SPECIAL);
	}
	else
	{
		SetAnimationState(AnimationState::IDLE);
	}
	TurnLeft();
	speed = initialSpeed;
}

Enemy::FightingData& Enemy::GetFightingData()
{
	return fightingData;
}

const std::string& Enemy::GetName(void) const
{
	return name;
}

const float& Enemy::GetPatrolRadius(void) const
{
	return patrolRadius;
}

const float& Enemy::GetTimerWhileAttackingMax(void) const
{
	return timerWhileAttackingMax;
}

const float& Enemy::GetTimerBeforeAttackingMax(void) const
{
	return timerBeforeAttackMax;
}

const bool& Enemy::GetIsInFight(void) const
{
	return isInFight;
}

const sf::Vector2f& Enemy::GetLastPosExplo(void) const
{
	return lastPosExplo;
}

const std::vector<std::shared_ptr<Enemy>>& Enemy::GetAllies(void) const
{
	return fightingData.allies;
}

MiniGame* Enemy::GetMiniGame(void)
{
	return fightingData.miniGame;
}

void Enemy::ClearAllies(void)
{
	for (auto& ally : fightingData.allies)
	{
		ally->~Enemy();
		ally.reset();
	}
	fightingData.allies.clear();
}

const AttackType& Enemy::GetAttackType(void) const
{
	return attackType;
}

AnimatedSprite* Enemy::GetCurAnim(void)
{
	return curAnim;
}

const Enemy::AnimationState& Enemy::GetAnimationState(void) const
{
	return animState;
}

void Enemy::SetStateExploration(const StatesExploration _newState)
{
	stateExploration = _newState;
	timerBeforeAttack = timerBeforeAttackMax;
	timerWhileAttacking = timerWhileAttackingMax;
	speed = initialSpeed;
}

void Enemy::SetName(const std::string& _name)
{
	name = _name;
}

void Enemy::SetPatrolRadius(const float& _patrolRadius)
{
	patrolRadius = _patrolRadius;
}

void Enemy::SetTimerWhileAttackingMax(const float& _timerAttack)
{
	timerWhileAttackingMax = _timerAttack;
}


#pragma region Patrols and walls collision

void Enemy::MoveKnight(const float& _dt, std::shared_ptr<Player>& _player, const std::vector<sf::FloatRect>& _walls)
{
	sf::Vector2f posEnemy = getPosition();
	sf::Vector2f posPlayer = _player->getPosition();

	if (stateExploration == PATROL)
	{

		// Decrease timer which define if attack is possible
		if (timerBeforeAttack > 0)
		{
			timerBeforeAttack -= _dt;
		}

		CheckIfAttackIsPossible(_player, scope, speedMultiplier);
		if (lastStateExploration == ATTACK)
		{
			audio.StopSound(walkSoundName);
			lastStateExploration = PATROL;
		}
	}
	else if (stateExploration == ATTACK)
	{
		SetAnimationState(AnimationState::MOVE);
		// Decrease timer which define if attack is finish
		if (timerWhileAttacking > 0)
		{
			timerWhileAttacking -= _dt;
		}

		GoToPoint(_dt, posEnemy, posPlayer);
		BackToInitPosition(_dt, posEnemy);

		// Spatialize sound
		audio.SetSoundPosition(walkSoundName, getPosition());
		audio.PlaySound(walkSoundName, 90.f);
	}
}

void Enemy::MoveFireBall(const float& _dt, std::shared_ptr<Player>& _player, const std::vector<sf::FloatRect>& _walls)
{
	sf::Vector2f posEnemy = getPosition();
	sf::Vector2f posPlayer = _player->getPosition();
	SetAnimationState(AnimationState::MOVE);

	if (stateExploration == PATROL)
	{
		// Decrease timer which define if attack is possible
		if (timerBeforeAttack > 0)
		{
			timerBeforeAttack -= _dt;
		}

		RunCircularPatrol(_dt, posEnemy, 100.f, rotateAngle);
		CheckIfAttackIsPossible(_player, scope, speedMultiplier);

		if (lastStateExploration == ATTACK)
		{
			lastStateExploration = PATROL;
		}

	}
	else if (stateExploration == ATTACK)
	{
		// Decrease timer which define if attack is finish
		if (timerWhileAttacking > 0)
		{
			timerWhileAttacking -= _dt;
		}

		// Follow player
		GoToPoint(_dt, posEnemy, posPlayer);

		// Stop attack
		ReturnToCircularPatrol(posEnemy, rotateAngle);

		if (lastStateExploration == PATROL)
		{
			lastStateExploration = ATTACK;
		}
	}

	// Spatialize sound
	audio.SetSoundPosition(walkSoundName, getPosition());
	audio.PlaySound(walkSoundName);
}

void Enemy::MoveZombie(const float& _dt, std::shared_ptr<Player>& _player, const std::vector<sf::FloatRect>& _walls)
{
	SetAnimationState(AnimationState::MOVE);
	sf::Vector2f posEnemy = getPosition();
	sf::Vector2f posPlayer = _player->getPosition();

	if (stateExploration == PATROL)
	{
		// Decrease timer which define if attack is possible
		if (timerBeforeAttack > 0)
		{
			timerBeforeAttack -= _dt;
		}

		RunHorizontalPatrol(_dt, posEnemy);
		CheckIfAttackIsPossible(_player, scope, speedMultiplier, true);
		audio.StopSound(walkSpeedSoundName);
		// Spatialize sound
		audio.SetSoundPosition(walkSoundName, getPosition());
		audio.PlaySound(walkSoundName);
	}
	else if (stateExploration == ATTACK)
	{
		// Decrease timer which define if attack is finish
		if (timerWhileAttacking > 0)
		{
			timerWhileAttacking -= _dt;
		}

		GoToPoint(_dt, posEnemy, posPlayer);
		ReturnToHorizontalPatrol();
		audio.StopSound(walkSoundName);
		// Spatialize sound
		audio.SetSoundPosition(walkSpeedSoundName, getPosition());
		audio.PlaySound(walkSpeedSoundName);
	}

	ResolveCollisionHorizontal(_walls, _dt, _player->getPosition());
}

void Enemy::MoveCandleBoss(const float& _dt, std::shared_ptr<Player>& _player, const std::vector<sf::FloatRect>& _walls)
{
	sf::Vector2f posEnemy = getPosition();
	sf::Vector2f posPlayer = _player->getPosition();

	// Spatialize sound
	audio.SetSoundPosition(walkSoundName, getPosition());
	audio.PlaySound(walkSoundName);
}

void Enemy::PlayDeathSound(const std::string& _bufferName, const float& _soundVolume)
{
	audio.ChangeBuffer("ENEMY_DEATH", _bufferName);
	audio.PlaySound("ENEMY_DEATH", _soundVolume);
}

void Enemy::MoveFrankenchief(const float& _dt, std::shared_ptr<Player>& _player, const std::vector<sf::FloatRect>& _walls)
{
	sf::Vector2f posEnemy = getPosition();
	sf::Vector2f posPlayer = _player->getPosition();

	SetAnimationState(AnimationState::MOVE);

	// Spatialize sound
	audio.SetSoundPosition(walkSoundName, getPosition());

	if (curAnim->GetCurrentFrame() == 2)
	{
		audio.PlaySound(walkSoundName);
	}
}

void Enemy::RunHorizontalPatrol(float _dt, sf::Vector2f& _posEnemy)
{
	switch (directionHorizontal)
	{
	case Enemy::LEFT:
		setPosition(_posEnemy.x - speed * _dt, _posEnemy.y);
		if (_posEnemy.x < posBasePatrol.x - patrolRadius)
		{
			TurnRight();
		}
		break;
	case Enemy::RIGHT:
		setPosition(_posEnemy.x + speed * _dt, _posEnemy.y);
		if (_posEnemy.x > posBasePatrol.x + patrolRadius)
		{
			TurnLeft();
		}
		break;
	default:
		break;
	}
}

void Enemy::ResolveCollisionHorizontal(const std::vector<sf::FloatRect>& _walls, const float& _dt, const sf::Vector2f& _playerPos)
{
	// Get enemy position and hitbox
	sf::Vector2f enemyPos = getPosition();
	sf::FloatRect enemyRect = GetFeetCollisionBox();

	for (auto& wall : _walls)
	{
		// Calculate right and bottom pos of enemy and wall
		float enemyRight = enemyRect.left + enemyRect.width;
		float enemyBottom = enemyRect.top + enemyRect.height;

		float wallRight = wall.left + wall.width;
		float wallBottom = wall.top + wall.height;

		// Calculate an offset to avoid enemy to cross the wall
		float collideOffset = speed * SIDE_COLLISION_MULTIPLIER * _dt;

		// If enemy and wall are colliding, replace enemy depending is position relative to the wall
		if (Collision::AABBs(this->GetFeetCollisionBox(), wall))
		{
			// Blocked by wall if in attack state
			if (stateExploration == ATTACK)
			{
				if (enemyBottom - collideOffset < wall.top)
				{
					setPosition(enemyPos.x, wall.top);
					AvoidObstacleInHorizontalWay(_dt, enemyPos, _playerPos);
				}
				if (enemyRect.top + collideOffset > wallBottom)
				{
					setPosition(enemyPos.x, wallBottom + FOOT_HEIGHT);
					AvoidObstacleInHorizontalWay(_dt, enemyPos, _playerPos);
				}
				if (enemyRight - collideOffset < wall.left)
				{
					setPosition(wall.left - enemyRect.width / 2, enemyPos.y);
					AvoidObstacleInVerticalWay(_dt, enemyPos, _playerPos);
				}
				if (enemyRect.left + collideOffset > wallRight)
				{
					setPosition(wallRight + enemyRect.width / 2, enemyPos.y);
					AvoidObstacleInVerticalWay(_dt, enemyPos, _playerPos);
				}
			}
			else
			{
				// Bound on the wall and take a new pos for patrol if in patrol state
				if (enemyRight - collideOffset < wall.left)
				{
					setPosition(wall.left - enemyRect.width / 2, enemyPos.y);
					TurnLeft();
					posBasePatrol = { wall.left - patrolRadius - enemyRect.width / 2, enemyPos.y };
				}
				else if (enemyRect.left + collideOffset > wallRight)
				{
					setPosition(wallRight + enemyRect.width / 2, enemyPos.y);
					TurnRight();
					posBasePatrol = { wallRight + patrolRadius + enemyRect.width / 2, enemyPos.y };
				}
			}
		}
	}
}

void Enemy::RunCircularPatrol(float _dt, sf::Vector2f& _posEnemy, const float& _rotationSpeed, float& _rotateAngle)
{
	// Speed factor with fraction of the patrol radius divide by number of degrees in a circle to keep the same speed according to the patrol radius
	float rotateFactor = (1000 / patrolRadius) / 360;

	switch (directionHorizontal)
	{
	case Enemy::LEFT:
		_rotateAngle -= _dt * (_rotationSpeed * rotateFactor);

		if (_rotateAngle < -2 * PI)
		{
			_rotateAngle = 0.f;
		}

		if (_rotateAngle < -PI)
		{
			this->setScale(-1, 1);
		}
		else
		{
			this->setScale(1, 1);
		}

		break;
	case Enemy::RIGHT:
		_rotateAngle += _dt * (_rotationSpeed * rotateFactor);

		if (_rotateAngle > 2 * PI)
		{
			_rotateAngle = 0.f;
		}

		if (_rotateAngle > PI)
		{
			this->setScale(-1, 1);
		}
		else
		{
			this->setScale(1, 1);
		}

		break;
	default:
		std::cerr << "Problem with circular patrol" << std::endl;
		break;
	}

	setPosition(Maths::ProjectPoint(posBasePatrol, patrolRadius, _rotateAngle));
}
#pragma endregion Patrols and walls collision


void Enemy::CheckIfAttackIsPossible(std::shared_ptr<Player>& _player, const float& _attackScope, const float& _speedFactor, const bool& _hasVisionCone)
{
	sf::Vector2f enemyPos = getPosition();
	sf::Vector2f playerPos = _player->getPosition();
	sf::Vector2f vectorEnemyPlayer = Maths::CreateVector(enemyPos, playerPos);
	float distanceWithPlayer = Maths::GetSquaredLength(enemyPos, playerPos);
	bool isAttackPossible = distanceWithPlayer < _attackScope * _attackScope && timerBeforeAttack <= 0;

	if (_hasVisionCone)
	{
		if (Maths::DotProduct(vectorForward, vectorEnemyPlayer) > 0)
		{
			if (isAttackPossible)
			{
				LaunchAttack(_speedFactor);
			}
		}
	}
	else
	{
		if (isAttackPossible)
		{
			LaunchAttack(_speedFactor);
		}
	}
}

void Enemy::ReturnToHorizontalPatrol(void)
{
	// If enemy doesn't exceed time in attack
	if (timerWhileAttacking <= 0.f)
	{
		// Restore datas
		stateExploration = PATROL;
		posBasePatrol = getPosition();
		speed = initialSpeed;
		ResetTimeBeforeAttack();
	}
}

void Enemy::ReturnToCircularPatrol(sf::Vector2f& _posEnemy, float& _rotateAngle)
{
	// If enemy doesn't exceed time in attack
	if (timerWhileAttacking <= 0.f)
	{
		stateExploration = PATROL;
		lastStateExploration = ATTACK;
		_rotateAngle = angle;
		posBasePatrol = { _posEnemy.x - patrolRadius * cosf(angle), _posEnemy.y - patrolRadius * sinf(angle) };
		speed = initialSpeed;
		timerBeforeAttack = timerBeforeAttackMax;
	}
}

void Enemy::TurnLeft(void)
{
	directionHorizontal = LEFT;
	vectorForward = { -1, 0 };
	setScale(1, 1);
}

void Enemy::TurnRight(void)
{
	directionHorizontal = RIGHT;
	vectorForward = { +1, 0 };
	setScale(-1, 1);
}

bool Enemy::IsPointOnTheRight(sf::Vector2f& _posEnemy, const sf::Vector2f& _point)
{
	float diffX = _point.x - _posEnemy.x;

	return diffX > 0;
}

bool Enemy::IsPointIsDowner(sf::Vector2f& _posEnemy, const sf::Vector2f& _point)
{
	float diffY = _point.y - _posEnemy.y;

	return diffY > 0;
}

void Enemy::LaunchAttack(const float& _speedFactor)
{
	stateExploration = ATTACK;
	speed = initialSpeed * _speedFactor;
	timerWhileAttacking = timerWhileAttackingMax;
}

void Enemy::GoToPoint(const float& _dt, sf::Vector2f& _posEnemy, sf::Vector2f& _point)
{
	angle = Maths::GetAngle(_posEnemy, _point);

	setPosition(_posEnemy.x + _dt * cosf(angle) * speed, _posEnemy.y + _dt * sinf(angle) * speed);
	if (IsPointOnTheRight(_posEnemy, _point))
	{
		TurnRight();
	}
	else
	{
		TurnLeft();
	}
}

bool Enemy::BackToInitPosition(const float& _dt, sf::Vector2f& _enemyPos)
{
	// If enemy doesn't exceed time in attack
	if (timerWhileAttacking <= 0.f)
	{
		// Move to initial position
		if (Maths::GetSquaredLength(_enemyPos, posBasePatrol) > 16)
		{
			GoToPoint(_dt, _enemyPos, posBasePatrol);
			AvoidObstacleInHorizontalWay(_dt, _enemyPos, posBasePatrol);
			AvoidObstacleInVerticalWay(_dt, _enemyPos, posBasePatrol);
		}
		else
		{
			// Stop the movement and reset datas
			stateExploration = PATROL;
			speed = initialSpeed;
			ResetTimeBeforeAttack();
			lastStateExploration = ATTACK;
			SetAnimationState(AnimationState::IDLE);
		}
	}
	return true;
}

void Enemy::AvoidObstacleInHorizontalWay(float _dt, sf::Vector2f& _enemyPos, const sf::Vector2f& _destination)
{
	if (IsPointOnTheRight(_enemyPos, _destination))
	{
		_enemyPos.x += speed * _dt;
	}
	else
	{
		_enemyPos.x -= speed * _dt;
	}
}

void Enemy::AvoidObstacleInVerticalWay(float _dt, sf::Vector2f& _enemyPos, const sf::Vector2f& _destination)
{
	if (IsPointIsDowner(_enemyPos, _destination))
	{
		_enemyPos.y += speed * _dt;
	}
	else
	{
		_enemyPos.y -= speed * _dt;
	}
}

void Enemy::ResetTimeBeforeAttack()
{
	timerBeforeAttack = timerBeforeAttackMax;
}

bool Enemy::CanAttack()
{
	return (timerBeforeAttack <= 0);
}

void Enemy::UpdateShadow()
{
	sf::FloatRect rect = GetGlobalAABB();

	shadow.setScale(1.f, 0.5f);
	shadow.setRadius(rect.width * shadowRadiusFactor);
	shadow.setOrigin(shadow.getRadius(), shadow.getRadius());
	shadow.setPosition(getOrigin() + shadowOffset);
}

void Enemy::UpdateAnimations(void)
{
	curAnim = &animations[animState];

	if (animations[AnimationState::DAMAGE].GetStatus() == AnimatedSprite::Status::STOPPED && animState == AnimationState::DAMAGE)
	{
		if (name == "KNIGHT" && isInFight)
		{
			animState = AnimationState::SPECIAL;
		}
		else
		{
			animState = AnimationState::IDLE;
		}

		curAnim->Reset();
	}

	if (animations[AnimationState::ATTACKING].GetStatus() == AnimatedSprite::Status::STOPPED && animState == AnimationState::ATTACKING)
	{
		if (name == "KNIGHT" && isInFight)
		{
			animState = AnimationState::SPECIAL;
		}
		else
		{
			animState = AnimationState::IDLE;
		}

		curAnim->Reset();
	}

	if (animations[AnimationState::SPECIAL].GetStatus() == AnimatedSprite::Status::STOPPED && animState == AnimationState::SPECIAL && name != "KNIGHT" && name != "FIREBALL")
	{
		if (name == "KNIGHT" && isInFight)
		{
			animState = AnimationState::SPECIAL;
		}
		else
		{
			animState = AnimationState::IDLE;
		}

		curAnim->Reset();
	}

	curAnim->Update();
}

const int& Enemy::GetSweetsDrop()
{
	return sweetsLoot;
}

const std::vector<std::shared_ptr<Enemy::ItemLoot>>& Enemy::GetLootableItems()
{
	return this->itemsLootable;
}

void Enemy::SaveDeath(void)
{
	if (!uniqueID.empty())
	{
		BoolManager::Get()[uniqueID] = true;
	}

	if (!unlockBool.empty())
	{
		BoolManager::Get()[unlockBool] = true;
	}
}

void Enemy::TakeDamage(Player& _player)
{
	std::shared_ptr<Tie> tie = _player.GetCurTie();

	int damages = static_cast<short>((float)_player.GetDamage() * GetResistances()[tie->GetAttackType()] * tie->GetLevelCoef());
	//Debug.log("damages : ", damages);

	AddHP(-damages);
	if (damages != 0)
	{
		if (name != "BIG BOO")
		{
			SetAnimationState(AnimationState::DAMAGE);
			GetCurAnim()->Reset();
			GetCurAnim()->Play();
		}

		GetParticles()[(int)Entity::ParticleTypes::DAMAGE]->setPosition(getPosition().x, getPosition().y - GetGlobalAABB().height / 2.f);
		GetParticles()[(int)Entity::ParticleTypes::DAMAGE]->setRotation(90.f + Maths::PreciseRand(20.f, -20.f));
		GetParticles()[(int)Entity::ParticleTypes::DAMAGE]->Reset();
		GetParticles()[(int)Entity::ParticleTypes::DAMAGE]->Play();
	}

	// Create a new TextDamage object and add it to the list of damage texts
	textDamages.push_back(std::make_shared<TextDamage>(std::to_string(damages), getPosition() + sf::Vector2f(Maths::PreciseRand(DAMAGE_TEXT_RANDOM_OFFSET, -DAMAGE_TEXT_RANDOM_OFFSET), -GetSprite().getGlobalBounds().height)));
}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	if ((name == "FIREBALL" && animState != SPECIAL) || (name != "FIREBALL"))
	{
		target.draw(shadow, states);
	}

	target.draw(*curAnim, states);
}
