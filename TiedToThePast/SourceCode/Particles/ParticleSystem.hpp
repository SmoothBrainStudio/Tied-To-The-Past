// Unity-inspired particle system
#ifndef PARTICLE_SYSTEM__HPP
#define PARTICLE_SYSTEM__HPP

#include "Particle.hpp"
#include "ParticleShape.hpp"
#include "../Color.hpp"
#include "../JsonManager.hpp"

class ParticleSystem : public sf::Drawable , public sf::Transformable
{
public:
	// Enum(s)
	enum class Statue
	{
		PLAYING,
		PAUSED,
		STOPPED
	};
private:
	// Enum(s)
	enum class Shape;

	struct Burst
	{
		Burst();
		void Reset();

		int m_saveCycle;
		float m_totalTime;
		float m_intervalTime;

		float m_time;
		int m_count;
		int m_cycle;
		float m_interval;
		float m_probability;
	};

	// Private method(s)
	void GetValueByType(const Json::Value& _value, std::array<float, 2>& _array);
	void GetValueByType(const Json::Value& _value, std::array<sf::Color, 2>& _array);
	float GetValue(std::array<float, 2>& _array);
	sf::Color GetValue(std::array<sf::Color, 2>& _array);
	sf::Color GetColorBetweenTwoAt(const sf::Color& _color1, const sf::Color& _color2, const float& _val);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	// System member(s)
	std::vector<Particle> m_particleList;
	std::array<sf::Vertex, 4>* m_particleVertex;
	Statue m_curStatue;
	double m_lifeTime;
	double m_durationTime;
	double m_rateByTime;
	double m_rateTime;
	ps::Shape* m_shape;
	std::vector<Burst> m_burstList;

	// Paricle member(s)
	sf::Texture m_texture;
	float m_duration;
	float m_gravityScale;
	float m_linearDamping;
	unsigned int m_maxParticle;
	float m_mass;
	std::array<sf::Color, 2> m_color;
	std::array<float, 2> m_scale;
	std::array<float, 2> m_speed;
	std::array<float, 2> m_rotation;
	bool m_loop;

	// Over time member(s)
	bool m_enableColorOverTime;
	std::array<sf::Color, 2> m_colorOverTime;
	bool m_enableScaleOverTime;
	std::array<float, 2> m_scaleOverTime;
	bool m_enableRotationOverTime;
	std::array<float, 2> m_rotationOverTime;

	// Over speed member(s)
	bool m_enableColorOverSpeed;
	std::array<sf::Color, 2> m_colorOverSpeed;
	bool m_enableScaleOverSpeed;
	std::array<float, 2> m_scaleOverSpeed;
	bool m_enableRotationOverSpeed;
	std::array<float, 2> m_rotationOverSpeed;

public:
	// Constructor(s) & Destructor(s)
	ParticleSystem(const std::string& _path);
	~ParticleSystem();

	// Main method(s)
	void LoadFromFile(const std::string& _path);
	void AddParticle(void);
	void Update(void);
	void Display(sf::RenderWindow& _window);
	// Working method(s)
	void Play(void);
	void Reset(void);
	void Pause(void);
	void Stop(void);

	// Setter(s)
	void SetStatue(const Statue& _statue);
	void SetTexture(const sf::Texture& _texture);
	void SetDuration(const float& _duration);
	void SetRateByTime(const float& _rateByTime);
	void SetGravityScale(const float& _gravityScale);
	void SetMaxParticle(const unsigned int& _maxParticle);
	void SetLoop(const bool& _loop);
	void SetShape(ps::Shape& _shape);

	// Getter(s)
	const unsigned int GetParticleCount(void) const;
	const double& GetLifeTimeSystem(void) const;
	const Statue& GetStatue(void) const;
	const sf::Texture& GetTexture(void) const;
	const float& GetDuration(void) const;
	const double& GetRateByTime(void) const;
	const float& GetGravityScale(void) const;
	const unsigned int& GetMaxParticle(void) const;
	const bool& IsLoop(void) const;
	const ps::Shape& GetShape(void) const;

};

#endif // !PARTICLE_SYSTEM__HPP
