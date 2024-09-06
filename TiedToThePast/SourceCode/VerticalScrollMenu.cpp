#include "Definitions.hpp"
#include "VerticalScrollMenu.hpp"

VerticalScrollMenu::VerticalScrollMenu()
{
	this->bar.setFillColor(sf::Color(127, 127, 127));
	this->bar.setPosition(this->renderTexture.getSize().x - BAR_WIDTH, 0.f);
}

VerticalScrollMenu::~VerticalScrollMenu()
{
	Clear();
}

void VerticalScrollMenu::SetLooping(bool _isLooping)
{
	this->isLooping = _isLooping;
}

void VerticalScrollMenu::AddItem(MenuItem* _item)
{
	this->items.push_back(std::move(_item));

	sf::FloatRect rect = items.back()->GetRect();
	this->maxBottom = fmaxf(this->maxBottom, rect.top + rect.height);

	float renderTextureHeight = (float)this->renderTexture.getSize().y;
	if (this->maxBottom > renderTextureHeight)
	{
		this->bar.setSize({ BAR_WIDTH, renderTextureHeight / this->maxBottom * renderTextureHeight });
	}

	if (this->items.size() == 1)
	{
		this->items[0]->CallOnOver();
	}
}

void VerticalScrollMenu::Clear()
{
	while (!items.empty())
	{
		if (items.back() != nullptr)
		{
			delete items.back();
			items.back() = nullptr;
			items.pop_back();
		}
	}
	
	this->items.clear();
	this->maxBottom = 0.f;
	this->bar.setSize({ 0.f, 0.f });
	this->bar.setPosition(this->renderTexture.getSize().x - BAR_WIDTH, 0.f);
	this->scrollOffset = 0.f;
	this->SetSelectedItemIndex(0);
}

std::vector<MenuItem*>& VerticalScrollMenu::GetItems()
{
	return this->items;
}

size_t VerticalScrollMenu::GetNbItems() const
{
	return this->items.size();
}

void VerticalScrollMenu::SetSelectedItemIndex(size_t _index)
{
	if (this->items.empty())
	{
		this->selectedItemIndex = 0;
		return;
	}

	this->items[this->selectedItemIndex]->CallOnOut();
	if (_index >= this->items.size())
	{
		if (_index < 1000000)
		{
			this->selectedItemIndex = 0;
		}
		else
		{
			this->selectedItemIndex = this->items.size() - 1;
		}
	}
	else
	{
		this->selectedItemIndex = _index;
	}
	this->items[this->selectedItemIndex]->CallOnOver();

	sf::FloatRect itemRect = this->items[this->selectedItemIndex]->GetRect();
	float itemBottom = itemRect.top + itemRect.height;
	float textureHeight = (float)this->renderTexture.getSize().y;
	if (itemRect.top < this->scrollOffset)
	{
		this->scrollOffset = itemRect.top;
	}
	else if (itemBottom > this->scrollOffset + textureHeight)
	{
		this->scrollOffset = itemBottom - textureHeight;
	}
	sf::Vector2f renderTextureSize(this->renderTexture.getSize());
	this->bar.setPosition(renderTextureSize.x - BAR_WIDTH, this->scrollOffset / (this->maxBottom - renderTextureSize.y) * (renderTextureSize.y - this->bar.getGlobalBounds().height));
}

void VerticalScrollMenu::SelectLeft()
{
	if (this->selectedItemIndex == 0)
	{
		if (this->isLooping)
		{
			this->SetSelectedItemIndex(this->selectedItemIndex - 1);
		}
	}
	else
	{
		this->SetSelectedItemIndex(this->selectedItemIndex - 1);
	}
}

void VerticalScrollMenu::SelectRight()
{
	if (this->selectedItemIndex == this->items.size() - 1)
	{
		if (this->isLooping)
		{
			this->SetSelectedItemIndex(this->selectedItemIndex + 1);
		}
	}
	else
	{
		this->SetSelectedItemIndex(this->selectedItemIndex + 1);
	}
}

void VerticalScrollMenu::SelectUp()
{
	if (this->verticalMode)
	{
		if (!this->items.size())
			return;

		size_t index = this->selectedItemIndex;
		float lastY = this->items[index]->getPosition().y;
		float lastX = this->items[index]->getPosition().x;
		do
		{
			--index;
			if (index == size_t(-1))
			{
				if (this->isLooping)
				{
					index = this->items.size() - 1;
				}
				else
				{
					index = 0;
				}
				break;
			}
		} while (this->items[index]->getPosition().y >= lastY || this->items[index]->getPosition().x != lastX);
		this->SetSelectedItemIndex(index);
	}
}

void VerticalScrollMenu::SelectDown()
{
	if (this->verticalMode)
	{
		if (!this->items.size())
			return;

		size_t index = this->selectedItemIndex;
		float lastY = this->items[index]->getPosition().y;
		float lastX = this->items[index]->getPosition().x;
		do
		{
			++index;
			if (index == this->items.size())
			{
				if (this->isLooping)
				{
					index = 0;
				}
				else
				{
					index = this->items.size() - 1;
				}
				break;
			}
		} while (this->items[index]->getPosition().y <= lastY || this->items[index]->getPosition().x != lastX);
		this->SetSelectedItemIndex(index);
	}
}

