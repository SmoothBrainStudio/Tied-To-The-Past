#ifndef ANIMATED_BACKGROUND__HPP
#define ANIMATED_BACKGROUND__HPP

#include "Common.hpp"

#define MOON_POSITION sf::Vector2f(95.f, 60.f)
#define OWL_POSITION sf::Vector2f(1795.f, 669.f)
#define CLOUD_1_POSITION sf::Vector2f(1353.f, 103.f)
#define CLOUD_2_POSITION sf::Vector2f(65.f, 168.f)
#define CLOUD_3_POSITION sf::Vector2f(776.f, 77.f)
#define CLOUD_4_POSITION sf::Vector2f(1187.f, 183.f)
#define CLOUD_5_POSITION sf::Vector2f(573.f, 212.f)
#define LILBOO_POSITION_1 sf::Vector2f(240.f, 790.f)
#define LILBOO_POSITION_2 sf::Vector2f(1500.f, 790.f)
#define BIGBOO_POSITION_1 sf::Vector2f(922.f, 320.f)
#define BIGBOO_POSITION_2 sf::Vector2f(922.f, 260.f)

class AnimatedBackground : public sf::Drawable, public sf::Transformable
{
private:
	// Managers
	TexturesManager& texMng = TexturesManager::Get();
	DrawManager& drwMng = DrawManager::Get();
	GameClock& glMng = GameClock::Get();
	Screen& scnMng = Screen::Get();

	// Private function(s)
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	// Constante(s)
	static constexpr float DURATION_LILBOO_MOVEMENTS = 16.f;
	static constexpr float DURATION_BIGBOO_MOVEMENTS = 6.f;
	static constexpr float WAIT_LILBOO_MIN = 3.f;
	static constexpr float WAIT_LILBOO_MAX = 10.f;
	static constexpr float WAIT_OWL_MIN = 1.5f;
	static constexpr float WAIT_OWL_MAX = 3.f;

	// Member(s)
	float totalTime = 0.f;
	float entitiesTime = 0.f;
	float waitTimeLilboo = 0.f;
	float waitTimeOwl = 0.f;

	sf::Sprite layerBackground;
	sf::Sprite layerMansion;
	sf::Sprite layerFront;

	sf::Sprite moon;
	std::array<sf::Sprite, 5> clouds;
	AnimatedSprite owl;

	AnimatedSprite lilBoo;
	sf::Sprite bigBoo;

public:
	AnimatedBackground() = default;
	~AnimatedBackground() override = default;

	void Init();
	void Update();

};

#endif // !ANIMATED_BACKGROUND__HPP
