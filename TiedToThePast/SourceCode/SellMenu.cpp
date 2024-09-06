#include "Definitions.hpp"
#include "SellMenu.hpp"

void SellMenu::Init(GameData& _gameData, std::shared_ptr<Player> _player)
{
	this->gameData = &_gameData;
	this->player = _player;

	this->items.clear();
	this->background.setSize(SIZE_BACKGROUND);
	this->background.setTexture(&texMng["INVENTORY_SUITCASE"]);

	this->scrollValue = 0;
	this->scrollMenu.SetSize({ WORKING_WIDTH * WORKING_WIDTH / Screen::Get().GetWidth(), WORKING_HEIGHT * WORKING_HEIGHT / Screen::Get().GetHeight() });
	this->scrollMenu.Clear();
	for (int i = 0; i < _player->GetItems().size(); i++)
	{
		const Item* curItem = _player->GetItems()[i].get();
		if (curItem->GetType() == Item::CONSUMABLE)
		{
			this->items.push_back(curItem);
		}
	}

	for (int i = 0; i < this->items.size(); i++)
	{
		Button* button = new Button();

		sf::Vector2f size = (sf::Vector2f)this->items[i]->GetTexture()->getSize();
		button->SetColor(sf::Color::White);
		button->SetTextureSprite(*this->items[i]->GetTexture(), true);
		button->SetOnOverFct(std::bind(&Button::SetColor, button, sf::Color(255, 255, 255, 200)));
		button->SetOnOutFct(std::bind(&Button::SetColor, button, sf::Color::White));
		button->SetOnClickFct(std::bind(&SellMenu::SelectItem, this, i));

		button->setOrigin(-size / 2.f);
		sf::Vector2f pos;
		pos.x = START_ORIGIN.x + (SIZE_ONE_BOX.x + SPACE_BETWEEN_BOXS) * (i % 4);
		pos.y = START_ORIGIN.y + (SIZE_ONE_BOX.y + SPACE_BETWEEN_BOXS) * float(std::floor(i / 4));
		
		pos.x *= Screen::Get().GetWidth() / WORKING_WIDTH;
		pos.y *= Screen::Get().GetHeight() / WORKING_HEIGHT;
		button->setPosition(pos);

		this->scrollMenu.AddItem(std::move(button));
	}
}

void SellMenu::Update(GameData& _gameData)
{
	this->scrollMenu.setPosition(this->getPosition().x * Screen::Get().GetWidth() / WORKING_WIDTH, this->getPosition().y * Screen::Get().GetHeight() / WORKING_HEIGHT);
	this->scrollMenu.Update(_gameData);
}

bool SellMenu::PollEvent(GameData& _gameData)
{
	if (!this->confirmMenu.IsFinished())
	{
		this->confirmMenu.PollEvent(_gameData);
		if (this->confirmMenu.IsFinished())
		{
			if (this->confirmMenu.IsConfirmed())
			{
				this->SellSelected();
			}
			return true;
		}
	}

	if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "INVENTORY"))
	{
		this->confirmMenu.Close();
	}

	this->scrollMenu.PollEvent(_gameData);
	return false;
}

const sf::FloatRect SellMenu::GetGlobalBounds()
{
	sf::FloatRect result;
	sf::Vector2f position = getPosition();
	sf::Vector2f size = background.getSize();

	result.width = size.x;
	result.height = size.y;
	result.left = position.x;
	result.top = position.y;

	return result;
}

void SellMenu::SelectItem(int _index)
{
	this->selectedItem = items[_index];
	this->confirmMenu.SetText("Sell " + this->selectedItem->GetName() + " for " + std::to_string(this->selectedItem->GetPrice() / 2) + " ?");
	this->confirmMenu.Reset();
	Audio::Get().PlaySoundNoSecurity("SHOP_SELECT");
}

void SellMenu::SellSelected()
{
	if (this->selectedItem)
	{
		this->player->AddSweets(this->selectedItem->GetPrice() / 2);
		this->player->DecreaseItemStack(this->selectedItem->GetId());
		this->Init(*this->gameData, this->player);
		Audio::Get().PlaySoundNoSecurity("ITEM_SELL");
	}
}

void SellMenu::SetColor(const sf::Color& _color)
{
	this->background.setFillColor(_color);
}

void SellMenu::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
{
	_states.transform *= getTransform();
	_target.draw(this->background, _states);
	_target.draw(this->scrollMenu);

	if (!this->confirmMenu.IsFinished())
	{
		_target.draw(this->confirmMenu);
	}
}