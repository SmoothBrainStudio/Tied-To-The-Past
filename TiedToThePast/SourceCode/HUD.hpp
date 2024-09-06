#ifndef HUD__HPP
#define HUD__HPP

#include "Common.hpp"
#include "Player.hpp"
#include "Bar.hpp"
#include "ImageButton.hpp"

static constexpr unsigned short int UI_OFFSET = 30;
static constexpr unsigned char NB_BARS = 2;
static constexpr float TIMER_FADE_MAX = 1.f;
static constexpr float TIMER_WAITING_MAX = 2.f;

class HUD
{
public:
	enum Instruction
	{
		NONE,
		OPEN_INVENTORY,
		OPEN_CIRCULAR_MENU
	};

	HUD();
	~HUD();

	void Init(void);
	void SetInputsTextures();
	void PollEvent(GameData& _gameData);
	void Update(Player& _player);
	void Display(sf::RenderWindow& _window);

private:

	enum BarTypes
	{
		HP,
		MP
	};

	std::array<std::unique_ptr<Bar>, NB_BARS> bars;
	sf::Sprite inventoryButton;
	sf::Sprite circularMenuButton;
	sf::Sprite inventoryKeySprite;
	sf::Sprite circularMenuKeySprite;
	sf::Sprite sweets;
	sf::Text nbSweets;

	Timer sweetTimerAnimation;

	float timerFade = 0.f;
	float timerWaiting = 2.1f;
	sf::Uint8 alpha = 255; //remettre à 0 si vous voulez remodifier le fade des touches
	bool isFadingIn = false;
	bool isFadingOut = false;
	PlayerState lastPlayerState;
	bool changedState = true;
	bool fadeDone = false;
};

#endif // !HUD__HPP



