#include "Definitions.hpp"
#include "Bar.hpp"

Bar::Bar() : value(0), min(0), max(100)
{
	className = "bar";
	InitText("");
}

Bar::Bar(const sf::Vector2f& _pos, sf::Texture& _textureBack, sf::Texture& _textureBar, sf::Texture& _textureOutline, sf::Texture& _textureSelector, const std::string& _text)
{
	back.setTexture(_textureBack);
	bar.setTexture(_textureBar);
	outline.setTexture(_textureOutline);
	selector.setTexture(_textureSelector);

	setPosition(_pos);

	sf::FloatRect selectRect = selector.getGlobalBounds();
	selector.setOrigin(selectRect.width / 2.f, selectRect.height / 2.f);

	InitText(_text);
}

void Bar::InitText(const std::string& _text)
{
	sf::FloatRect barRect = bar.getGlobalBounds();
	
	text = CreateText(FontsManager::Get()["SPOOKY"], _text, 42);
	SetCenteredText(text);
	SetOutlined(text, 2);
}

void Bar::PollEvent(GameData& _gameData)
{
	if (ControlsManager::Get().GetControllerType() == KEYBOARD)
	{
		if (_gameData.gameEvent.type == sf::Event::MouseButtonPressed)
		{
			CallOnClick(wasOver);
		}
		if (_gameData.gameEvent.type == sf::Event::MouseButtonReleased)
		{
			CallOnReleaseClick();
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			CallWhileClick(wasOver);
		}
		if (_gameData.gameEvent.type == sf::Event::MouseMoved)
		{
			if (enable && visible)
			{
				bool IsOverButton = GetRect().contains(Screen::Get().GetMousePosition(_gameData.window));
				if (IsOverButton && !wasOver)
				{
					_gameData.window.setMouseCursor(_gameData.cursor);
					//Audio::Get().PlaySoundNoSecurity("BUTTON_SWAP");
					wasOver = true;
					CallOnOver();
				}
				else if (!IsOverButton && wasOver)
				{
					_gameData.window.setMouseCursor(_gameData.cursorGrey);
					wasOver = false;
					CallOnOut();
				}
			}
		}
	}
}

void Bar::Update(GameData& _gameData)
{
	sf::FloatRect rect = GetRect();

	int newWidth = static_cast<int>(((float)value / (float)max) * rect.width);
	bar.setTextureRect(sf::IntRect(0, 0, newWidth, static_cast<int>(rect.height)));

	float xPos = Maths::Clamp(static_cast<float>(newWidth), 20, rect.width - 20);
	selector.setPosition(xPos, rect.height / 2.f);

	text.setPosition(sf::Vector2f(rect.width / 2.f, rect.height / 2.f));
	text.setString(std::to_string(value));
	SetCenteredText(text);

	CallOnChange(lastValue != value);
	lastValue = value;
}

void Bar::SetBackSprite(const sf::Texture& _texture)
{
	back.setTexture(_texture);
}

void Bar::SetBarSprite(const sf::Texture& _texture)
{
	bar.setTexture(_texture);
}

void Bar::SetOutlineSprite(const sf::Texture& _texture)
{
	outline.setTexture(_texture);
}

void Bar::SetSelectorSprite(const sf::Texture& _texture)
{
	selector.setTexture(_texture);

	sf::FloatRect selectRect = selector.getGlobalBounds();
	selector.setOrigin(selectRect.width / 2.f, selectRect.height / 2.f);
}

void Bar::SetValue(const short int& _val)
{
	value = static_cast<short>(Maths::Clamp(_val, min, max));
}

void Bar::SetMax(const short int& _val)
{
	max = _val;
}

void Bar::SetMin(const short int& _val)
{
	min = _val;
}

const short int& Bar::GetValue(void)
{
	return value;
}

const short int& Bar::GetMax(void)
{
	return max;
}

const short int& Bar::GetMin(void)
{
	return min;
}

sf::FloatRect Bar::GetRect() const
{
	sf::Vector2f pos = getPosition();
	sf::FloatRect rect = back.getGlobalBounds();
	rect.left = pos.x;
	rect.top = pos.y;

	return rect;
}

sf::FloatRect Bar::GetGlobalBounds(void) const
{
	return bar.getGlobalBounds();
}

void Bar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(back, states);
	target.draw(bar, states);
	target.draw(selector, states);
	target.draw(outline, states);
	target.draw(text, states);
}
