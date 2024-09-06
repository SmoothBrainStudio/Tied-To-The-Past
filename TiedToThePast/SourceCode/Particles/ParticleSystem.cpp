#include "ParticleSystem.hpp"
#include "Particle.hpp"
#include "../Screen.hpp"

#pragma region CONSTRUCTOR(S) & DESTRUCTOR(S)

ParticleSystem::ParticleSystem(const std::string& _path)
	: m_maxParticle(0), m_curStatue(Statue::STOPPED), m_lifeTime(0.f), m_durationTime(0.f), m_rateTime(0.f)
{
	LoadFromFile(_path);
}

ParticleSystem::~ParticleSystem()
{
	if (m_shape != nullptr)
	{
		delete m_shape;
		m_shape = nullptr;
	}

	delete m_particleVertex;
	m_particleVertex = nullptr;
}

ParticleSystem::Burst::Burst()
	: m_saveCycle(0), m_totalTime(0.f), m_intervalTime(0.f), m_time(0.f), m_count(0), m_cycle(0), m_interval(0.f), m_probability(0.f)
{
}

#pragma endregion

#pragma region MAIN_METHODS

void ParticleSystem::LoadFromFile(const std::string& _path)
{
	Reset();
	m_burstList.clear();

	// Load json file
	Json::Value json = JsonManager::LoadFromFile(_path);

	// ShapeSelect
	switch (json["Shape"]["type"].asInt())
	{
	case 1:			// Is cone
		m_shape = new ps::Cone(json["Shape"]["radius"].asFloat(), json["Shape"]["angle"].asFloat());
		break;
	default:		// Is circle
		m_shape = new ps::Circle(json["Shape"]["spawnRadius"].asFloat(), json["Shape"]["finalRadius"].asFloat());
		break;
	}

	// Set all particle parameters
	m_texture.loadFromFile(json["Texture"].asString());
	m_texture.setSmooth(json["IsSmooth"].asBool());
	m_duration = json["Duration"].asFloat();
	m_rateByTime = json["RateByTime"].asInt();
	m_gravityScale = json["GravityScale"].asFloat();
	m_mass = json["Mass"].asFloat();
	m_linearDamping = json["LinearDamping"].asFloat();
	m_maxParticle = json["MaxParticle"].asInt();
	m_loop = json["Loop"].asBool();
	GetValueByType(json["StartColor"], m_color);
	GetValueByType(json["StartScale"], m_scale);
	GetValueByType(json["StartRotation"], m_rotation);
	GetValueByType(json["StartSpeed"], m_speed);

	if (json["StartAwake"].asBool()) m_curStatue = Statue::PLAYING;
	else m_curStatue = Statue::STOPPED;

	// Burst(s)
	for (unsigned int i = 0; i < json["Burst"].size(); i++)
	{
		Burst newBurst{};
		newBurst.m_time = json["Burst"][i]["time"].asFloat();
		newBurst.m_count = json["Burst"][i]["count"].asInt();
		newBurst.m_saveCycle = json["Burst"][i]["cycle"].asInt();
		newBurst.m_interval = json["Burst"][i]["interval"].asFloat();
		newBurst.m_probability = json["Burst"][i]["probability"].asFloat();
		newBurst.m_cycle = newBurst.m_saveCycle;
		newBurst.m_intervalTime = newBurst.m_interval;
		newBurst.m_totalTime = 0.f;

		m_burstList.push_back(newBurst);
	}

	// Over time member(s)
	m_enableScaleOverTime = json["ScaleOverTime"]["enable"].asBool();
	m_scaleOverTime[0] = json["ScaleOverTime"]["max"].asFloat();
	m_scaleOverTime[1] = json["ScaleOverTime"]["min"].asFloat();

	m_enableRotationOverTime = json["RotationOverTime"]["enable"].asBool();
	m_rotationOverTime[0] = json["RotationOverTime"]["max"].asFloat();
	m_rotationOverTime[1] = json["RotationOverTime"]["min"].asFloat();

	m_enableColorOverTime = json["ColorOverTime"]["enable"].asBool();
	m_colorOverTime[0] = HexToRgba(json["ColorOverTime"]["max"].asString());
	m_colorOverTime[1] = HexToRgba(json["ColorOverTime"]["min"].asString());

	// Over speed member(s)
	m_enableScaleOverSpeed = json["ScaleOverSpeed"]["enable"].asBool();
	m_scaleOverSpeed[0] = json["ScaleOverSpeed"]["max"].asFloat();
	m_scaleOverSpeed[1] = json["ScaleOverSpeed"]["min"].asFloat();

	m_enableRotationOverSpeed = json["RotationOverSpeed"]["enable"].asBool();
	m_rotationOverSpeed[0] = json["RotationOverSpeed"]["max"].asFloat();
	m_rotationOverSpeed[1] = json["RotationOverSpeed"]["min"].asFloat();

	m_enableColorOverSpeed = json["ColorOverSpeed"]["enable"].asBool();
	m_colorOverSpeed[0] = HexToRgba(json["ColorOverSpeed"]["max"].asString());
	m_colorOverSpeed[1] = HexToRgba(json["ColorOverSpeed"]["min"].asString());

	// Create new vertexs
	if (!m_particleVertex) delete m_particleVertex;
	m_particleVertex = new std::array<sf::Vertex, 4>[m_maxParticle];
}

