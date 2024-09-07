#include "Definitions.hpp"
#include "Status.hpp"
#include "CollisionManager.hpp"
#include "Entity.hpp"
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
//Copyright 2024 Property of Olivier Maurin.All rights reserved.

Entity::Entity(void)
{
	particles[(int)ParticleTypes::DAMAGE] = std::make_shared<ParticleSystem>("Contents/Data/Particles/Damage.json");
	particles[(int)ParticleTypes::BLEEDING] = std::make_shared<ParticleSystem>("Contents/Data/Particles/bleeding.json");
	particles[(int)ParticleTypes::BURNING] = std::make_shared<ParticleSystem>("Contents/Data/Particles/burning.json");
	particles[(int)ParticleTypes::POISON] = std::make_shared<ParticleSystem>("Contents/Data/Particles/Poison.json");

	for (auto& particle : particles)
	{
		particle->setPosition(-500.f, -500.f);
		particle->SetStatue(ParticleSystem::Statue::STOPPED);
		DrawManager::Get().AddObject("FIGHT_HUD", *particle);
	}
	particles[(int)ParticleTypes::POISON]->setRotation(180);

	// INIT DEBUG COLLISION 
	collideBox.setFillColor(sf::Color::Transparent);
	collideBox.setOutlineThickness(1.f);
	collideBox.setOutlineColor(sf::Color::Blue);

	feetHitbox.setFillColor(sf::Color::Transparent);
	feetHitbox.setOutlineThickness(1.f);
	feetHitbox.setOutlineColor(sf::Color::Green);

	position.setRadius(5.f);
	position.setOrigin(position.getRadius(), position.getRadius());
	position.setFillColor(sf::Color::Red);

#ifdef _DEBUG
	DrawManager& drawMng = DrawManager::Get();

	drawMng.AddObject("ExplorationUnSortables", collideBox);
	drawMng.SetObjectVisible("ExplorationUnSortables", &collideBox, false);

	drawMng.AddObject("ExplorationUnSortables", feetHitbox);
	drawMng.SetObjectVisible("ExplorationUnSortables", &feetHitbox, false);

	drawMng.AddObject("ExplorationUnSortables", position);
	drawMng.SetObjectVisible("ExplorationUnSortables", &position, false);
#endif // !_DEBUG

	isStun = false;
}

Entity::~Entity(void)
{
	for (auto& particle : particles)
	{
		DrawManager::Get().DeleteObject("FIGHT_HUD", particle.get());
		particle->~ParticleSystem();
		particle.reset();
	}

	while (!textDamages.empty())
	{
		textDamages.back()->~TextDamage();
		textDamages.back().reset();
		textDamages.pop_back();
	}

	while (!applicableStatutes.empty())
	{
		if (applicableStatutes.back().status != nullptr)
		{
			delete applicableStatutes.back().status;
			applicableStatutes.back().status = nullptr;
		}
		applicableStatutes.pop_back();
	}

#ifdef _DEBUG
	DrawManager::Get().DeleteObject("ExplorationUnSortables", &feetHitbox);
	DrawManager::Get().DeleteObject("ExplorationUnSortables", &collideBox);
	DrawManager::Get().DeleteObject("ExplorationUnSortables", &position);
#endif // _DEBUG
}

void Entity::UpdateCollide(void)
{
	collideBox.setPosition(getPosition());
	feetHitbox.setPosition(getPosition());
	position.setPosition(getPosition());

#ifdef _DEBUG
	if (Keyboard::Get().IsKeyPressed(sf::Keyboard::C))
	{
		SetHitBoxDraw(true);
	}
	else if (Keyboard::Get().IsKeyPressed(sf::Keyboard::V))
	{
		SetHitBoxDraw(false);
	}
#endif
}

const std::vector<Entity::ApplicableEffects>& Entity::GetApplicableEffects(void) const
{
	return applicableStatutes;
}

const short int& Entity::GetHP(void) const
{
	return hp;
}

const float Entity::GetPercentHPLeft(void) const
{
	float result = (float)hp / (float)hpMax;
	return result;
}

void Entity::AddHP(const short int& _value)
{
	hp += _value;
	if (hp > hpMax)
	{
		hp = hpMax;
	}
	else if (hp < 0)
	{
		hp = 0;
	}
}

void Entity::SetHP(const short int& _value)
{
	hp = _value;
	if (hp > hpMax)
	{
		hp = hpMax;
	}
}

const unsigned short int& Entity::GetHPMax(void) const
{
	return hpMax;
}

const sf::Color Entity::GetHpColor(void) const
{

	sf::Color enemyColorName;
	float hpPercent = GetPercentHPLeft();
	if (hpPercent >= 0.5) {
		enemyColorName.r = 255;
		enemyColorName.g = 255;
		enemyColorName.b = static_cast<sf::Uint8>(255 * (hpPercent - 0.5f) / 0.5f);
	}
	else {
		enemyColorName.r = 255;
		enemyColorName.g = static_cast<sf::Uint8>(255 * (hpPercent) / 0.5f);
		enemyColorName.b = 0;
	}
	return enemyColorName;

}

