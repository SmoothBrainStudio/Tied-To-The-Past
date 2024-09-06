#ifndef COLLISIONS__HPP
#define COLLISIONS__HPP

#include "Maths.hpp"

namespace Collision
{
	// Check if two segments in the same dimensions are colliding  
	const bool Overlaps(const float& _min1, const float& _max1, const float& _min2, const float& _max2);

	const bool AABBs(const sf::FloatRect& _rect1, const sf::FloatRect& _rect2);

	const bool RectPoint(const sf::FloatRect& _rect, const sf::Vector2f& _point);

	const bool CirclePoint(const float& _radius, const sf::Vector2f& _circlePos, const sf::Vector2f& _point);

	const bool Circles(const float& _radius1, const sf::Vector2f& _position1, const float& _radius2, const sf::Vector2f& _position2);

	void SAT(const sf::Vector2f& _axis, sf::Vector2f _points[], const int& _numPoints, float* _min, float* _max);

	const bool ConvexePoint(sf::Vector2f _conexePoints[], const int& _numPoints, sf::Vector2f* _point);

	const bool PixelPerfect(const sf::Image& _image, const sf::Vector2f& _imagePos, const sf::Vector2f& _point);
}

#endif // !COLLISIONS__HPP