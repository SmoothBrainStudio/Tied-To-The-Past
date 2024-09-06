#ifndef SHAPES__HPP
#define SHAPES__HPP

#include"Common.hpp"

namespace Shapes
{
	sf::RectangleShape CreateRectangle(const sf::Vector2f& _pos, const sf::Vector2f& _size, const sf::Color& _color = sf::Color::White, const bool& _isCentered = true);

	sf::CircleShape CreateCircle(const sf::Vector2f& _pos, const float& _rad, const sf::Color& _color = sf::Color::White , const bool& _isCentered = true);
};


#endif // !SHAPES_HPP


