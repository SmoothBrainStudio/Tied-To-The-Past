#include "Definitions.hpp"
#include "Shapes.hpp"

sf::RectangleShape Shapes::CreateRectangle(const sf::Vector2f& _pos, const sf::Vector2f& _size, const sf::Color& _color, const bool& _isCentered)
{
	sf::RectangleShape rect;

	rect.setSize(_size);
	if (_isCentered)
	{
		rect.setOrigin(_size.x / 2.f, _size.y / 2.f);
	}
	rect.setPosition(_pos);
	rect.setFillColor(_color);
	rect.setOutlineColor(sf::Color::Magenta);

	return rect;
}

sf::CircleShape Shapes::CreateCircle(const sf::Vector2f& _pos, const float& _rad, const sf::Color& _color, const bool& _isCentered)
{
	sf::CircleShape circle;

	circle.setRadius(_rad);
	if (_isCentered)
	{
		circle.setOrigin(_rad / 2.f, _rad / 2.f);
	}
	circle.setPosition(_pos);
	circle.setFillColor(_color);
	circle.setOutlineColor(sf::Color::Magenta);

	return circle;
}