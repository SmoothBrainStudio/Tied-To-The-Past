#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include "State.hpp"
#include "AnimatedBackground.hpp"
#include "SettingsMenu.hpp"
#include "DialogBox.hpp"
#include "CreditsMenu.hpp"
#include "SaveMenu.hpp"

static constexpr unsigned char BUTTON_NBR_MAIN_MENU = 5;
static constexpr float HEIGHT_ELS = 108.f;
static constexpr unsigned char SPACING_BUTTON = 16;

class MainMenu: public State
{
public:
	MainMenu() = default;
	~MainMenu() override;

	void Init(GameData& _gameData) override;
	void Update(GameData& _gameData) override;
	void PollEvent(GameData& _gameData) override;
	void Display(sf::RenderWindow& _window) override;

private:
	void InitButtons(GameData& _gameData);

	std::vector<Button*> buttons;

	AnimatedBackground background;

	enum Buttons
	{
		PLAY,
		LOAD,
		SETTINGS,
		QUIT,
		CREDITS_BTN
	};

	char currentButton = 0;
	std::vector<State*> subStates;
	State* subState = nullptr;
	StateEnum gameState;
	SubStates subStateID = SubStates::NULL_STATE;
	Audio& audioManager = Audio::Get();
	bool saveAvailable = false;
	bool isGoingInGame = false;
};

#endif // !MAINMENU_HPP
