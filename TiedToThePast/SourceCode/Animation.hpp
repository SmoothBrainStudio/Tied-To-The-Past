#ifndef ANIMATION__HPP
#define ANIMATION__HPP

#include "Definitions.hpp"


class Animation
{
private:
	sf::Sprite& GetSpriteAtFrame(unsigned short frame);

	sf::Sprite spriteSheet;
	sf::Sprite result;
	unsigned short startSprite;
	unsigned short sizeSprite;
	sf::Vector2<unsigned short> sizeBySprite;
	unsigned short currentFrame;

	float duration;
	float totalTime;
	bool isLoop;
	bool isStart;

public:
	Animation();
	~Animation() = default;

	void Update();
	void Restart();
	void Play();
	void Stop();
	bool IsOver();

	void SetTextureSheet(const sf::Texture& _texture);
	void SetStartSprite(const unsigned short& _startSprite);
	void SetSizeSprite(const unsigned short& _sizeSprite);
	void SetSizeBySprite(const sf::Vector2<unsigned short>& _sizeBySprite);
	void SetDuration(const float& _duration);
	void SetLoop(const bool& _loop);
	void SetFrame(const unsigned short& _frame);

	const sf::Sprite& GetSpriteSheet();
	const sf::Sprite& GetCurrentSprite();
	const sf::Sprite& GetLastSprite();
	const unsigned short& GetCurrentFrame();
	const unsigned short& GetStartSprite();
	const unsigned short& GetSizeSprite();
	const sf::Vector2<unsigned short>& GetSizeBySprite();
	const float& GetDuration();
	const bool& GetLoop();

};

Animation LoadAnimation(const std::string& _fileName);

#endif // !ANIMATION__HPP
