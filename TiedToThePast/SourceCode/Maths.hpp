#ifndef MATHS__HPP
#define MATHS__HPP

#include "Definitions.hpp"

namespace Maths
{

	const sf::Vector2f CreateVector(const sf::Vector2f& _point1, const sf::Vector2f& _point2);

	const float GetVectorLength(const sf::Vector2f& _vector);

	const float GetVectorLength(const sf::Vector2f& _point1, const sf::Vector2f& _point2);

	const float GetSquaredLength(const sf::Vector2f& vector);

	const float GetSquaredLength(const sf::Vector2f& _point1, const sf::Vector2f& _point2);

	const float DotProduct(const sf::Vector2f& _vector1, const sf::Vector2f& _vector2);

	const float ProjectionOnAxis(const sf::Vector2f& _axis, const sf::Vector2f& _point);

	const sf::Vector2f NormalizeVector(const sf::Vector2f& _vector);

	const sf::Vector2f RotateVector(const sf::Vector2f& _vector, const float& _angle);

	const float DegreesToRadians(const float& _angle);

	const float RadiansToDegrees(const float& _angle);

	const float PreciseRand(const float& _max, const float& _min);

	const float Clamp(const float& _value, const float& _min, const float& _max);

	const float GetAngle(const sf::Vector2f& _fromPoint, const sf::Vector2f& _toPoint);

	float GetPower(float _value, size_t power);

	const sf::Vector2f InterpolateVector(const sf::Vector2f& _pos1, const sf::Vector2f& _pos2, const float& _factor);

	// Return point with distance and angle of a starting point
	const sf::Vector2f ProjectPoint(const sf::Vector2f& _startingPoint, const float& _distance, const float& _AngleRad);

	template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
	const bool IsSorted(const std::vector<T> _vector)
	{
		for (int i = 1; i < _vector.size(); i++)
		{
			if (_vector[i] < _vector[i-1])
			{
				return false;
			}
		}
		return true;
	}

	template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
	const bool IsIn(const std::vector<T> _vector, T _value)
	{
		for (const auto& value : _vector)
		{
			if (value == _value)
			{
				return true;
			}
		}
		return false;
	}
};

#endif // !MATHS__HPP