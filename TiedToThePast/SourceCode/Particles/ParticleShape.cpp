#include "ParticleShape.hpp"

namespace ps
{
	Circle::Circle(const float& _spawnRadius, const float& _finalRadius)
	{
		m_spawnRadius = Maths::Clamp(_spawnRadius, 1.f, FLT_MAX);
		m_finalRadius = Maths::Clamp(_finalRadius, 1.f, FLT_MAX);
	}

	sf::Vector2f Circle::GetRandomPosition()
	{
		sf::Vector2f result(Maths::PreciseRand(m_spawnRadius, 0.f), 0.f);
		result = Maths::RotateVector(result, Maths::DegreesToRadians(Maths::PreciseRand(360.f, 0.f)));
		return result;
	}

	sf::Vector2f Circle::GetVelocity(const sf::Vector2f& _position)
	{
		sf::Vector2f velocity = Maths::CreateVector(sf::Vector2f(0, 0), _position);
		sf::Vector2f normalizeVelocity = Maths::NormalizeVector(velocity);
		return (normalizeVelocity * m_finalRadius);
	}

	void Circle::SetSpawnRadius(const float& _spawnRadius)
	{
		m_spawnRadius = _spawnRadius;
	}

	void Circle::SetFinalRadius(const float& _finalRadius)
	{
		m_finalRadius = _finalRadius;
	}

	const float& Circle::GetSpawnRadius(void) const
	{
		return m_spawnRadius;
	}

	const float& Circle::GetFinalRadius(void) const
	{
		return m_finalRadius;
	}

	Cone::Cone(const float& _radius, const float& _angle)
		: m_randAng(0.f)
	{
		m_radius = Maths::Clamp(_radius, 0.f, FLT_MAX);
		m_angle = Maths::Clamp(_angle, 0.f, 90.f);
	}

	sf::Vector2f Cone::GetRandomPosition()
	{
		sf::Vector2f position(0, 0);
		m_randAng = Maths::PreciseRand(m_radius, -m_radius);
		position = sf::Vector2f(m_randAng, 0.f);
		position = Maths::RotateVector(position, Maths::DegreesToRadians(getRotation()));

		return position;
	}

	sf::Vector2f Cone::GetVelocity(const sf::Vector2f& _position)
	{
		sf::Vector2f velocity(0, 1);
		velocity = Maths::RotateVector(velocity, Maths::DegreesToRadians(m_angle * -(m_randAng / m_radius) + getRotation()));

		return velocity;
	}

	void Cone::SetRadius(const float& _radius)
	{
		m_radius = _radius;
	}

	void Cone::SetAngle(const char& _angle)
	{
		m_angle = _angle;
	}

	const float& Cone::GetRadius(void) const
	{
		return m_radius;
	}

	const char Cone::GetAngle(void) const
	{
		return (char)m_angle;
	}
}
