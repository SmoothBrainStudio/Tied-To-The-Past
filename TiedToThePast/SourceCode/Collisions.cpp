#include "Definitions.hpp"
#include "Collisions.hpp"


const bool Collision::Overlaps(const float& _min1, const float& _max1, const float& _min2, const float& _max2)
{
	return (_min1 <= _min2 && _min2 <= _max1) || (_min2 <= _min1 && _min1 <= _max2);
}

const bool Collision::AABBs(const sf::FloatRect& _rect1, const sf::FloatRect& _rect2)
{
	return !(_rect1.top + _rect1.height < _rect2.top || _rect1.left + _rect1.width < _rect2.left ||
		_rect2.top + _rect2.height < _rect1.top || _rect2.left + _rect2.width < _rect1.left);
}

const bool Collision::RectPoint(const sf::FloatRect& _rect, const sf::Vector2f& _point)
{
	return (_point.x > _rect.left) && (_point.x < _rect.left + _rect.width) && (_point.y > _rect.top) && (_point.y < _rect.top + _rect.height);
}

const bool Collision::CirclePoint(const float& _radius, const sf::Vector2f& _circlePos, const sf::Vector2f& _point)
{
	return Maths::GetSquaredLength(_circlePos, _point) < (_radius * _radius);
}

const bool Collision::Circles(const float& _radius1, const sf::Vector2f& _position1, const float& _radius2, const sf::Vector2f& _position2)
{
	const float sumRadiusCircles = _radius1 + _radius2;

	return Maths::GetSquaredLength(_position1, _position2) < (sumRadiusCircles * sumRadiusCircles);
}

void Collision::SAT(const sf::Vector2f& _axis, sf::Vector2f _points[], const int& _numPoints, float* _min, float* _max)
{
	*_min = FLT_MAX;
	*_max = FLT_MIN;

	for (int i = 0; i < _numPoints; i++)
	{
		const float dotProduct = Maths::ProjectionOnAxis(_axis, _points[i]);

		if (dotProduct < *_min)
		{
			*_min = dotProduct;
		}

		if (dotProduct > *_max)
		{
			*_max = dotProduct;
		}
	}
}

const bool Collision::ConvexePoint(sf::Vector2f _conexePoints[], const int& _numPoints, sf::Vector2f* _point)
{
	float point1Min;
	float point1Max;
	float point2Min;
	float point2Max;

	for (int i = 0; i < 4; i++)
	{
		int j = i + 1;
		if (j == 4)
		{
			j = 0;
		}

		sf::Vector2f axe = Maths::CreateVector(_conexePoints[i], _conexePoints[j]);
		SAT(axe, _conexePoints, 4, &point1Min, &point1Max);
		SAT(axe, _point, 1, &point2Min, &point2Max);

		if (!Overlaps(point1Min, point1Max, point2Min, point2Max))
		{
			return false;
		}
	}

	return true;
}

const bool Collision::PixelPerfect(const sf::Image& _image, const sf::Vector2f& _imagePos, const sf::Vector2f& _point)
{
	sf::Color pixColor = _image.getPixel((unsigned int)(_point.x - _imagePos.x), (unsigned int)(_point.y - _imagePos.y));
	return (pixColor.a != 0);
}
