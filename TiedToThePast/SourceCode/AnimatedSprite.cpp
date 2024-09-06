#include "AnimatedSprite.hpp"

#pragma region CONSTRUCTORS_DECONSTRUCTORS

AnimatedSprite::AnimatedSprite()
	: m_lifeTime(0.f), m_total(0.f), m_currentFrame(0)
{
}

AnimatedSprite::AnimatedSprite(const std::string& _path)
	: m_lifeTime(0.f), m_total(0.f), m_currentFrame(0)
{
	LoadFromFile(_path);
}

AnimatedSprite::~AnimatedSprite()
{
}

AnimatedSprite::AnimationData::AnimationData(void)
	: m_duration(0.f), m_texture(nullptr)
{
}

#pragma endregion

#pragma region MAIN_METHODS

void AnimatedSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(m_sprite, states);
}

void AnimatedSprite::LoadFromFile(const std::string& _path)
{
	TexturesManager& textureManager = TexturesManager::Get();
	const Json::Value jsonFile = JsonManager::LoadFromFile(_path);

	m_name = jsonFile["data"]["name"].asString();
	m_speed = jsonFile["data"]["speed"].asFloat();
	m_loop = jsonFile["data"]["loop"].asBool();

	if (jsonFile["data"]["startAwake"].asBool())
	{
		m_currentStatue = Status::PLAYING;
	}
	else
	{
		m_currentStatue = Status::STOPPED;
	}

	// Data animation Load
	animationData.clear();
	m_currentFrame = 0;

	m_animationFrameCount = jsonFile["animation"].size();
	animationData.resize(m_animationFrameCount);

	for (int i = 0; i < m_animationFrameCount; i++)
	{
		animationData[i].m_texture = &textureManager[jsonFile["animation"][i]["texture"].asString()];

		float duration = jsonFile["animation"][i]["duration"].asFloat();
		animationData[i].m_duration = duration;
		m_total += duration;
	}

	m_lifeTime = animationData[m_currentFrame].m_duration;

	m_sprite.setColor(m_color);
	m_sprite.setTexture(*animationData[m_currentFrame].m_texture);
}

void AnimatedSprite::Update(void)
{
	if (m_currentStatue != Status::PLAYING) return;

	// Time Management
	m_lifeTime -= GameClock::Get().GetDeltaTime() * m_speed;
	m_lifeTime = fmod(m_lifeTime, m_total);


	if (m_lifeTime <= 0.f)
	{
		// If end of total duration
		if (++m_currentFrame > m_animationFrameCount - 1)
		{
			if (!m_loop)
			{
				m_lifeTime = animationData[--m_currentFrame].m_duration;
				m_currentStatue = Status::STOPPED;
			}
			else
			{
				m_currentFrame = 0;
			}
		}

		m_lifeTime += animationData[m_currentFrame].m_duration;

		// Change sprite
		m_sprite.setColor(m_color);
		m_sprite.setTexture(*animationData[m_currentFrame].m_texture, true);
	}
}

void AnimatedSprite::Play(void)
{
	m_currentStatue = Status::PLAYING;
}

void AnimatedSprite::Pause(void)
{
	m_currentStatue = Status::PAUSED;
}

void AnimatedSprite::Stop(void)
{
	m_currentStatue = Status::STOPPED;
	Reset();
}

void AnimatedSprite::Reset(void)
{
	m_currentFrame = 0;
	m_lifeTime = animationData[m_currentFrame].m_duration;

	m_sprite.setColor(m_color);
	m_sprite.setTexture(*animationData[m_currentFrame].m_texture, true);
}

#pragma endregion

#pragma region SETTERS

void AnimatedSprite::SetCurrentFrame(const unsigned short& _currentFrame)
{
	m_currentFrame = _currentFrame;
	m_sprite.setColor(m_color);
	m_sprite.setTexture(*animationData[m_currentFrame].m_texture, true);
}

void AnimatedSprite::SetName(const std::string& _name)
{
	m_name = _name;
}

void AnimatedSprite::SetSpeed(const float& _speed)
{
	m_speed = _speed;
}

void AnimatedSprite::SetLoop(const bool& _loop)
{
	m_loop = _loop;
}

void AnimatedSprite::SetStatus(const Status& _statue)
{
	m_currentStatue = _statue;
}

void AnimatedSprite::SetColor(const sf::Color& _color)
{
	m_color = _color;
	m_sprite.setColor(m_color);
	m_sprite.setTexture(*animationData[m_currentFrame].m_texture, true);
}

#pragma endregion

#pragma region GETTERS

const unsigned short& AnimatedSprite::GetCurrentFrame(void) const
{
	return m_currentFrame;
}

const unsigned short& AnimatedSprite::GetFrameCount(void) const
{
	return m_animationFrameCount;
}

const std::string& AnimatedSprite::GetName(void) const
{
	return m_name;
}

const float& AnimatedSprite::GetSpeed(void) const
{
	return m_speed;
}

const bool& AnimatedSprite::GetLoop(void) const
{
	return m_loop;
}

const AnimatedSprite::Status& AnimatedSprite::GetStatus(void) const
{
	return m_currentStatue;
}

sf::Sprite& AnimatedSprite::GetSprite(void)
{
	return m_sprite;
}

#pragma endregion
