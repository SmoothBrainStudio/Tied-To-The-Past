#include "Definitions.hpp"
#include "Player.hpp"
#include "InGameDatas.hpp"
#include "Status.hpp"
#include "Burning.hpp"
#include "Bleeding.hpp"
#include "AttackBuff.hpp"
#include "BreakAttack.hpp"
#include "DefBuff.hpp"
#include "BreakDef.hpp"
#include "Immunity.hpp"
#include "AntiBuff.hpp"
#include "Stun.hpp"
//Copyright 2024 Property of Olivier Maurin.All rights reserved.

Player::Player(void) : Entity()
{
	hpMax = 200;
	hp = hpMax;
	speed = 500.f;
	possessedSweets = 0;
	damage = 15;
	initialDamage = damage;
	state = PlayerState::IDLE;

	InitAnimations();
	InitPlayerSprite();
	InitPhilacterySprite();
	InitPickedItem();
	punchCollider.setSize(sf::Vector2f(60.f, 40.f));
	punchCollider.setOrigin(punchCollider.getSize().x / 2.f, punchCollider.getSize().y / 2.f);
	punchCollider.setFillColor(sf::Color::Transparent);
	punchCollider.setOutlineThickness(1.f);
	punchCollider.setOutlineColor(sf::Color::Magenta);

	shadow.setFillColor(sf::Color(0, 0, 0, 50));
	shadow.setScale(1.f, 0.5f);

	animations[costumeState][(int)PlayerState::IDLE]->Reset();
	animations[costumeState][(int)PlayerState::MOVE]->Reset();
	UpdateShadow();

	for (auto& particle : particles)
	{
		particle->setPosition(getPosition().x + getOrigin().x, getPosition().y + getOrigin().y - GetGlobalAABB().height / 2.f);
	}

	collideBox.setSize(sf::Vector2f(112.f, 207.f));
	collideBox.setOrigin(collideBox.getSize().x / 2.f, collideBox.getSize().y);

	feetHitbox.setSize(sf::Vector2f(GetFeetCollisionBox().width, GetFeetCollisionBox().height));
	feetHitbox.setOrigin(sf::Vector2f(feetHitbox.getSize().x / 2.f, feetHitbox.getSize().y));

	//DrawManager::Get().AddObject("ExplorationUnSortables", punchCollider);
}

Player::~Player(void)
{
	TexturesManager& TextursMng = TexturesManager::Get();

	TextursMng.Delete("SpritePhylactery_NORMAL");
	TextursMng.Delete("SpritePhylactery_TIE");
	TextursMng.Delete("SpritePhylactery_KEY");
	TextursMng.Delete("SpritePhylactery_WALL");
	TextursMng.Delete("SpritePhylactery_CANDLE");
	TextursMng.Delete("SpritePhylactery_LOCKED");
	TextursMng.Delete("SpritePhylactery_SAVE");

	for (size_t i = 0; i < candleAttackAnimations.size(); i++)
	{
		delete candleAttackAnimations[i];
		candleAttackAnimations[i] = nullptr;
	}
	animations[CANDLE][(int)PlayerState::ATTACK] = nullptr;

	for (size_t i = 0; i < candleIdleAnimations.size(); i++)
	{
		delete candleIdleAnimations[i];
		candleIdleAnimations[i] = nullptr;
	}
	animations[CANDLE][(int)PlayerState::IDLE] = nullptr;

	for (size_t i = 0; i < boxerAttackAnimations.size(); i++)
	{
		delete boxerAttackAnimations[i];
		boxerAttackAnimations[i] = nullptr;
	}
	animations[BOXER][(int)PlayerState::ATTACK] = nullptr;


	for (size_t i = 0; i < animations.size(); i++)
	{
		for (size_t j = 0; j < animations[i].size(); j++)
		{
			if (animations[i][j] != nullptr)
			{
				delete animations[i][j];
				animations[i][j] = nullptr;
			}
		}
	}
}

void Player::InitPlayerSprite(void)
{
	DrawManager& drawMng = DrawManager::Get();
	TexturesManager& TextursMng = TexturesManager::Get();

	curAnim = animations[BASIC][(int)PlayerState::IDLE];

	drawMng.AddObject("Exploration", *this);
}

void Player::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
{
	_states.transform *= getTransform();
	if (hp > 0)
	{
		_target.draw(shadow, _states);
	}
	_target.draw(*curAnim, _states);
	_target.draw(pickedItem, _states);

	if (smoke.GetStatus() == AnimatedSprite::Status::PLAYING)
	{
		_target.draw(smoke, _states);
	}
}

