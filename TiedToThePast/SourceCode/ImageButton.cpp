#include "ImageButton.hpp"

ImageButton::ImageButton()
{
	SetOnClickFct([] {});
	SetOnOverFct([] {});
	SetOnOutFct([] {});
}

void ImageButton::SetColor(const sf::Color& _color)
{
	this->color = _color;
	if (!this->isHovered)
	{
		this->sprite.setColor(this->color);
	}
}

void ImageButton::SetHoveredColor(const sf::Color& _color)
{
	this->hoveredColor = _color;
	if (this->isHovered)
	{
		this->sprite.setColor(this->hoveredColor);
	}
}

bool ImageButton::IsHovered() const
{
	return this->isHovered;
}

void ImageButton::SetTexture(const sf::Texture& _texture)
{
	this->sprite.setTexture(_texture);
}

void ImageButton::PollEvent(GameData& _gameData)
{
	if (ControlsManager::Get().GetControllerType() == KEYBOARD)
	{
		if (_gameData.gameEvent.type == sf::Event::MouseMoved)
		{
			sf::FloatRect rect = this->getTransform().transformRect(this->sprite.getGlobalBounds());
			bool isColliding = rect.contains(Screen::Get().GetMousePosition(_gameData.window));

			if (isColliding && !wasHovered)
			{
				_gameData.window.setMouseCursor(_gameData.cursor);
				wasHovered = true;
				this->CallOnOver();
			}
			else if (!isColliding && wasHovered)
			{
				_gameData.window.setMouseCursor(_gameData.cursorGrey);
				wasHovered = false;
				this->CallOnOut();
			}
		}
		else if (_gameData.gameEvent.type == sf::Event::MouseButtonPressed && _gameData.gameEvent.mouseButton.button == sf::Mouse::Left)
		{
			sf::FloatRect rect = this->getTransform().transformRect(this->sprite.getGlobalBounds());
			if (rect.contains(Screen::Get().GetMousePosition(_gameData.window)))
			{
				this->CallOnClick();
			}
		}
	}

	if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "INTERACT")
		|| ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "INTERACT_2"))
	{
		this->CallOnClick();
	}
}

sf::FloatRect ImageButton::GetRect() const
{
	return { this->getPosition() - this->getOrigin(), { this->sprite.getGlobalBounds().width, this->sprite.getGlobalBounds().height }};
}

void ImageButton::CallOnClick(bool _isClick)
{
	if (this->isHovered)
	{
		Eventable::CallOnClick(_isClick);
	}
}

void ImageButton::CallOnOver(bool _isOver)
{
	this->sprite.setColor(this->hoveredColor);
	this->isHovered = true;
	Eventable::CallOnOver(_isOver);
}

void ImageButton::CallOnOut(bool _isOut)
{
	this->sprite.setColor(this->color);
	this->isHovered = false;
	Eventable::CallOnOut(_isOut);
}

void ImageButton::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
{
	_states.transform *= this->getTransform();
	_target.draw(this->sprite, _states);
}