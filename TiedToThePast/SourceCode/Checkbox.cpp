#include "Definitions.hpp"
#include "Checkbox.hpp"

Checkbox::Checkbox()
{
	className = "checkbox";

	checkbox = new sf::RectangleShape;
	checkbox->setSize(sf::Vector2f(SIZE, SIZE));
	check = new sf::RectangleShape;
	check->setSize(sf::Vector2f(SIZE, SIZE));

	text = new sf::Text(CreateText(FontsManager::Get()["SPOOKY"]));
	SetOutlined(*text, 2);

	SetOnClickFct([this]() -> void {
		this->SetValue(!this->IsChecked());
		});
}

Checkbox::~Checkbox()
{
	delete checkbox;
	delete check;
	delete text;
}

void Checkbox::Update(GameData& _gameData)
{
	CallOnChange(lastValue != value);
	lastValue = value;
}

void Checkbox::PollEvent(GameData& _gameData)
{
	if (ControlsManager::Get().GetControllerType() == KEYBOARD)
	{
		CallOnClick(wasOver && _gameData.gameEvent.type == sf::Event::MouseButtonPressed);
		CallWhileClick(wasOver && sf::Mouse::isButtonPressed(sf::Mouse::Left));

		if (_gameData.gameEvent.type == sf::Event::MouseMoved)
		{
			if (enable && visible)
			{
				bool IsOverButton = GetRect().contains(Screen::Get().GetMousePosition(_gameData.window));
				if (IsOverButton && !wasOver)
				{
					_gameData.window.setMouseCursor(_gameData.cursor);
					Audio::Get().PlaySoundNoSecurity("BUTTON_SWAP");
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

void Checkbox::SetCheckboxTexture(const sf::Texture& _texture)
{
	checkbox->setTexture(&_texture, true);
}

void Checkbox::SetCheckTexture(const sf::Texture& _texture)
{
	check->setTexture(&_texture, true);
}

void Checkbox::SetValue(const bool& _value)
{
	value = _value;
}

void Checkbox::SetText(const std::string& _text)
{
	text->setString(_text);

	AdapatText();
}

void Checkbox::SetText(const sf::Text& _text)
{
	delete text;
	text = new sf::Text(_text);

	AdapatText();
}

sf::FloatRect Checkbox::GetRect() const
{
	sf::Vector2f position = getPosition();
	sf::FloatRect result;
	sf::FloatRect checkRect = checkbox->getGlobalBounds();
	sf::FloatRect textRect = text->getGlobalBounds();

	result.left = position.x;
	result.top = position.y;
	result.height = std::max(checkRect.height, textRect.height);
	result.width = checkRect.width + textRect.width + SPACE_BETWEEN_CHECK_TEXT;

	return result;
}

const bool& Checkbox::IsChecked(void)
{
	return value;
}

void Checkbox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(*checkbox, states);
	if (value) target.draw(*check, states);
	target.draw(*text, states);
}

void Checkbox::AdapatText()
{
	sf::FloatRect rectText;
	sf::FloatRect rectCheck;
	sf::Vector2f textOrigin;
	sf::Vector2f textPosition;

	rectText = text->getGlobalBounds();
	rectCheck = checkbox->getGlobalBounds();

	textOrigin.x = 0;
	textOrigin.y = rectText.height / 2.f;
	text->setOrigin(textOrigin);

	textPosition.x = rectCheck.width + SPACE_BETWEEN_CHECK_TEXT;
	textPosition.y = rectCheck.height / 2.f - 2;
	text->setPosition(textPosition);
}