void Player::InitPhilacterySprite(void)
{
	DrawManager& drawMng = DrawManager::Get();
	TexturesManager& TextursMng = TexturesManager::Get();
	sf::Vector2f playerPos = getPosition();
	sf::Vector2f phylacteryPos;

	TextursMng.Add("Contents/Assets/Sprites/Entities/Player/PhylacteryInteract.png", "SpritePhylactery_NORMAL");
	TextursMng.Add("Contents/Assets/Sprites/Entities/Player/PhylacteryMysteryTie.png", "SpritePhylactery_TIE");
	TextursMng.Add("Contents/Assets/Sprites/Entities/Player/PhylacteryKeyMystery.png", "SpritePhylactery_KEY");
	TextursMng.Add("Contents/Assets/Sprites/Entities/Player/Broken_Wall.png", "SpritePhylactery_WALL");
	TextursMng.Add("Contents/Assets/Sprites/Entities/Player/Candle.png", "SpritePhylactery_CANDLE");
	TextursMng.Add("Contents/Assets/Sprites/Entities/Player/Door_Locked.png", "SpritePhylactery_LOCKED");
	TextursMng.Add("Contents/Assets/Sprites/Entities/Player/Save.png", "SpritePhylactery_SAVE");

	phylactery.setTexture(TexturesManager::Get()["SpritePhylactery_NORMAL"]);
	phylactery.setOrigin(sf::Vector2f(phylactery.getGlobalBounds().width / 2.f, phylactery.getGlobalBounds().height / 2.f));

	phylacteryPos.x = playerPos.x;
	phylacteryPos.y = playerPos.y - GetGlobalAABB().height - phylactery.getGlobalBounds().height / 1.5f;

	phylactery.setPosition(phylacteryPos);
	drawMng.AddObject("ExplorationUnSortables", phylactery);
	drawMng.SetObjectVisible("ExplorationUnSortables", &phylactery, false);
}

void Player::InitPickedItem(void)
{
	pickedItem.setColor(sf::Color::Transparent);
}

void Player::InitAnimations(void)
{
	//basic tie animations
	candleIdleAnimations[0] = new AnimatedSprite("Contents/Data/Animations/Entities/Player/Candle/idle_fight.json");
	candleIdleAnimations[1] = new AnimatedSprite("Contents/Data/Animations/Entities/Player/Candle/idle_front.json");

	animations[BASIC][(int)PlayerState::IDLE] = new AnimatedSprite("Contents/Data/Animations/Entities/Player/Basic/idle_front.json");

	animations[BASIC][(int)PlayerState::MOVE] = new AnimatedSprite("Contents/Data/Animations/Entities/Player/Basic/move_down.json");

	//candle animations
	animations[CANDLE][(int)PlayerState::IDLE] = candleIdleAnimations[1];

	animations[CANDLE][(int)PlayerState::MOVE] = new AnimatedSprite("Contents/Data/Animations/Entities/Player/Candle/move_down.json");

	animations[CANDLE][(int)PlayerState::DODGE] = new AnimatedSprite("Contents/Data/Animations/Entities/Player/Candle/dodge_front.json");

	animations[CANDLE][(int)PlayerState::DEATH] = new AnimatedSprite("Contents/Data/Animations/Entities/Player/Candle/death_front.json");
	animations[CANDLE][(int)PlayerState::COUNTER] = new AnimatedSprite("Contents/Data/Animations/Entities/Player/Candle/counter_front.json");
	animations[CANDLE][(int)PlayerState::DAMAGE] = new AnimatedSprite("Contents/Data/Animations/Entities/Player/Candle/damage_front.json");


	for (size_t i = 0; i < candleAttackAnimations.size(); i++)
	{
		candleAttackAnimations[i] = new AnimatedSprite("Contents/Data/Animations/Entities/Player/Candle/attack_front_" + std::to_string(i) + ".json");
	}

	animations[CANDLE][(int)PlayerState::ATTACK] = candleAttackAnimations[0];
	animations[CANDLE][(int)PlayerState::ATTACK]->SetLoop(false);


	//boxer animations
	animations[BOXER][(int)PlayerState::IDLE] = new AnimatedSprite("Contents/Data/Animations/Entities/Player/Boxer/idle_front.json");


	animations[BOXER][(int)PlayerState::MOVE] = new AnimatedSprite("Contents/Data/Animations/Entities/Player/Boxer/move_down.json");

	boxerAttackAnimations[0] = new AnimatedSprite("Contents/Data/Animations/Entities/Player/Boxer/punch_down.json");
	boxerAttackAnimations[1] = new AnimatedSprite("Contents/Data/Animations/Entities/Player/Boxer/missedPunch.json");


	animations[BOXER][(int)PlayerState::ATTACK] = boxerAttackAnimations[0];

	animations[BOXER][(int)PlayerState::DODGE] = new AnimatedSprite("Contents/Data/Animations/Entities/Player/Boxer/dodge_front.json");

	animations[BOXER][(int)PlayerState::DEATH] = new AnimatedSprite("Contents/Data/Animations/Entities/Player/Boxer/death_front.json");
	animations[BOXER][(int)PlayerState::COUNTER] = new AnimatedSprite("Contents/Data/Animations/Entities/Player/Boxer/counter_front.json");

	animations[BOXER][(int)PlayerState::CHARGING_ATTACK] = new AnimatedSprite("Contents/Data/Animations/Entities/Player/Boxer/charging_attack.json");

	animations[BOXER][(int)PlayerState::DAMAGE] = new AnimatedSprite("Contents/Data/Animations/Entities/Player/Boxer/damage_front.json");


	// Music animations
	animations[MUSIC][(int)PlayerState::IDLE] = new AnimatedSprite("Contents/Data/Animations/Entities/Player/Music/idle_front.json");

	animations[MUSIC][(int)PlayerState::MOVE] = new AnimatedSprite("Contents/Data/Animations/Entities/Player/Music/move_down.json");

	animations[MUSIC][(int)PlayerState::ATTACK] = new AnimatedSprite("Contents/Data/Animations/Entities/Player/Music/attack_front.json");

	animations[MUSIC][(int)PlayerState::DODGE] = new AnimatedSprite("Contents/Data/Animations/Entities/Player/Music/dodge_front.json");

	animations[MUSIC][(int)PlayerState::DEATH] = new AnimatedSprite("Contents/Data/Animations/Entities/Player/Music/death_front.json");
	animations[MUSIC][(int)PlayerState::COUNTER] = new AnimatedSprite("Contents/Data/Animations/Entities/Player/Music/counter_front.json");

	animations[MUSIC][(int)PlayerState::DAMAGE] = new AnimatedSprite("Contents/Data/Animations/Entities/Player/Music/damage_front.json");
	animations[MUSIC][(int)PlayerState::CHARGING_ATTACK] = new AnimatedSprite("Contents/Data/Animations/Entities/Player/Music/charging_attack.json");
	//other
	smoke = AnimatedSprite("Contents/Data/Animations/Other/smoke.json");
}

