#ifndef PARTICLE_SHAPE__HPP
#define PARTICLE_SHAPE__HPP

#include "../Definitions.hpp"
#include "../GameClock.hpp"
#include "../Maths.hpp"

namespace ps
{
	class Shape : public sf::Transformable
	{
	private:

	public:
		Shape() = default;
		~Shape() = default;

		virtual sf::Vector2f GetRandomPosition() = 0;
		virtual sf::Vector2f GetVelocity(const sf::Vector2f& _position) = 0;

	};

	class Circle : public Shape
	{
	private:
		float m_spawnRadius;
		float m_finalRadius;

	public:
		Circle() = default;
		Circle(const float& _spawnRadius, const float& _finalRadius);
		~Circle() = default;

		sf::Vector2f GetRandomPosition() override;
		sf::Vector2f GetVelocity(const sf::Vector2f& _position) override;

		void SetSpawnRadius(const float& _spawnRadius);
		void SetFinalRadius(const float& _finalRadius);

		const float& GetSpawnRadius(void) const;
		const float& GetFinalRadius(void) const;

	};

	class Cone : public Shape
	{
	private:
		float m_radius;
		float m_angle;
		float m_randAng;

	public:
		Cone() = default;
		Cone(const float& _radius, const float& _angle);
		~Cone() = default;

		sf::Vector2f GetRandomPosition() override;
		sf::Vector2f GetVelocity(const sf::Vector2f& _position) override;

		void SetRadius(const float& _radius);
		void SetAngle(const char& _angle);

		const float& GetRadius(void) const;
		const char GetAngle(void) const;

	};
}

#endif // !PARTICLE_SHAPE__HPP
