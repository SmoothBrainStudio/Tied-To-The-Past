#ifndef INTRODUCTION_HPP
#define INTRODUCTION_HPP

#include "State.hpp"
#include "AnimatedBackground.hpp"
#include "SceneManager.hpp"

#define POSITION_CLOUD_1 sf::Vector2f(1211, 685)
#define POSITION_CLOUD_2 sf::Vector2f(647, 298)
#define POSITION_CLOUD_3 sf::Vector2f(515, 399)
#define POSITION_CLOUD_4 sf::Vector2f(134, 699)
#define POSITION_CLOUD_5 sf::Vector2f(187, 138)
#define POSITION_CLOUD_6 sf::Vector2f(1054, 591)
#define POSITION_CLOUD_7 sf::Vector2f(1353, 303)
#define POSITION_CLOUD_8 sf::Vector2f(489, 950)

class Introduction : public State
{
private:
	// Manager(s)
	TexturesManager& texMng = TexturesManager::Get();
	ShadersManager& shdMng = ShadersManager::Get();
	DrawManager& drwMng = DrawManager::Get();
	GameClock& glMng = GameClock::Get();
	Screen& scrMng = Screen::Get();
	Audio& audMng = Audio::Get();

	// Constants
	static constexpr float DURATION_DEFAULT_INTRO = 10.f;
	static constexpr float DURATION_FLASH = 0.075f;
	static constexpr float DURATION_ENTITIES_MOVEMENTS = 16.f;
	static constexpr float WAIT_LILBOO_MIN = 3.f;
	static constexpr float WAIT_LILBOO_MAX = 10.f;

	// Enum(s) and Struct(s)
	enum class IntroductionState
	{
		DAY,
		THUNDER,
		NIGHT
	};

	enum class ThunderState
	{
		THUNDER_ANIM,
		THUNDER1,
		THUNDER2
	};

	// Private function(s)
	void InitTextures();
	void InitAudios();
	void InitShaders();

	void DayUpdate();
	void ThunderUpdate();
	void NightUpdate();

	void NextToThunder();
	void NextToNight();

	// Member(s)
	void (Introduction::*curUpdate)() = nullptr;
	float totalTime;
	float entitiesTime;
	float waitTime;

	sf::Sprite dayBackground;
	sf::Sprite colorBackground;
	AnimatedBackground nightBackground;

	sf::Texture textureColor;
	sf::Image blackColor;
	sf::Image whiteColor;

	sf::Sprite title;
	sf::Sprite press;
	AnimatedSprite lightning;

	IntroductionState introductionState;
	ThunderState thunderState;

	std::array<sf::Sprite, 8> dayClouds;

public:
	Introduction() = default;
	~Introduction() override;

	void Init(GameData& _gameData) override;
	void Update(GameData& _gameData) override;
	void PollEvent(GameData& _gameData) override;
	void Display(sf::RenderWindow& _window) override;

};

#endif // !INTRODUCTION_HPP