void Player::PollEvent(sf::Event& _event)
{
	if (ControlsManager::Get().IsPressedInput(_event, "HIT") && costumeState == BOXER && (state == PlayerState::IDLE || state == PlayerState::MOVE))
	{
		isPunching = true;
		lastState = state;
		SetState(PlayerState::ATTACK);
		audio.PlaySoundNoSecurity("PUNCH_WOOSH");
		if (lastVectorMove.x < 0.f)
		{
			punchCollider.setPosition(getPosition().x - GetGlobalAABB().width / 2.f,
				getPosition().y - GetGlobalAABB().height / 2.f + GetGlobalAABB().height / 10.f);
		}
		else
		{
			punchCollider.setPosition(getPosition().x + GetGlobalAABB().width / 2.f,
				getPosition().y - GetGlobalAABB().height / 2.f + GetGlobalAABB().height / 10.f);
		}
	}

#ifdef _DEBUG
	//maybe temp
	for (unsigned short int i = 0; i < NB_COSTUMES; ++i)
	{
		if (_event.type == sf::Event::KeyPressed && _event.key.code == ((sf::Keyboard::Key)(27 + i)) && (state == PlayerState::IDLE || state == PlayerState::MOVE)) //27 = Num1
		{
			if (i <= GetTies().size())
			{
				SetCostumeState((CostumeState)i);
			}
		}
	}
#endif // _DEBUG
}

void Player::Update(InGameDatas& _inGameDatas)
{
	if (isInFight)
	{
		if (state != PlayerState::MOVE)
		{
			lastVectorMove.x = 1.f; //face down and right for fights
		}

		for (int i = 0; i < textDamages.size(); i++)
		{
			textDamages[i]->Update();
			if (textDamages[i]->IsDelete())
			{
				textDamages[i].reset();
				textDamages.erase(textDamages.begin() + i);
			}
		}

		// Position effect's icons on player
		nbIconX = 0;
		nbIconFloorY = 0;
		for (int i = 0; i < statutes.size(); i++)
		{
			const float posX = getPosition().x - 20.f - GetGlobalAABB().width - 75.f * nbIconX;
			if (i % 3 == 0 && i != 0)
			{
				nbIconX = 0;
				nbIconFloorY++;
			}
			const float posY = getPosition().y - GetGlobalAABB().height - 25.f + 75.f * nbIconFloorY;
			statutes[i]->setPosition(posX, posY);
			nbIconX++;

			statutes[i]->Update();

		}


		particles[(int)ParticleTypes::DAMAGE]->Update();


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

		if (!isAlive && state != PlayerState::DEATH)
		{
			switch (_inGameDatas.player->GetCostumeState())
			{
			case BOXER:
				audio.ChangeBuffer("PLAYER_DEATH", "BOXER_DEATH");
				audio.PlaySound("PLAYER_DEATH", 70.f);
				break;
			case MUSIC:
				audio.ChangeBuffer("PLAYER_DEATH", "BOXER_DEATH");
				audio.PlaySound("PLAYER_DEATH", 70.f);
				break;
			case CANDLE:
				audio.ChangeBuffer("PLAYER_DEATH", "CANDLE_DEATH");
				audio.PlaySound("PLAYER_DEATH", 70.f);
				break;
			default:
				break;
			}
			SetState(PlayerState::DEATH);
			curAnim->Reset();
			curAnim->Play();
		}
	}
	else
	{
		UpdatePhylacteryPos();
		lastPos = getPosition();
		Move();
		CheckPlayerCollidingWithMapBorders(_inGameDatas.camera->GetBorders());
		ResolveWallCollision(_inGameDatas.map->GetCollisions());

		for (auto& crate : _inGameDatas.crateVect)
		{
			if (!crate->GetIsDestroy())
			{
				CollisionManager::StopMovementOnObstacleEdge(*this, crate->GetCollisionBox());
			}
		}
		for (auto& wall : _inGameDatas.wallsRiddle)
		{
			if (!wall->IsDestroyed())
			{
				CollisionManager::StopMovementOnObstacleEdge(*this, wall->GetCollisionBox());
			}
		}
	}


	UpdateAnimations();
	sf::FloatRect rect = curAnim->GetSprite().getGlobalBounds();
	setOrigin(rect.width / 2.f, rect.height);

	UpdateShadow();
	UpdatePickedItem();

	UpdateCollide();
}