void Entity::AddHPMax(const short int& _value)
{
	hpMax += _value;
}

void Entity::SetHPMax(const short int& _value)
{
	hpMax = _value;
}

void Entity::SetAlive(const bool& _alive)
{
	isAlive = _alive;
}

void Entity::SetDamage(const int& _damage)
{
	damage = _damage;
}

void Entity::SetSpeed(const float& _speed)
{
	speed = _speed;
}

void Entity::AddDamageText(std::shared_ptr<TextDamage>&& _toAdd)
{
	textDamages.push_back(_toAdd);
}

void Entity::SetHitBoxDraw(const bool& _isDraw)
{
#ifdef _DEBUG
	DrawManager::Get().SetObjectVisible("ExplorationUnSortables", &feetHitbox, _isDraw);
	DrawManager::Get().SetObjectVisible("ExplorationUnSortables", &collideBox, _isDraw);
	DrawManager::Get().SetObjectVisible("ExplorationUnSortables", &position, _isDraw);
#endif // _DEBUG
}

void Entity::AddDamage(const int& _damage)
{
	damage += _damage;
}

void Entity::AddStatus(Status* _status)
{
	this->statutes.push_back(_status);
}

const std::vector<Status*>& Entity::GetStatutes()
{
	return this->statutes;
}

bool Entity::DoesItHaveStatus(const Effects& _effect)
{
	std::string tempName = "";
	switch (_effect)
	{
	case BURNING:
		tempName = "burning";
		break;
	case BLEEDING:
		tempName = "bleeding";
		break;
	case POISON:
		tempName = "poison";
		break;
	case ATK_BUFF:
		tempName = "attackBuff";
		break;
	case ATK_BREAK:
		tempName = "breakAttack";
		break;
	case DEF_BUFF:
		tempName = "defBuff";
		break;
	case DEF_BREAK:
		tempName = "breakDef";
		break;
	case IMMUNITY:
		tempName = "immunity";
		break;
	case ANTI_BUFF:
		tempName = "antiBuff";
		break;
	case STUN:
		tempName = "Stun";
		break;
	default:
		break;
	}

	for (int i = 0; i < this->statutes.size(); i++)
	{
		if (this->statutes[i]->effectName == tempName)
		{
			return true;
		}
	}
	return false;
}

bool Entity::DoesItImmuned(const int& _id)
{
	return immunities[_id];
}

void Entity::ApplyEffect(std::shared_ptr<Entity> _target)
{
	bool applicable = true;
	for (int i = 0; i < this->applicableStatutes.size(); i++)
	{
		applicable = true;
		// Check if _target is already affected
		for (const auto& it : _target->GetStatutes())
		{
			if (it->effectName == this->applicableStatutes[i].status->effectName)
			{
				if (it->stackable && rand() % 100 <= this->applicableStatutes[i].probability * 100 && !_target->immunities[this->applicableStatutes[i].status->id])
				{
					it->AddNewStack();
					applicable = false;
				}
				else if (!it->stackable && rand() % 100 <= this->applicableStatutes[i].probability * 100 && !_target->immunities[this->applicableStatutes[i].status->id])
				{
					it->ResetCooldown();
					applicable = false;
				}
				else
				{
					applicable = false;
				}
			}
		}
		if (applicable && rand() % 100 <= this->applicableStatutes[i].probability * 100 && !_target->immunities[this->applicableStatutes[i].status->id])
		{
			Status* effect = this->applicableStatutes[i].status->Clone();

			//if (effect->effectName == "Stun")
			//{
			//	effect->Init(this, _target, 1);
			//}
			//else
			//{

			effect->Init(this, _target);

			//}
			_target->AddStatus(effect);
		}
	}
}

void Entity::ApplyParticularEffect(std::shared_ptr<Entity> _target, const Effects& _effect, const int& _nbTurns)
{
	Status* tempStatus = nullptr;
	switch (_effect)
	{
	case BURNING:
		tempStatus = new Burning();
		break;
	case BLEEDING:
		tempStatus = new Bleeding();
		break;
	case POISON:
		tempStatus = new Poison();
		break;
	case ATK_BUFF:
		tempStatus = new AttackBuff();
		break;
	case ATK_BREAK:
		tempStatus = new BreakAttack();
		break;
	case DEF_BUFF:
		tempStatus = new DefBuff();
		break;
	case DEF_BREAK:
		tempStatus = new BreakDef();
		break;
	case IMMUNITY:
		tempStatus = new Immunity();
		break;
	case ANTI_BUFF:
		tempStatus = new AntiBuff();
		break;
	case STUN:
		tempStatus = new Stun();
		break;
	default:
		break;
	}
	if (tempStatus != nullptr && !_target->immunities[_effect])
	{
		if (!DoesItHaveStatus(_effect))
		{
			tempStatus->Init(this, _target, _nbTurns);
			_target->AddStatus(tempStatus);
		}
		else
		{
			for (const auto& it : _target->GetStatutes())
			{
				if (it->effectName == tempStatus->effectName)
				{
					if (it->stackable)
					{
						it->AddNewStack();
					}
					else
					{
						it->ResetCooldown();
					}
				}
			}
		}

	}
}

