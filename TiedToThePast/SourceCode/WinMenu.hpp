#ifndef WIN_MENU__HPP
#define WIN_MENU__HPP

#include "State.hpp"
#include "DialogBox.hpp"
#include "SceneManager.hpp"

static constexpr float PTIT_BOO_SPEED_WIN = 250.f;

class WinMenu : public State
{
public:
	WinMenu();
	~WinMenu() override;

	void Init(GameData& _gameData) override;
	void Update(GameData& _gameData) override;
	void PollEvent(GameData& _gameData) override;
	void Display(sf::RenderWindow& _window) override;

private:

	sf::Sprite background;
	sf::Sprite title;
	sf::Vector2f endTitlePos = sf::Vector2f(0.f, 0.f);
	sf::Vector2f startTitlePos = sf::Vector2f(0.f, -400.f);
	float timerTitle = 0.f;
	Audio& audioMng = Audio::Get();
	bool isFading = true;
};

#endif // !WIN_MENU__HPP