void Player::Move(void)
{
	if (state == PlayerState::MOVE || state == PlayerState::IDLE)
	{
		ControlsManager& controlsM = ControlsManager::Get();

		vectorMove = { 0.f, 0.f };
		switch (controlsM.GetControllerType())
		{
		case KEYBOARD:
			if (controlsM.IsHeldInput("UP"))
			{
				vectorMove.y--;
			}
			if (controlsM.IsHeldInput("DOWN"))
			{
				vectorMove.y++;
			}
			if (controlsM.IsHeldInput("LEFT"))
			{
				vectorMove.x--;
			}
			if (controlsM.IsHeldInput("RIGHT"))
			{
				vectorMove.x++;
			}
			break;
		case CONTROLLER:
			vectorMove = controlsM.GetJoystickLeftAxis();
			break;
		default:
			std::cerr << "Can't find Controller Type for player move" << std::endl;
			break;
		}

		if (vectorMove.x != 0 || vectorMove.y != 0)
		{
			lastState = state;
			SetState(PlayerState::MOVE);
			float angle = atan2f(vectorMove.y, vectorMove.x);
			sf::Vector2f movement;
			movement.x = speed * GameClock::Get().GetDeltaTime() * cosf(angle);
			movement.y = speed * GameClock::Get().GetDeltaTime() * sinf(angle);
			move(movement);

			lastVectorMove = vectorMove;

			// Play walk sound			 
			if (costumeState == CostumeState::CANDLE)
			{
				if (curAnim->GetCurrentFrame() == 2)
				{
					audio.PlaySound("PLAYER_WALK", 40.f);
				}
			}
			else
			{
				audio.PlaySound("PLAYER_WALK", 10.f);
			}
		}
		else
		{
			lastState = state;
			SetState(PlayerState::IDLE);
			audio.StopSound("PLAYER_WALK");
		}
	}

	sf::Vector3f listnerPos = { getPosition().x, getPosition().y, 0.f };
	sf::Listener::setPosition(listnerPos);
}

void Player::UpdateAnimations(void)
{
	if (state == PlayerState::SPEAK || state == PlayerState::INTERACT)
	{
		curAnim = animations[costumeState][(int)PlayerState::IDLE];
	}
	else
	{
		curAnim = animations[costumeState][(int)state];
	}

	if (animations[BOXER][(int)PlayerState::ATTACK]->GetStatus() == AnimatedSprite::Status::STOPPED && costumeState == BOXER && isPunching && state == PlayerState::ATTACK)
	{
		isPunching = false;
		lastState = PlayerState::IDLE;
		curAnim->Reset();
		SetState(PlayerState::IDLE);
	}

	if (animations[CANDLE][(int)PlayerState::ATTACK]->GetStatus() == AnimatedSprite::Status::STOPPED && costumeState == CANDLE && state == PlayerState::ATTACK)
	{
		lastState = PlayerState::IDLE;
		curAnim->Reset();
		SetState(PlayerState::IDLE);

		animations[CANDLE][(int)PlayerState::ATTACK]->Reset();
	}

	if (animations[CostumeState::BOXER][(int)PlayerState::CHARGING_ATTACK]->GetStatus() == AnimatedSprite::Status::STOPPED && state == PlayerState::CHARGING_ATTACK)
	{
		lastState = PlayerState::IDLE;
		curAnim->Reset();
		SetState(PlayerState::IDLE);
	}

	if (costumeState != CostumeState::BASIC)
	{
		if (animations[costumeState][(int)PlayerState::DODGE]->GetStatus() == AnimatedSprite::Status::STOPPED && state == PlayerState::DODGE)
		{
			lastState = PlayerState::IDLE;
			curAnim->Reset();
			SetState(PlayerState::IDLE);
		}

		if (animations[costumeState][(int)PlayerState::DAMAGE]->GetStatus() == AnimatedSprite::Status::STOPPED && state == PlayerState::DAMAGE)
		{
			lastState = PlayerState::IDLE;
			curAnim->Reset();
			SetState(PlayerState::IDLE);
		}
	}

	if (lastVectorMove.x < 0.f)
	{
		setScale(1.f, 1.f);
	}
	else
	{
		setScale(-1.f, 1.f);
	}

	smoke.Update();
	curAnim->Update();
}

