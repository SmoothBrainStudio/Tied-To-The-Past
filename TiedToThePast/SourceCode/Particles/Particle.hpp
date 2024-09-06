#ifndef PARTICLE__HPP
#define PARTICLE__HPP

#include "../Definitions.hpp"
#include "../GameClock.hpp"
#include "../Maths.hpp"

#define GRAVITY sf::Vector2f(0.f, 900.f)

struct Particle : public sf::Transformable
{
	// Method(s)
	void Update(void);
	void ApplyForce(const sf::Vector2f& _force);

	float m_lifeTime;
	float m_gravityScale;

	float m_maxSpeed;
	float m_speed;

	float m_scale;
	float m_rotation;
	sf::Color m_saveColor;
	sf::Color m_curColor;

	sf::Vector2f m_velocity;
	sf::Vector2f m_AppliedForces = sf::Vector2f(0, 0);
	sf::Vector2f initialScale = sf::Vector2f(1, 1);
	sf::Vector2f initialPos;
	float m_linearDamping;
	float m_mass;

};

#endif // !PARTICLE__HPP