void ParticleSystem::AddParticle(void)
{
	if (m_particleList.size() < m_maxParticle)
	{
		// Get random velocity
		sf::Vector2f rdmPosition = m_shape->GetRandomPosition();
		sf::Vector2f startVelocity = m_shape->GetVelocity(rdmPosition);

		// Create new particle
		Particle newParticle;
		newParticle.m_lifeTime = m_duration;
		newParticle.m_gravityScale = m_gravityScale;
		newParticle.m_maxSpeed = GetValue(m_speed);
		newParticle.m_speed = newParticle.m_maxSpeed;
		newParticle.m_saveColor = GetValue(m_color);
		newParticle.m_curColor = newParticle.m_saveColor;
		newParticle.m_velocity = startVelocity * newParticle.m_speed;
		newParticle.m_mass = m_mass;
		newParticle.m_linearDamping = m_linearDamping;
		newParticle.m_rotation = GetValue(m_rotation);
		newParticle.m_scale = GetValue(m_scale);

		newParticle.setOrigin(sf::Vector2f(m_texture.getSize() / 2u));
		newParticle.setPosition(getPosition() + rdmPosition);
		newParticle.setRotation(newParticle.m_rotation);
		newParticle.setScale(newParticle.m_scale, newParticle.m_scale);
		newParticle.initialScale = sf::Vector2f(newParticle.m_scale, newParticle.m_scale);
		newParticle.initialPos = newParticle.getPosition();

		// Add to vector
		m_particleList.push_back(newParticle);
	}
}

