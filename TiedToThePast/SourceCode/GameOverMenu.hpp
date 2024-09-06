#ifndef GAME_OVER_MENU__HPP
#define GAME_OVER_MENU__HPP

#include "State.hpp"
#include "DialogBox.hpp"
#include "SceneManager.hpp"

static constexpr unsigned char NB_BUTTONS_GAME_OVER = 2;
static constexpr float HEIGHT_ELS_GAME_OVER = 108.f;
static constexpr unsigned char SPACING_BUTTON_GAME_OVER = 16;
static constexpr float PTIT_BOO_SPEED_LOSE = 250.f;

class GameOverMenu : public State
{
public:
	GameOverMenu();
	~GameOverMenu() override;

	void Init(GameData& _gameData) override;
	void Update(GameData& _gameData) override;
	void PollEvent(GameData& _gameData) override;
	void Display(sf::RenderWindow& _window) override;

private:
	TexturesManager& texturesManager = TexturesManager::Get();
	DrawManager& drawManager = DrawManager::Get();
	FontsManager& fontsManager = FontsManager::Get();
	Audio& audioMng = Audio::Get();

	void InitButtons(GameData& _gameData);

	sf::Vector2f endTitlePos = sf::Vector2f(0.f, 0.f);
	sf::Vector2f startTitlePos = sf::Vector2f(0.f, -400.f);
	sf::Sprite background;
	sf::Sprite title;
	int currentId;
	Button* load;
	Button* quit;
	std::vector<MenuItem*> items;

	float timerTitle = 0.f;
	bool isFading = true;
};

#endif // !GAME_OVER_MENU__HPP



