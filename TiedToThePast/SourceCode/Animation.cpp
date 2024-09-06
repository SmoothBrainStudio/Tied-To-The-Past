#include "Animation.hpp"
#include "TexturesManager.hpp"
#include "GameClock.hpp"
#include "Maths.hpp"

Animation::Animation() : currentFrame(0), startSprite(0), sizeSprite(0), sizeBySprite(sf::Vector2<unsigned short>(0, 0)), duration(0.f), totalTime(0.f), isLoop(true), isStart(false)
{
}

void Animation::Update()
{
	if (isStart)
	{
		GameClock& timeManager = GameClock::Get();

		totalTime += timeManager.GetDeltaTime();
		totalTime = Maths::Clamp(totalTime, 0.f, duration);

		currentFrame = (unsigned short)((float)startSprite + (totalTime / duration) * (float)sizeSprite);

		if (totalTime >= duration && isLoop)
		{
			totalTime = 0.f;
		}
	}
}

void Animation::Restart()
{
	totalTime = 0.f;
	isStart = true;
}

void Animation::Play()
{
	isStart = true;
}

void Animation::Stop()
{
	isStart = false;
}

bool Animation::IsOver()
{
	return (!isLoop && totalTime >= duration);
}

void Animation::SetTextureSheet(const sf::Texture& _texture)
{
	spriteSheet.setTexture(_texture, true);
}

void Animation::SetStartSprite(const unsigned short& _startSprite)
{
	startSprite = _startSprite;
}

void Animation::SetSizeSprite(const unsigned short& _sizeSprite)
{
	sizeSprite = _sizeSprite;
}

void Animation::SetSizeBySprite(const sf::Vector2<unsigned short>& _sizeBySprite)
{
	sizeBySprite = _sizeBySprite;
}

void Animation::SetDuration(const float& _duration)
{
	duration = _duration;
}

void Animation::SetLoop(const bool& _loop)
{
	isLoop = _loop;
}

void Animation::SetFrame(const unsigned short& _frame)
{
	currentFrame = (unsigned short)Maths::Clamp(_frame, (float)startSprite, (float)(startSprite + sizeSprite));
}

const sf::Sprite& Animation::GetSpriteSheet()
{
	return spriteSheet;
}

const sf::Sprite& Animation::GetCurrentSprite()
{
	result = GetSpriteAtFrame(currentFrame);
	result.setOrigin(0, 0);

	return result;
}

const sf::Sprite& Animation::GetLastSprite()
{
	unsigned short lastFrame = startSprite + sizeSprite;

	return GetSpriteAtFrame(lastFrame);
}

const unsigned short& Animation::GetCurrentFrame()
{
	return currentFrame;
}

const unsigned short& Animation::GetStartSprite()
{
	return startSprite;
}

const unsigned short& Animation::GetSizeSprite()
{
	return sizeSprite;
}

const sf::Vector2<unsigned short>& Animation::GetSizeBySprite()
{
	return sizeBySprite;
}

const float& Animation::GetDuration()
{
	return duration;
}

const bool& Animation::GetLoop()
{
	return isLoop;
}

// Loaders

Animation LoadAnimation(const std::string& _fileName)
{
	TexturesManager& textureManager = TexturesManager::Get();
	Animation result;
	std::ifstream file(_fileName);

	while (!file.eof())
	{
		std::string spriteSheetFile;
		unsigned short startSprite;
		unsigned short sizeSprite;
		sf::Vector2<unsigned short> sizeBySprite;
		float duration;
		bool isLoop;

		file >> spriteSheetFile;
		file >> startSprite;
		file >> sizeSprite;
		file >> sizeBySprite.x;
		file >> sizeBySprite.y;
		file >> duration;
		file >> isLoop;

		result.SetTextureSheet(textureManager[spriteSheetFile]);
		result.SetStartSprite(startSprite);
		result.SetSizeSprite(sizeSprite);
		result.SetSizeBySprite(sizeBySprite);
		result.SetDuration(duration);
		result.SetLoop(isLoop);
	}

	file.close();
	return result;
}

sf::Sprite& Animation::GetSpriteAtFrame(unsigned short frame)
{
	result = spriteSheet;
	sf::IntRect rect = sf::IntRect(result.getGlobalBounds());

	unsigned short left = 0;
	unsigned short top = 0;
	if (rect.width != 0)
	{
		left = (frame * sizeBySprite.x) % (int)rect.width;
		top = (frame * sizeBySprite.x) / (int)rect.width;
	}

	sf::IntRect resultRect = sf::IntRect(left, top, sizeBySprite.x, sizeBySprite.y);
	result.setTextureRect(resultRect);

	return result;
}
