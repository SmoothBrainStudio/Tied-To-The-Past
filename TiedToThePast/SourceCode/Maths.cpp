#include "Definitions.hpp"
#include "Maths.hpp"


const sf::Vector2f Maths::CreateVector(const sf::Vector2f& _point1, const sf::Vector2f& _point2)
{
	return sf::Vector2f(_point2.x - _point1.x, _point2.y - _point1.y);
}

const float Maths::GetVectorLength(const sf::Vector2f& _vector)
{
	return sqrtf(_vector.x * _vector.x + _vector.y * _vector.y);
}

const float Maths::GetVectorLength(const sf::Vector2f& _point1, const sf::Vector2f& _point2)
{
	return Maths::GetVectorLength(CreateVector(_point1, _point2));
}

const float Maths::GetSquaredLength(const sf::Vector2f& _vector)
{
	return _vector.x * _vector.x + _vector.y * _vector.y;
}

const float Maths::GetSquaredLength(const sf::Vector2f& _point1, const sf::Vector2f& _point2)
{
	return Maths::GetSquaredLength(Maths::CreateVector(_point1, _point2));
}

const float Maths::DotProduct(const sf::Vector2f& _vector1, const sf::Vector2f& _vector2)
{
	return _vector1.x * _vector2.x + _vector1.y * _vector2.y;
}

const float Maths::ProjectionOnAxis(const sf::Vector2f& _axis, const sf::Vector2f& _point)
{
	return Maths::DotProduct(_axis, _point);
}

const sf::Vector2f Maths::NormalizeVector(const sf::Vector2f& _vector)
{
	const float vectorLength = GetVectorLength(_vector);
	const sf::Vector2f normalizedVector = vectorLength == 0.f ? sf::Vector2f(0.f, 0.f) : _vector / vectorLength;

	return normalizedVector;
}

const sf::Vector2f Maths::RotateVector(const sf::Vector2f& _vector, const float& _angle)
{
	const float COS = cos(_angle);
	const float SIN = sin(_angle);

	const float newX = _vector.x * COS - _vector.y * SIN;
	const float newY = _vector.x * SIN + _vector.y * COS;

	return sf::Vector2f(newX, newY);
}

const float Maths::DegreesToRadians(const float& _angle)
{
	return _angle * PI / 180.f;
}

const float Maths::RadiansToDegrees(const float& _angle)
{
	return _angle * 180.f / PI;
}

const float Maths::PreciseRand(const float& _max, const float& _min)
{
	return float(rand()) / RAND_MAX * (_max - _min) + _min;
}

const float Maths::Clamp(const float& _value, const float& _min, const float& _max)
{
	if (_value > _max)
	{
		return _max;
	}
	else if (_value < _min)
	{
		return _min;
	}
	else
	{
		return _value;
	}
}

const float Maths::GetAngle(const sf::Vector2f& _fromPoint, const sf::Vector2f& _toPoint)
{
	float diffX = _toPoint.x - _fromPoint.x;
	float diffY = _toPoint.y - _fromPoint.y;
	float angle = atan2f(diffY, diffX);

	return angle;
}

float Maths::GetPower(float _value, size_t _power)
{
	if (_power == 0)
	{
		return 1.f;
	}

	while (_power > 1)
	{
		_value *= _value;
		--_power;
	}
	return _value;
}

const sf::Vector2f Maths::ProjectPoint(const sf::Vector2f& _startingPoint, const float& _distance, const float& _AngleRad)
{
	return { _startingPoint.x + _distance * cosf(_AngleRad), _startingPoint.y + _distance * sinf(_AngleRad) };
}

const sf::Vector2f Maths::InterpolateVector(const sf::Vector2f& _pos1, const sf::Vector2f& _pos2, const float& _factor)
{
	sf::Vector2f vectorBetween;
	sf::Vector2f result;

	vectorBetween = Maths::CreateVector(_pos1, _pos2);
	result = _pos1 + vectorBetween * _factor;

	return result;
}
