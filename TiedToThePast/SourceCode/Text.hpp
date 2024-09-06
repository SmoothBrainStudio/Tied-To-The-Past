#ifndef TEXT__HPP
#define TEXT__HPP

#include "Definitions.hpp"

static constexpr unsigned short int DEFAULT_SIZE_TEXT = 24;

sf::Text CreateText(const sf::Font& _font, const std::string& _string = "", const unsigned short int& _size = DEFAULT_SIZE_TEXT, const sf::Color& = sf::Color::White, const bool& _isCentered = false);

void SetCenteredText(sf::Text& _text);
void SetCenteredString(sf::Text& _text, std::string _string);
void SetCenteredToObject(sf::Text& _text, const sf::Transformable& _object);

void SetOutlined(sf::Text& _text, const unsigned short int& _thickness, const sf::Color& _color = sf::Color::Black);

int countOneLetter(std::string _text, char _letter);

#endif