void ParticleSystem::Update(void)
{
	m_shape->setRotation(getRotation());

	if (m_durationTime > m_duration)
	{
		if (m_loop) m_durationTime = 0.f;
		else m_curStatue = Statue::STOPPED;
	}

	if (m_curStatue == Statue::PLAYING)
	{
		const double dt = GameClock::Get().GetDeltaTime();

		m_lifeTime += dt;
		m_durationTime += dt;
		m_rateTime += dt;

		// Burst(s)
		for (int i = 0; i < m_burstList.size(); i++)
		{
			m_burstList[i].m_totalTime += (float)dt;
			if (m_burstList[i].m_totalTime >= m_burstList[i].m_time)
			{
				m_burstList[i].m_intervalTime += (float)dt;
				while (m_burstList[i].m_intervalTime >= m_burstList[i].m_interval && m_burstList[i].m_cycle > 0)
				{
					if (Maths::PreciseRand(1.f, 0.f) <= m_burstList[i].m_probability)
					{
						for (int j = 0; j < m_burstList[i].m_count; j++)
						{
							AddParticle();
						}
					}

					m_burstList[i].m_cycle--;
					m_burstList[i].m_intervalTime -= m_burstList[i].m_interval;
				}
			}
		}

		// Add particles by the rateByTime
		double rateByFrame = 1.0f / m_rateByTime;
		while (m_rateTime >= rateByFrame)
		{
			m_rateTime -= rateByFrame;
			AddParticle();
		}
	}

	// Update all particles
	for (int i = 0; i < m_particleList.size(); i++)
	{
		m_particleList[i].setScale(m_particleList[i].initialScale);
		m_particleList[i].setPosition(m_particleList[i].initialPos);
		// Velocity update
		m_particleList[i].Update();

		// Rotation / Scale update over time
		float factorTime = (m_duration - m_particleList[i].m_lifeTime) / m_duration;

		if (m_enableRotationOverTime)
		{
			float calculRot = -(m_rotationOverTime[0] - m_rotationOverTime[1]) * factorTime + m_rotationOverTime[0];
			m_particleList[i].setRotation(m_particleList[i].m_rotation + calculRot);
		}

		if (m_enableScaleOverTime)
		{
			float calculSca = -(m_scaleOverTime[0] - m_scaleOverTime[1]) * factorTime + m_scaleOverTime[0];
			m_particleList[i].setScale(sf::Vector2f(m_particleList[i].m_scale * calculSca, m_particleList[i].m_scale * calculSca));
		}

		if (m_enableColorOverTime)
		{
			m_particleList[i].m_curColor = m_particleList[i].m_saveColor * GetColorBetweenTwoAt(m_colorOverTime[0], m_colorOverTime[1], factorTime);
		}

		// Rotation / Scale update over speed
		float factorSpeed = m_particleList[i].m_speed / m_particleList[i].m_maxSpeed;

		if (m_enableRotationOverSpeed)
		{
			float calculRot = -(m_rotationOverSpeed[0] - m_rotationOverSpeed[1]) * factorSpeed + m_rotationOverSpeed[0];
			m_particleList[i].setRotation(m_particleList[i].m_rotation + calculRot);
		}

		if (m_enableScaleOverSpeed)
		{
			float calculSca = -(m_scaleOverSpeed[0] - m_scaleOverSpeed[1]) * factorSpeed + m_scaleOverSpeed[0];
			m_particleList[i].setScale(sf::Vector2f(m_particleList[i].m_scale * calculSca, m_particleList[i].m_scale * calculSca));
		}

		if (m_enableColorOverSpeed)
		{
			m_particleList[i].m_curColor = m_particleList[i].m_saveColor * GetColorBetweenTwoAt(m_colorOverSpeed[0], m_colorOverSpeed[1], factorSpeed);
		}

		// If is alive
		if (m_particleList[i].m_lifeTime < 0)
		{
			// Swap and pop to optimise
			std::swap(m_particleList[i], m_particleList.back());
			m_particleList.pop_back();
			--i;
		}
		else
		{
			// Update vertexs
			sf::Vector2f size = (sf::Vector2f)m_texture.getSize();
			sf::Color color = m_particleList[i].m_curColor;

			sf::Vertex tempVertex[4] = {
				sf::Vertex(sf::Vector2f(0.f, 0.f),				color, sf::Vector2f(0.f, 0.f)),
				sf::Vertex(sf::Vector2f(0.f, size.y),			color, sf::Vector2f(0.f, size.y)),
				sf::Vertex(sf::Vector2f(size.x, size.y),	color, sf::Vector2f(size.x, size.y)),
				sf::Vertex(sf::Vector2f(size.x, 0.f),			color, sf::Vector2f(size.x, 0.f))
			};
			for (int j = 0; j < 4; j++)
			{
				m_particleVertex[i][j] = tempVertex[j];
			}

			m_particleList[i].initialScale = m_particleList[i].getScale();
			m_particleList[i].initialPos = m_particleList[i].getPosition();
			m_particleList[i].setScale(sf::Vector2f(m_particleList[i].initialScale.x * getScale().x, m_particleList[i].initialScale.y * getScale().y));
			m_particleList[i].setPosition(Screen::Get().Pos(m_particleList[i].initialPos));
		}
	}
}

void ParticleSystem::Display(sf::RenderWindow& _window)
{
	for (int i = 0; i < m_particleList.size(); i++)
	{
		sf::RenderStates states;
		states.transform *= m_particleList[i].getTransform();
		states.texture = &m_texture;
		_window.draw(m_particleVertex[i].data(), 4, sf::Quads, states);
	}
}

#pragma endregion

#pragma region WORKING_METHODS

void ParticleSystem::Play(void)
{
	m_curStatue = Statue::PLAYING;
}

void ParticleSystem::Reset(void)
{
	m_particleList.clear();
	m_lifeTime = 0.f;
	m_durationTime = 0.f;
	m_rateTime = 0.f;

	for (int i = 0; i < m_burstList.size(); i++)
	{
		m_burstList[i].Reset();
	}
}

void ParticleSystem::Pause(void)
{
	m_curStatue = Statue::PAUSED;
}

void ParticleSystem::Stop(void)
{
	m_curStatue = Statue::STOPPED;
	Reset();
}

#pragma endregion

#pragma region PRIVATE_METHODS

