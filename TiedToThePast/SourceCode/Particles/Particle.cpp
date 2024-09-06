#include "Particle.hpp"

void Particle::Update(void)
{
	const float dt = GameClock::Get().GetDeltaTime();
	m_lifeTime -= dt;

	// Forces
	m_velocity += dt * m_mass * m_AppliedForces;

	// Apply
	move(m_velocity * dt);
	m_AppliedForces = sf::Vector2f(0.f, 0.f);

	// Gravity apply
	ApplyForce(GRAVITY * m_gravityScale);
	// Linear Damping apply
	ApplyForce(-m_linearDamping * m_velocity);

	// Speed calcul
	sf::Vector2f position = getPosition();
	m_speed = Maths::GetVectorLength(position, position + m_AppliedForces);
	if (m_speed > m_maxSpeed) m_maxSpeed = m_speed;
}

void Particle::ApplyForce(const sf::Vector2f& _force)
{
	m_AppliedForces  += _force;
}
