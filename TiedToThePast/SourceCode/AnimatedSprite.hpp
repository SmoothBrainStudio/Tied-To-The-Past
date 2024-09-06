#ifndef ANIMATED_SPRITE__HPP
#define ANIMATED_SPRITE__HPP

#include "Common.hpp"

class AnimatedSprite : public sf::Transformable, public sf::Drawable
{
public:
	// Enum(s) & Struct(s)
	enum class Status
	{
		PLAYING,
		PAUSED,
		STOPPED
	};

private:
	// Enum(s) & Struct(s)
	struct AnimationData
	{
		AnimationData(void);

		sf::Texture* m_texture;
		float m_duration;
	};

	// Private method(s)
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	// Animation member(s)
	sf::Sprite m_sprite;
	float m_lifeTime = 0.f;
	unsigned short m_currentFrame = 0;
	unsigned short m_animationFrameCount = 0;
	float m_total = 0.f;

	// Animation data
	std::string m_name;
	float m_speed;
	bool m_loop;
	sf::Color m_color = sf::Color::White;
	Status m_currentStatue;
	std::vector<AnimationData> animationData;

public:
	// Constructor(s) & Deconstructor(s)
	AnimatedSprite();
	AnimatedSprite(const std::string& _path);
	~AnimatedSprite();

	// Main method(s)
	void LoadFromFile(const std::string& _path);
	void Update(void);
	void Play(void);
	void Pause(void);
	void Stop(void);
	void Reset(void);

	// Setters
	void SetCurrentFrame(const unsigned short& _currentFrame);
	void SetName(const std::string& _name);
	void SetSpeed(const float& _speed);
	void SetLoop(const bool& _loop);
	void SetStatus(const Status& _statue);
	void SetColor(const sf::Color& _color);

	// Getters
	const unsigned short& GetCurrentFrame(void) const;
	const unsigned short& GetFrameCount(void) const;
	const std::string& GetName(void) const;
	const float& GetSpeed(void) const;
	const bool& GetLoop(void) const;
	const Status& GetStatus(void) const;
	sf::Sprite& GetSprite(void);
};

#endif // !ANIMATED_SPRITE__HPP