void Player::UpdatePhylacteryPos(void)
{
	sf::Vector2f playerPos = getPosition();
	sf::Vector2f phylacteryPos;
	phylacteryPos.x = playerPos.x;
	phylacteryPos.y = playerPos.y - GetGlobalAABB().height - phylactery.getGlobalBounds().height / 1.5f;
	phylactery.setPosition(phylacteryPos);
}

void Player::UpdateShadow()
{
	sf::FloatRect rect = GetGlobalAABB();

	shadow.setScale(1.f, 0.5f);
	shadow.setRadius(rect.width * 0.5f);
	shadow.setOrigin(shadow.getRadius(), shadow.getRadius());
	shadow.setPosition(getOrigin());
}

void Player::UpdatePickedItem(void)
{
	if (pickedItemTimeRemaining == 0.f)
	{
		return;
	}

	pickedItemTimeRemaining -= GameClock::Get().GetDeltaTime();
	if (pickedItemTimeRemaining <= 0.f)
	{
		pickedItemTimeRemaining = 0.f;
		pickedItem.setColor(sf::Color::Transparent);
		return;
	}

	sf::FloatRect itemRect = pickedItem.getGlobalBounds();
	pickedItem.setPosition(GetGlobalAABB().width / 2.f - itemRect.width / 2.f, -itemRect.height);

	const float alpha = 1.f - Maths::GetPower(1.f - pickedItemTimeRemaining / PICKED_ITEM_SHOW_TIME, 4);
	pickedItem.setColor(sf::Color(255, 255, 255, sf::Uint8(alpha * 255)));
}

void Player::CheckPlayerCollidingWithMapBorders(const CameraBorder& _border)
{
	sf::Vector2f playerPos = getPosition();
	sf::FloatRect playerAABB = GetFeetCollisionBox();

	if (getPosition().y < (playerAABB.height + _border.offsetY)) //boxCollision Up
	{
		setPosition(getPosition().x, playerAABB.height + _border.offsetY);
	}
	if (getPosition().y > _border.maxY) //boxCollision Down
	{
		setPosition(getPosition().x, _border.maxY);
	}
	if (getPosition().x < (playerAABB.width / 2.f + _border.offsetX)) //boxCollision Left
	{
		setPosition(playerAABB.width / 2.f + _border.offsetX, getPosition().y);
	}
	if (getPosition().x > _border.maxX - playerAABB.width / 2.f) //boxCollision Right
	{
		setPosition(_border.maxX - playerAABB.width / 2.f, getPosition().y);
	}
}

sf::Sprite& Player::GetPhylactery(void)
{
	return phylactery;
}

const PlayerState& Player::GetState(void) const
{
	return state;
}

const CostumeState Player::GetCostumeState(void) const
{
	return (CostumeState)costumeState;
}

const bool& Player::GetIsPunching(void) const
{
	return isPunching;
}

const std::vector<std::shared_ptr<Item>>& Player::GetItems() const
{
	return this->items;
}

std::vector<std::string> Player::GetKeyIds() const
{
	std::vector<std::string> importantItems;
	for (auto& item : this->items)
	{
		if (item->GetType() == Item::IMPORTANT)
		{
			importantItems.push_back(item->GetId());
		}
	}
	return importantItems;
}

std::vector<std::shared_ptr<Consumable>> Player::GetConsumables() const
{
	std::vector<std::shared_ptr<Consumable>> consumables;
	for (auto& item : this->items)
	{
		if (item->GetType() == Item::CONSUMABLE)
		{
			std::shared_ptr<Consumable> consumable = std::dynamic_pointer_cast<Consumable>(item);
			if (consumable->GetCount())
			{
				consumables.push_back(consumable);
			}
		}
	}
	return consumables;
}

std::vector<std::shared_ptr<Item>> Player::GetImportantItems() const
{
	std::vector<std::shared_ptr<Item>> items;
	for (auto& item : this->items)
	{
		if (item->GetType() == Item::CONSUMABLE)
		{
			items.push_back(item);
		}
	}
	return items;
}

std::vector<std::shared_ptr<Tie>> Player::GetTies() const
{
	std::vector<std::shared_ptr<Tie>> ties;
	for (auto& item : this->items)
	{
		if (item->GetType() == Item::TIE)
		{
			ties.push_back(std::dynamic_pointer_cast<Tie>(item));
		}
	}
	return ties;
}

sf::Int16 Player::GetPossessedSweets() const
{
	return possessedSweets;
}

const sf::RectangleShape& Player::GetPunchCollider(void) const
{
	return punchCollider;
}

const bool Player::GetIsInPunchingFrame(void) const
{
	if (curAnim->GetCurrentFrame() == 3)
	{
		return true;
	}
	else
	{
		return false;
	}
}

