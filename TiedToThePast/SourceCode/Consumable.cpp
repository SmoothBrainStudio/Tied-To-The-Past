#include "Definitions.hpp"
#include "Consumable.hpp"
#include "Player.hpp"
#include "AttackBuff.hpp"
#include "Burning.hpp"
#include "Bleeding.hpp"
#include "Poison.hpp"

Consumable::Consumable()
{
	this->type = CONSUMABLE;
}

Consumable::Consumable(const std::string& _id, sf::Uint16 _count) : Item(_id), count(_count)
{
	this->effect = ((Consumable*)prefabs[_id].get())->GetEffect();
	this->countText = CreateText(FontsManager::Get()["SPOOKY"], "x" + std::to_string(_count), 30);
	this->countText.setOutlineThickness(2.f);
	this->countText.setOutlineColor(sf::Color::Black);
	this->SetTexture(*this->sprite.getTexture());
}

void Consumable::ApplyEffect(std::shared_ptr<Entity>& _entity)
{
	this->SetCount(this->count - 1);
	switch (this->effect)
	{
	case Effect::HEAL:
		_entity->AddHP(10000);
		Audio::Get().PlaySound("USE_POTION");
		break;

	case Effect::CURE:
		_entity->ClearBadStatutes();
		Audio::Get().PlaySound("USE_ANTIDOTE");
		break;
	case Effect::BOMB:
		_entity->AddHP(-20);
		this->bombDamage = std::make_shared<TextDamage>("-20", sf::Vector2f{ _entity->getPosition().x, _entity->getPosition().y - _entity->GetGlobalAABB().height });
		_entity->textDamages.push_back(this->bombDamage);
		Audio::Get().PlaySound("USE_BOMB");
		break;
	case Effect::ATK_BUFF:
		this->status = new AttackBuff();
		this->status->Init(nullptr, _entity, 3);
		if (_entity->DoesItHaveStatus(Entity::Effects::ATK_BUFF))
		{
			for (auto& status : _entity->GetStatutes())
			{
				if (status->id == Entity::Effects::ATK_BUFF)
				{
					status->AddTurns(3);
				}
			}
		}
		else
		{
			_entity->AddStatus(this->status);
		}
		Audio::Get().PlaySound("USE_POWER_POTION");
		break;
	default:
		Debug.warning(this->GetName(), " doesn't have effect");
		break;
	}
}

void Consumable::ApplyEffectAoE(std::vector<std::shared_ptr<Entity>> _entities)
{
	this->SetCount(this->count - 1);
	switch (this->effect)
	{
	case Effect::HEAL:
		for (int i = 0; i < _entities.size(); i++)
		{
			_entities[i]->AddHP(10000);
		}
		Audio::Get().PlaySound("USE_POTION");
		break;

	case Effect::CURE:
		for (int i = 0; i < _entities.size(); i++)
		{
			_entities[i]->ClearBadStatutes();
		}
		
		Audio::Get().PlaySound("USE_ANTIDOTE");
		break;
	case Effect::BOMB:
		for (int i = 0; i < _entities.size(); i++)
		{
			_entities[i]->AddHP(-20);
			this->bombDamage = std::make_shared<TextDamage>("-20",sf::Vector2f{ _entities[i]->getPosition().x, _entities[i]->getPosition().y - _entities[i]->GetGlobalAABB().height});
			_entities[i]->textDamages.push_back(this->bombDamage);
		}
		Audio::Get().PlaySound("USE_BOMB");
		break;
	case Effect::ATK_BUFF:
		for (int i = 0; i < _entities.size(); i++)
		{
			this->status = new AttackBuff();
			this->status->Init(nullptr, _entities[i], 5);
			_entities[i]->AddStatus(this->status);
		}
		Audio::Get().PlaySound("USE_POWER_POTION");
		break;
	default:
		Debug.warning(this->GetName(), " doesn't have effect");
		break;
	}
}

void Consumable::Increase()
{
	this->SetCount(this->count + 1);
}

void Consumable::Decrease()
{
	this->SetCount(this->count - 1);
}

void Consumable::SetTexture(const sf::Texture& _texture)
{
	this->sprite.setTexture(_texture, true);
	this->CreateRenderTexture();
}

void Consumable::SetEffect(const std::string& _effect)
{
	if (_effect == "HEAL") this->effect = HEAL;
	else if (_effect == "CURE") this->effect = CURE;
	else if (_effect == "ATK_BUFF") this->effect = ATK_BUFF;
	else if (_effect == "BOMB") this->effect = BOMB;
}

void Consumable::SetEffect(Effect _effect)
{
	this->effect = _effect;
}

void Consumable::SetCount(sf::Uint16 _count)
{
	this->count = _count;
	this->countText.setString("x" + std::to_string(this->count));
	this->UpdateRenderTexture();
}

void Consumable::SetColor(const sf::Color& _color)
{
	this->stackSprite.setColor(_color);
}

const sf::Texture* Consumable::GetTexture() const
{
	return this->stackSprite.getTexture();
}

sf::FloatRect Consumable::GetRect() const
{
	return this->stackSprite.getGlobalBounds();
}

const sf::Texture& Consumable::GetBigTexture() const
{
	return TexturesManager::Get()["BIG_" + this->id];
}

void Consumable::CreateRenderTexture(sf::RenderStates _states)
{
	sf::Vector2u textureSize = this->sprite.getTexture()->getSize();
	sf::Vector2f scaleSize = Screen::Get().Scale((float)textureSize.x, (float)textureSize.y);
	this->countRenderTexture.create((unsigned int)scaleSize.x, (unsigned int)scaleSize.y);
	this->UpdateRenderTexture(_states);
}

Consumable::Effect Consumable::GetEffect() const
{
	return this->effect;
}

sf::Uint16 Consumable::GetCount() const
{
	return this->count;
}

void Consumable::Save(std::ostream& _file)
{
	Item::Save(_file);
	_file << ' ' << this->count;
}

void Consumable::Load(std::istream& _file)
{
	_file >> this->count;
}

void Consumable::UpdateRenderTexture(sf::RenderStates _states)
{
	sf::FloatRect localBounds = this->countText.getLocalBounds();
	this->countText.setOrigin(localBounds.left + localBounds.width, localBounds.top + localBounds.height);
	sf::FloatRect spriteRect = this->sprite.getGlobalBounds();
	this->countText.setPosition(spriteRect.width - 2.f, spriteRect.height - 2.f);

	sf::RenderStates states;
	states.transform.scale(Screen::Get().GetScaleFactor(), Screen::Get().GetScaleFactor());
	this->countRenderTexture.clear(sf::Color::Transparent);
	this->countRenderTexture.draw(this->sprite, states);
	this->countRenderTexture.draw(this->countText);
	this->countRenderTexture.display();
	this->stackSprite.setTexture(this->countRenderTexture.getTexture(), true);
}

void Consumable::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
{
	_target.draw(this->stackSprite, _states);
}