void ParticleSystem::GetValueByType(const Json::Value& _value, std::array<float, 2>& _array)
{
	std::string type = _value["type"].asString();
	if (type == "normal")
	{
		_array[0] = _value["value"].asFloat();
		_array[1] = _array[0];
	}
	else if (type == "random")
	{
		_array[0] = _value["value"][0].asFloat();
		_array[1] = _value["value"][1].asFloat();
	}
	else
	{
		throw;
	}
}

void ParticleSystem::GetValueByType(const Json::Value& _value, std::array<sf::Color, 2>& _array)
{
	std::string type = _value["type"].asString();
	if (type == "normal")
	{
		_array[0] = HexToRgba(_value["value"].asString());
		_array[1] = _array[0];
	}
	else if (type == "random")
	{
		_array[0] = HexToRgba(_value["value"][0].asString());
		_array[1] = HexToRgba(_value["value"][1].asString());
	}
	else
	{
		throw;
	}
}

float ParticleSystem::GetValue(std::array<float, 2>& _array)
{
	return Maths::PreciseRand(_array[1], _array[0]);
}

sf::Color ParticleSystem::GetValue(std::array<sf::Color, 2>& _array)
{
	return GetColorBetweenTwoAt(_array[0], _array[1], Maths::PreciseRand(1.f, 0.f));
}

sf::Color ParticleSystem::GetColorBetweenTwoAt(const sf::Color& _color1, const sf::Color& _color2, const float& _val)
{
	sf::Color result;

	result.r = (sf::Uint8)std::round(_color1.r + (_color2.r - _color1.r) * _val);
	result.g = (sf::Uint8)std::round(_color1.g + (_color2.g - _color1.g) * _val);
	result.b = (sf::Uint8)std::round(_color1.b + (_color2.b - _color1.b) * _val);
	result.a = (sf::Uint8)std::round(_color1.a + (_color2.a - _color1.a) * _val);

	return result;
}

void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < m_particleList.size(); i++)
	{
		sf::Vector2f initialScale = m_particleList[i].getScale();
		sf::RenderStates nStates = states;
		nStates.transform *= m_particleList[i].getTransform();
		nStates.texture = &m_texture;
		target.draw(m_particleVertex[i].data(), 4, sf::Quads, nStates);
	}
}

#pragma endregion

#pragma region SETTERS

void ParticleSystem::SetStatue(const Statue& _statue)
{
	m_curStatue = _statue;
}

void ParticleSystem::SetTexture(const sf::Texture& _texture)
{
	m_texture = _texture;
}

void ParticleSystem::SetDuration(const float& _duration)
{
	m_duration = _duration;
}

void ParticleSystem::SetRateByTime(const float& _rateByTime)
{
	m_rateByTime = _rateByTime;
}

void ParticleSystem::SetGravityScale(const float& _gravityScale)
{
	m_gravityScale = _gravityScale;
}

void ParticleSystem::SetMaxParticle(const unsigned int& _maxParticle)
{
	m_maxParticle = _maxParticle;
}

void ParticleSystem::SetLoop(const bool& _loop)
{
	m_loop = _loop;
}

void ParticleSystem::SetShape(ps::Shape& _shape)
{
	m_shape = &_shape;
}

#pragma endregion

#pragma region GETTERS

const unsigned int ParticleSystem::GetParticleCount(void) const
{
	return (unsigned int)m_particleList.size();
}

const double& ParticleSystem::GetLifeTimeSystem(void) const
{
	return m_lifeTime;
}

const ParticleSystem::Statue& ParticleSystem::GetStatue(void) const
{
	return m_curStatue;
}

const sf::Texture& ParticleSystem::GetTexture(void) const
{
	return m_texture;
}

const float& ParticleSystem::GetDuration(void) const
{
	return m_duration;
}

const double& ParticleSystem::GetRateByTime(void) const
{
	return m_rateByTime;
}

const float& ParticleSystem::GetGravityScale(void) const
{
	return m_gravityScale;
}

const unsigned int& ParticleSystem::GetMaxParticle(void) const
{
	return m_maxParticle;
}

const bool& ParticleSystem::IsLoop(void) const
{
	return m_loop;
}

const ps::Shape& ParticleSystem::GetShape(void) const
{
	return *m_shape;
}

#pragma endregion

void ParticleSystem::Burst::Reset()
{
	m_cycle = m_saveCycle;
	m_intervalTime = 0;
	m_totalTime = 0;
}
