#include "Definitions.hpp"
#include "Text.hpp"

sf::Text CreateText(const sf::Font& _font, const std::string& _string, const unsigned short int& _size, const sf::Color& _color, const bool& _isCentered)
{
	sf::Text newText;

	newText.setString(_string);
	newText.setFont(_font);
	newText.setCharacterSize(_size);
	newText.setFillColor(_color);

	if (_isCentered)
	{
		SetCenteredText(newText);
	}

	return newText;
}

void SetCenteredText(sf::Text& _text)
{
	sf::FloatRect textLocalBounds = _text.getLocalBounds();
	_text.setOrigin(sf::Vector2f(roundf(textLocalBounds.left + textLocalBounds.width / 2.f), roundf(textLocalBounds.top + textLocalBounds.height / 2.f)));
}

void SetCenteredString(sf::Text& _text, std::string _string)
{
	_text.setString(_string);
	sf::FloatRect textLocalBounds = _text.getLocalBounds();
	_text.setOrigin(sf::Vector2f(roundf(textLocalBounds.left + textLocalBounds.width / 2.f), roundf(textLocalBounds.top + textLocalBounds.height / 2.f)));
}

void SetCenteredToObject(sf::Text& _text, const sf::Transformable& _object)
{
	SetCenteredText(_text);
	_text.setPosition(_object.getPosition());
}

void SetOutlined(sf::Text& _text, const unsigned short int& _thickness, const sf::Color& _color)
{
	_text.setOutlineThickness((float)_thickness);
	_text.setOutlineColor(_color);
}

int countOneLetter(std::string _text, char _letter)
{
	int count = 0;
	for (int i = 0; i < _text.size(); i++)
	{
		if (_text[i] == _letter)
		{
			count++;
		}
	}
	return count;
}