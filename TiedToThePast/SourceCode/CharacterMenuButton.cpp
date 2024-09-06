#include "CharacterMenuButton.hpp"
#include "Consumable.hpp"

CharacterMenuButton::CharacterMenuButton()
{
	this->SetOnClickFct([] {});
	this->SetOnOverFct([] {});
	this->SetOnOutFct([] {});
}

CharacterMenuButton::CharacterMenuButton(std::shared_ptr<Item> _item, const sf::Vector2f& _position)
{
	this->SetOnClickFct([] {});
	this->SetOnOverFct([] {});
	this->SetOnOutFct([] {});

	sf::FloatRect rect = _item->GetRect();
	this->background.setTexture(TexturesManager::Get()["INVENTORY_ITEM_ICON"]);
	this->setPosition(_position);
	this->item = _item;
	if (_item->GetType() == Item::CONSUMABLE)
	{
		this->sprite.setTexture(TexturesManager::Get()["BIG_" + _item->GetId()]);
	}
	else
	{
		this->sprite.setTexture(*_item->GetTexture());
	}
	this->sprite.setOrigin(this->sprite.getGlobalBounds().width * 0.5f, this->sprite.getGlobalBounds().height * 0.5f);
	this->sprite.setPosition(WIDTH * 0.5f, HEIGHT * 0.5f);
}

void CharacterMenuButton::SetSelected(bool _flag)
{
	if (_flag)
	{
		this->background.setColor(sf::Color(255, 255, 255, 150));
	}
	else
	{
		this->background.setColor(sf::Color::White);
	}
}

std::shared_ptr<Item> CharacterMenuButton::GetItem() const
{
	return this->item;
}

void CharacterMenuButton::PollEvent(GameData& _gameData)
{
	
}

sf::FloatRect CharacterMenuButton::GetRect() const
{
	return { this->getPosition(), { WIDTH, HEIGHT } };
}

void CharacterMenuButton::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
{
	_states.transform *= this->getTransform();
	_target.draw(this->background, _states);
	_target.draw(this->sprite, _states);
}