const sf::Vector2f& Player::GetLastPosExplo(void) const
{
	return lastPosExplo;
}

const bool& Player::GetIsInFight(void) const
{
	return isInFight;
}

AnimatedSprite* Player::GetCurAnim(void)
{
	return curAnim;
}

const bool Player::GetXDir(void)
{
	return (lastVectorMove.x >= 0.f); // false = LEFT, true = RIGHT
}

const std::array<std::array<AnimatedSprite*, (int)PlayerState::NB_STATES>, NB_COSTUMES>& Player::GetAnims(void) const
{
	return animations;
}

const bool& Player::CanTp(void)
{
	return canTp;
}

void Player::SetTp(const bool& _canTp)
{
	canTp = _canTp;
}

void Player::SetPhylacteryType(PhylacteryType _type)
{
	TexturesManager& TextursMng = TexturesManager::Get();

	switch (_type)
	{
	case Player::PhylacteryType::NORMAL:
		phylactery.setTexture(TextursMng["SpritePhylactery_NORMAL"]);
		break;
	case Player::PhylacteryType::TIE:
		phylactery.setTexture(TextursMng["SpritePhylactery_TIE"]);
		break;
	case Player::PhylacteryType::KEY:
		phylactery.setTexture(TextursMng["SpritePhylactery_KEY"]);
		break;
	case Player::PhylacteryType::WALL:
		phylactery.setTexture(TextursMng["SpritePhylactery_WALL"]);
		break;
	case Player::PhylacteryType::CANDLE:
		phylactery.setTexture(TextursMng["SpritePhylactery_CANDLE"]);
		break;
	case Player::PhylacteryType::LOCKED:
		phylactery.setTexture(TextursMng["SpritePhylactery_LOCKED"]);
		break;
	case Player::PhylacteryType::SAVE:
		phylactery.setTexture(TextursMng["SpritePhylactery_SAVE"]);
		break;
	default:
		break;
	}
}

void Player::SetPossessedSweets(sf::Int16 _amount)
{
	possessedSweets = _amount;
}

void Player::SetIsSpeaking(bool _bool)
{
	if (_bool && state != PlayerState::SPEAK)
	{
		lastState = state;
		state = PlayerState::SPEAK;
	}
	else if (!_bool && state != PlayerState::ATTACK)
	{
		lastState = PlayerState::IDLE;
		state = PlayerState::IDLE;
	}
}

void Player::ResolveWallCollision(const std::vector<sf::FloatRect>& walls)
{
	for (auto& wall : walls)
	{
		CollisionManager::StopMovementOnObstacleEdge(*this, wall);
	}
}

void Player::SetApplicableStatus(void)
{
	for (auto& effect : applicableStatutes)
	{
		delete effect.status;
		effect.status = nullptr;
	}
	applicableStatutes.clear();
	ApplicableEffects newEffet = {};
	for (int i = 0; i < curTie->GetStatutesProperties().size(); i++)
	{
		switch (curTie->GetStatutesProperties()[i].id)
		{
		case BURNING:
			newEffet.status = new Burning();
			break;
		case BLEEDING:
			newEffet.status = new Bleeding();
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
			Debug.warning("Can't find matching statue property with ", curTie->GetStatutesProperties()[i].id, " in function ", __FUNCTION__);
			break;
		}
		newEffet.probability = curTie->GetStatutesProperties()[i].probability;
		applicableStatutes.push_back(newEffet);
	}
}

void Player::AddItem(const Item& _item, const bool& _isFree, bool _show)
{
	if (!_isFree)
	{
		this->AddSweets(-_item.GetPrice());
	}

	if (_show)
	{
		this->pickedItemTimeRemaining = PICKED_ITEM_SHOW_TIME;
		this->pickedItem.setTexture(*_item.GetTexture(), true);
	}

	switch (_item.GetType())
	{
	case Item::CONSUMABLE:
	{
		auto it = std::find_if(this->items.begin(), this->items.end(), [&](const std::shared_ptr<Item>& _other)
			{
				return _item.GetId() == _other->GetId();
			});

		if (it == this->items.end())
		{
			this->items.push_back(std::make_shared<Consumable>(_item.GetId()));
		}
		else
		{
			((Consumable*)it->get())->Increase();
		}
		break;
	}
	case Item::TIE:
	{
		this->items.push_back(std::make_shared<Tie>(_item.GetId()));
		break;
	}

	default:
		this->items.push_back(std::make_shared<Item>(_item));
		break;
	}
}

void Player::RemoveItem(const std::string& _id)
{
	for (auto it = this->items.begin(); it != this->items.end(); ++it)
	{
		if ((*it)->GetId() == _id)
		{
			this->items.erase(it);
			return;
		}
	}
}

void Player::DecreaseItemStack(const std::string& _id)
{
	for (auto it = this->items.begin(); it != this->items.end(); ++it)
	{
		if ((*it)->GetId() == _id)
		{
			if ((*it)->GetType() == Item::CONSUMABLE)
			{
				Consumable* consumable = ((Consumable*)(*it).get());
				consumable->Decrease();
				if (consumable->GetCount() == 0)
				{
					this->items.erase(it);
				}
			}
			else
			{
				this->items.erase(it);
			}
			return;
		}
	}
}

