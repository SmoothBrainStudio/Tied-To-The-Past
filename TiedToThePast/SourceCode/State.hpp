#ifndef STATE__HPP
#define STATE__HPP

#include "Common.hpp"
#include "Button.hpp"
#include "Select.hpp"
#include "SelectBox.hpp"
#include "KeyInput.hpp"
#include "Slider.hpp"
#include "Bar.hpp"
#include "Checkbox.hpp"
#include "ImageButton.hpp"
#include "AnimatedSprite.hpp"
#include "Fade.hpp"
#include "GameData.hpp"

enum class StateEnum
{
	QUIT,
	PRE,
	INTRODUCTION,
	MAIN,
	GAME,
	GAME_OVER,
	WIN,
	NONE
};

enum SubStates
{
	SETTINGS,
	PAUSE,
	CHARACTER_MENU,
	SHOP,
	TIE_SELECTOR,
	SAVE_MENU,
	CREDITS,
	NULL_STATE
};

enum Adjust
{
	FILL,
	ADJUST_X,
	ADJUST_Y,
	STRETCH
};

class State
{
public:
	State() = default;
	virtual ~State();

	//virtual functions
	virtual void Init(GameData& _gameData) = 0;
	virtual void Update(GameData& _gameData) = 0;
	virtual void PollEvent(GameData& _gameData) = 0;
	virtual void Display(sf::RenderWindow& _window) = 0;

	bool isDelete = false;
	SubStates nextSubState = SubStates::NULL_STATE;

protected:
	Fade* fade = nullptr;
};

#endif // ! MENU_H