void VerticalScrollMenu::Update(GameData& _gameData)
{
	ControlsManager& controlsMng = ControlsManager::Get();
	if (controlsMng.GetControllerType() == CONTROLLER)
	{
		if (controlsMng.IsPressedDirection("UP"))
		{
			this->SelectUp();
		}
		else if (controlsMng.IsPressedDirection("DOWN"))
		{
			this->SelectDown();
		}
		else if (controlsMng.IsPressedDirection("LEFT"))
		{
			this->SelectLeft();
		}
		else if (controlsMng.IsPressedDirection("RIGHT"))
		{
			this->SelectRight();
		}
	}

	for (MenuItem* item : this->items)
	{
		item->Update(_gameData);
	}
}

void VerticalScrollMenu::PollEvent(GameData& _gameData)
{
	if (_gameData.gameEvent.type == sf::Event::KeyReleased
		|| _gameData.gameEvent.type == sf::Event::TextEntered)
	{
		return;
	}

	ControlsManager& controlsMng = ControlsManager::Get();
	if (controlsMng.IsPressedInput(_gameData.gameEvent, "INTERACT")
		|| controlsMng.IsPressedInput(_gameData.gameEvent, "INTERACT_2"))
	{
		this->CallSelectedItem();
	}
	else if (controlsMng.GetControllerType() == KEYBOARD)
	{
		if (_gameData.gameEvent.type == sf::Event::MouseMoved)
		{
			this->SelectItemAtMousePos(_gameData);
		}
		else if (_gameData.gameEvent.type == sf::Event::MouseButtonPressed)
		{
			if (_gameData.gameEvent.mouseButton.button == sf::Mouse::Left)
			{
				if (this->SelectItemAtMousePos(_gameData))
				{
					this->CallSelectedItem();
				}
			}
		}
		else if (_gameData.gameEvent.type == sf::Event::MouseWheelScrolled)
		{
			if (_gameData.gameEvent.mouseWheelScroll.delta > 0)
			{
				this->SelectLeft();
			}
			else
			{
				this->SelectRight();
			}
		}
		else if (controlsMng.IsPressedInput(_gameData.gameEvent, "UP"))
		{
			this->SelectUp();
		}
		else if (controlsMng.IsPressedInput(_gameData.gameEvent, "DOWN"))
		{
			this->SelectDown();
		}
		else if (controlsMng.IsPressedInput(_gameData.gameEvent, "LEFT"))
		{
			this->SelectLeft();
		}
		else if (controlsMng.IsPressedInput(_gameData.gameEvent, "RIGHT"))
		{
			this->SelectRight();
		}
	}
}

void VerticalScrollMenu::SetSize(const sf::Vector2f& _size)
{
	this->renderTexture.create((unsigned int)Screen::Get().PosX(_size.x), (unsigned int)Screen::Get().PosY(_size.y));
}

sf::FloatRect VerticalScrollMenu::GetRect() const
{
	sf::Vector2f textureSize = (sf::Vector2f)this->renderTexture.getSize();
	sf::Vector2f size(textureSize.x, textureSize.y);
	return { this->getPosition() - this->getOrigin(), size };
}

MenuItem* VerticalScrollMenu::operator[](size_t _index)
{
	return this->items[_index];
}

void VerticalScrollMenu::SetVerticalMode(bool _flag)
{
	this->verticalMode = _flag;
}

bool VerticalScrollMenu::GetVerticalMode() const
{
	return this->verticalMode;
}

void VerticalScrollMenu::CallSelectedItem()
{
	if (!this->items.empty())
	{
		this->items[this->selectedItemIndex]->CallOnClick();
	}
}

bool VerticalScrollMenu::SelectItemAtMousePos(GameData& _gameData)
{
	sf::Vector2f mousePos = Screen::Get().GetMousePosition(_gameData.window);

	if (sf::FloatRect(this->getPosition() - this->getOrigin(), (sf::Vector2f)this->renderTexture.getSize()).contains(mousePos))
	{
		sf::Vector2f transformedPos = this->getInverseTransform().transformPoint(Screen::Get().GetMousePosition(_gameData.window));
		transformedPos.y += this->scrollOffset;
		for (size_t i = 0; i < this->items.size(); ++i)
		{
			if (this->items[i]->GetRect().contains(transformedPos))
			{
				_gameData.window.setMouseCursor(_gameData.cursor);
				this->SetSelectedItemIndex(i);
				return true;
			}
		}
	}
	_gameData.window.setMouseCursor(_gameData.cursorGrey);
	return false;
}

void VerticalScrollMenu::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
{
	_states.transform *= this->getTransform();
	this->renderTexture.clear(sf::Color::Transparent);

	sf::Transform itemTransform;
	itemTransform.translate(0.f, -this->scrollOffset);
	for (MenuItem* item : this->items)
	{
		this->renderTexture.draw(*item, itemTransform);
	}
	this->renderTexture.draw(this->bar);

	this->renderTexture.display();
	this->sprite.setTexture(this->renderTexture.getTexture());
	_target.draw(this->sprite, _states);
}