void Player::AddTie(const std::shared_ptr<Tie>& _tie, sf::Int16 _price)
{
	std::shared_ptr<Item> item = _tie;
	hasNewTie = true;
	item->SetType(Item::TIE);
	this->items.push_back(_tie);
	curTie = _tie;
	this->possessedSweets -= _price;
}

void Player::AddSweets(sf::Int16 _nbSweets)
{
	int tempSweet = this->possessedSweets + _nbSweets;
	this->possessedSweets = (unsigned short)Maths::Clamp((float)tempSweet, 0.f, FLT_MAX);
	gotSweets = true;
}

void Player::SetCostumeState(const CostumeState& _costumeState)
{
	auto ties = this->GetTies();
	if ((int)_costumeState < ties.size() + 1 && _costumeState != costumeState) //+1 because of basic tie
	{
		smoke.Reset();
		smoke.Play();
		audio.PlaySoundNoSecurity("CHANGE_TIE");

		const sf::FloatRect rect = smoke.GetSprite().getGlobalBounds();
		const sf::FloatRect rectPly = GetGlobalAABB();
		smoke.setOrigin(rect.width / 2.f - rectPly.width / 2.f, rect.height / 2.f - rectPly.height / 2.f);

		costumeState = _costumeState;

		//changing tie for fighting ties

		if (DoesItHaveStatus(Entity::Effects::DEF_BREAK) || DoesItHaveStatus(Entity::Effects::DEF_BUFF))
		{
			for (unsigned int i = 0; i < NB_ATTACK_TYPES; i++)
			{
				curTie->SetResistance(i, curTie->GetInitialResistances()[i]);
			}
		}

		if (costumeState != BASIC)
		{
			curTie = ties[(size_t)(_costumeState - 1)]; //-1 because of the basic tie
			curTie->SetCostumeID((CostumeState)_costumeState);

			resistances = curTie->GetResistances();

			if (DoesItHaveStatus(Entity::Effects::DEF_BREAK))
			{
				AddResistanceCoef(BreakDef::DEF_BREAK_COEF);
			}
			if (DoesItHaveStatus(Entity::Effects::DEF_BUFF))
			{
				AddResistanceCoef(-DefBuff::DEF_BUFF_COEF);
			}
		}

		//reset anim to idle
		SetState(PlayerState::IDLE);
		animations[costumeState][(int)PlayerState::IDLE]->Reset();
		animations[costumeState][(int)PlayerState::MOVE]->Reset();
		UpdateShadow();

		// Change sound buffer
		if (_costumeState == CostumeState::CANDLE)
		{
			audio.ChangeBuffer("PLAYER_WALK", "CANDLE_WALK");
			audio.ChangeBuffer("LEAK", "CANDLE_WALK");
		}
		else
		{
			audio.ChangeBuffer("PLAYER_WALK", "NORMAL_WALK");
		}
	}
}

void Player::SetState(const PlayerState& _state)
{
	lastState = state;
	state = _state;
	if (state == PlayerState::SPEAK || state == PlayerState::INTERACT)
	{
		if (curAnim != animations[costumeState][(int)PlayerState::IDLE])
		{
			curAnim->Reset();
			curAnim = animations[costumeState][(int)PlayerState::IDLE];
			curAnim->Play();
		}
	}
	else
	{
		if (curAnim != animations[costumeState][(int)state])
		{
			curAnim->Reset();
			curAnim = animations[costumeState][(int)state];
			curAnim->Play();
		}
	}

	const sf::FloatRect rect = curAnim->GetSprite().getGlobalBounds();
	setOrigin(rect.width / 2.f, rect.height);
	UpdateShadow();
}

void Player::SetEnablePhylactery(const bool& _bool, PhylacteryType _type)
{
	DrawManager::Get().SetObjectVisible("ExplorationUnSortables", &GetPhylactery(), _bool);
	SetPhylacteryType(_type);
}

void Player::SetLastPosExplo(const sf::Vector2f& _pos)
{
	lastPosExplo = _pos;
}

void Player::SetIsInFight(const bool& _bool)
{
	isInFight = _bool;
}

void Player::SetCandleAttackAnimation(const unsigned short& _id)
{
	animations[CANDLE][(int)PlayerState::ATTACK] = candleAttackAnimations[_id];
}

void Player::SetBoxerAttackAnimation(const unsigned short& _id)
{
	animations[BOXER][(int)PlayerState::ATTACK] = boxerAttackAnimations[_id];
}

void Player::SetCandleIdleAnimation(const unsigned short& _id)
{
	animations[CANDLE][(int)PlayerState::IDLE] = candleIdleAnimations[_id];
}