void Entity::SetImmunity(const bool& _bool)
{
	//needs to be modified if new buffs are added
	if (_bool)
	{
		for (unsigned int i = 0; i < NB_EFFECTS; i++)
		{
			if (i != ATK_BUFF && i != DEF_BUFF && i != IMMUNITY)
			{
				immunities[i] = true;
			}
		}
	}
	else
	{
		immunities = initialImmunities;
	}
}

void Entity::SetAntiBuff(const bool& _bool)
{
	//needs to be modified if new buffs are added
	if (_bool)
	{
		for (unsigned int i = 0; i < NB_EFFECTS; i++)
		{
			if (i == ATK_BUFF || i == DEF_BUFF || i == IMMUNITY)
			{
				immunities[i] = true;
			}
		}
	}
	else
	{
		immunities = initialImmunities;
	}
}

const sf::Vector2f Entity::GetLastPos(void) const
{
	return lastPos;
}

const bool& Entity::IsStun(void) const
{
	return this->isStun;
}

void Entity::SetStun(const bool& _stun)
{
	this->isStun = _stun;
}

void Entity::SetShadowOffset(sf::Vector2f& _offset)
{
	shadowOffset = _offset;
}

void Entity::SetShadowOffset(const float& _offsetX, const float& _offsetY)
{
	shadowOffset.x = _offsetX;
	shadowOffset.y = _offsetY;
}

void Entity::SetShadowRadiusFactor(const float& _factor)
{
	shadowRadiusFactor = _factor;
}

void Entity::RunStatutes()
{
	for (int i = (int)statutes.size() - 1; i >= 0; i--)
	{
		statutes[i]->Run();
	}
}

void Entity::ClearStatutes()
{
	while (!this->statutes.empty())
	{
		this->statutes.back()->Stop();
		delete statutes.back();
		this->statutes.back() = nullptr;
		this->statutes.pop_back();
	}
}

void Entity::ClearBadStatutes()
{
	for (int i = static_cast<int>(statutes.size()) - 1; i >= 0; i--)
	{
		if (statutes[i]->type != Status::BUFF)
		{
			statutes[i]->Stop();
			delete statutes[i];
			statutes[i] = nullptr;
			statutes.erase(statutes.begin() + i);
		}
	}
}

void Entity::DeleteStatus(const std::string& _name)
{
	for (int i = 0; i < this->statutes.size(); i++)
	{
		if (this->statutes[i]->effectName == _name)
		{
			delete this->statutes[i];
			this->statutes[i] = nullptr;
			this->statutes.erase(this->statutes.begin() + i);
			return;
		}
	}

	Debug.error("Impossible to delete", _name, " no status corresponds");
}
const bool& Entity::IsAlive(void) const
{
	return isAlive;
}

const float& Entity::GetSpeed(void) const
{
	return speed;
}

sf::Sprite& Entity::GetSprite(void)
{
	return curAnim->GetSprite();
}

sf::FloatRect Entity::GetGlobalAABB(void) const
{
	return collideBox.getGlobalBounds();
}

sf::FloatRect Entity::GetLocalAABB(void) const
{
	return collideBox.getLocalBounds();
}

const sf::FloatRect Entity::GetFeetCollisionBox(void) const
{
	const sf::FloatRect entityRect = GetGlobalAABB();

	return sf::FloatRect(entityRect.left, entityRect.top + entityRect.height - FOOT_HEIGHT, entityRect.width, FOOT_HEIGHT);
}

const float Entity::GetBoxRight(void) const
{
	return getPosition().x + GetFeetCollisionBox().width / 2.f;
}

const float Entity::GetBoxLeft(void) const
{
	return getPosition().x - GetFeetCollisionBox().width / 2.f;
}

std::deque<std::shared_ptr<TextDamage>>& Entity::GetTextDamages()
{
	return textDamages;
}

const int& Entity::GetDamage(void)
{
	return damage;
}

const int& Entity::GetInitialDamage(void) const
{
	return initialDamage;
}

const sf::Vector2u Entity::GetShadowSize(void) const
{
	return sf::Vector2u((unsigned int)shadow.getGlobalBounds().width, (unsigned int)shadow.getGlobalBounds().height);
}

const std::array<float, NB_ATTACK_TYPES>& Entity::GetResistances(void) const
{
	return resistances;
}

const std::array<bool, Entity::NB_EFFECTS>& Entity::GetImmunities(void) const
{
	return this->immunities;
}

void Entity::AddResistanceCoef(const float& _coef)
{
	for (auto& resistance : resistances)
	{
		resistance += _coef;
	}
}

std::array<std::shared_ptr<ParticleSystem>, (int)Entity::ParticleTypes::NB_PARTICLES>& Entity::GetParticles(void)
{
	return particles;
}

const sf::Vector2f& Entity::GetShadowOffset(void) const
{
	return shadowOffset;
}

const float& Entity::GetShadowRadiusFactor(void) const
{
	return shadowRadiusFactor;
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(shadow, states);
	target.draw(*curAnim, states);
}