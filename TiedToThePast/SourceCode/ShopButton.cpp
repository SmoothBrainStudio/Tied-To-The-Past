#include "ShopButton.hpp"

ShopButton::ShopButton(Player& _player, const sf::Vector2f& _position, std::shared_ptr<Item> _item) : player(_player), item(_item)
{
	this->SetOnClickFct(std::bind(&ShopButton::Buy, this));
	this->SetOnOverFct(std::bind(&ShopButton::SetSelected, this, true));
	this->SetOnOutFct(std::bind(&ShopButton::SetSelected, this, false));

	this->setPosition(_position);
	this->sprite.setTexture(((Consumable*)this->item.get())->GetBigTexture(), true);
	//this->sprite.setScale(SCALE, SCALE);
}

void ShopButton::SetSelected(bool _flag)
{
	if (_flag)
	{
		this->sprite.setColor(sf::Color(255, 255, 255, 200));
	}
	else
	{
		this->sprite.setColor(sf::Color::White);
	}
}

void ShopButton::Buy()
{
	if (this->player.GetPossessedSweets() >= this->item->GetPrice() && !this->isSold)
	{
		this->player.AddItem(*this->item, false, false);
		Audio::Get().PlaySoundNoSecurity("ITEM_PURCHASE");
		if (this->item->GetType() == Item::CONSUMABLE)
		{
			Consumable* consumable = (Consumable*)this->item.get();
			consumable->SetCount(consumable->GetCount() - 1);
			if (consumable->GetCount() == 0)
			{
				this->isSold = true;
			}
		}
		else
		{
			this->isSold = true;
		}
	}
	else
	{
		Audio::Get().PlaySoundNoSecurity("CANNOT_PURCHASE");
	}
}

void ShopButton::SetCount(sf::Uint16 _count)
{
	if (_count)
	{
		this->isSold = false;
		if (this->item->GetType() == Item::CONSUMABLE)
		{
			((Consumable*)this->item.get())->SetCount(_count);
		}
	}
	else
	{
		this->isSold = true;
	}
}

sf::Uint16 ShopButton::GetCount() const
{
	if (this->isSold)
	{
		return 0;
	}
	else if (this->item->GetType() == Item::CONSUMABLE)
	{
		return ((Consumable*)this->item.get())->GetCount();
	}
	return 1;
}

bool ShopButton::IsEmpty() const
{
	return this->isSold;
}

const Item* ShopButton::GetItem() const
{
	return this->item.get();
}

void ShopButton::UpdateRenderTexture()
{
	if (this->item->GetType() == Item::CONSUMABLE)
	{
		((Consumable*)this->item.get())->CreateRenderTexture(this->getTransform());
		this->sprite.setTexture(((Consumable*)this->item.get())->GetBigTexture(), true);
	}
}

void ShopButton::PollEvent(GameData& _gameData)
{
	
}

sf::FloatRect ShopButton::GetRect() const
{
	return { this->getPosition(), { this->sprite.getGlobalBounds().width, this->sprite.getGlobalBounds().height }};
}

void ShopButton::SetShopIndex(sf::Uint32 _index)
{
	this->shopIndex = _index;
}

sf::Uint32 ShopButton::GetShopIndex() const
{
	return this->shopIndex;
}

void ShopButton::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
{
	_states.transform *= this->getTransform();
	_target.draw(this->sprite, _states);
}