void Player::SetXDir(const int& _xDir)
{
	if (_xDir == -1 || _xDir == 1)
	{
		lastVectorMove.x = static_cast<float>(_xDir);
		vectorMove.x = static_cast<float>(_xDir);
	}
}

void Player::MovePlayerInFight(const sf::Vector2f& _endPosition, const int& _dir, const float& _speed)
{
	if (_dir == -1)
	{
		if (_endPosition.x < getPosition().x)
		{
			move(sf::Vector2f(-_speed * GameClock::Get().GetDeltaTime(), 0.f));
		}
	}
	else if (_dir == 1)
	{
		if (_endPosition.x > getPosition().x)
		{
			move(sf::Vector2f(_speed * GameClock::Get().GetDeltaTime(), 0.f));
		}
	}
}

std::shared_ptr<Tie>& Player::GetCurTie()
{
	return curTie;
}

void Player::PlaySoundCounter(void)
{
	auto& audioMng = Audio::Get();

	switch (GetCostumeState())
	{
	case BOXER:
		audioMng.ChangeBuffer("COUNTER_ATTACK", "BOXER_COUNTER");
		break;
	case CANDLE:
		audioMng.ChangeBuffer("COUNTER_ATTACK", "CANDLE_COUNTER");
		break;
	case MUSIC:
		audioMng.ChangeBuffer("COUNTER_ATTACK", "MUSICIAN_COUNTER");
		break;
	default:
		break;
	}
	audioMng.StopAllSounds();
	audioMng.PlaySound("COUNTER_ATTACK");
}

void Player::TakeDamage(short _damageDealt, const AttackType& _attackType)
{
	_damageDealt = static_cast<short>((float)_damageDealt * GetResistances()[_attackType]);

	AddHP(-_damageDealt);
	if (_damageDealt != 0)
	{
		SetState(PlayerState::DAMAGE);
		GetCurAnim()->Reset();
		GetCurAnim()->Play();
		GetParticles()[(int)Entity::ParticleTypes::DAMAGE]->setPosition(getPosition().x, getPosition().y - GetGlobalAABB().height / 2.f);
		GetParticles()[(int)Entity::ParticleTypes::DAMAGE]->setRotation(90.f + Maths::PreciseRand(20.f, -20.f));
		GetParticles()[(int)Entity::ParticleTypes::DAMAGE]->Reset();
		GetParticles()[(int)Entity::ParticleTypes::DAMAGE]->Play();
	}

	// Create a new TextDamage object and add it to the list of damage texts
	textDamages.push_back(std::make_shared<TextDamage>(std::to_string(_damageDealt), getPosition() + DAMAGE_TEXT_PLAYER_OFFSET + sf::Vector2f(Maths::PreciseRand(DAMAGE_TEXT_RANDOM_OFFSET, -DAMAGE_TEXT_RANDOM_OFFSET), -GetSprite().getGlobalBounds().height)));
}

std::ostream& operator << (std::ostream& _buffer, Player& _player)
{
	_buffer << _player.hpMax << ' ';
	_buffer << _player.possessedSweets << ' ';
	_buffer << _player.costumeState << ' ';
	_buffer << _player.getPosition().x << ' ';
	_buffer << _player.getPosition().y << ' ';
	_buffer << _player.phylactery.getPosition().x << ' ';
	_buffer << _player.phylactery.getPosition().y << ' ';

	_buffer << _player.items.size() << ' ';
	for (auto& item : _player.items)
	{
		item->Save(_buffer);
		_buffer << ' ';
	}

	return _buffer;
}

std::istream& operator >> (std::istream& _buffer, Player& _player)
{
	_buffer >> _player.hpMax;
	_buffer >> _player.possessedSweets;

	short int costumeState;
	_buffer >> costumeState;
	_player.SetCostumeState(CostumeState(costumeState));

	sf::Vector2f playerPos;
	_buffer >> playerPos.x;
	_buffer >> playerPos.y;
	_player.setPosition(playerPos);

	sf::Vector2f phylacteryPos;
	_buffer >> phylacteryPos.x;
	_buffer >> phylacteryPos.y;
	_player.phylactery.setPosition(phylacteryPos);

	size_t itemsSize;
	_buffer >> itemsSize;

	while (!_player.items.empty())
	{
		_player.items.back().reset();
		_player.items.pop_back();
	}

	for (size_t i = 0; i < itemsSize; ++i)
	{
		std::shared_ptr<Item> item;
		std::string itemId;
		_buffer >> itemId;
		auto it = Item::prefabs.find(itemId);
		if (it == Item::prefabs.end())
		{
			auto it = Tie::prefabs.find(itemId);
			if (it != Tie::prefabs.end())
			{
				item = Tie::prefabs[itemId];
			}
		}
		else
		{
			switch (Item::prefabs[itemId]->GetType())
			{
			case Item::CONSUMABLE:
			{
				item = std::make_shared<Consumable>(itemId);
				break;
			}
			default:
				item = std::make_shared<Item>(itemId);
			}
		}
		item->Load(_buffer);
		_player.items.push_back(item);
	}

	return _buffer;
}
