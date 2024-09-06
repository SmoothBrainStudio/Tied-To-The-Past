#ifndef CREDITS_MENU__HPP
#define CREDITS_MENU__HPP

#include "State.hpp"
#include "SceneManager.hpp"

static constexpr unsigned char SPACING_BUTTON_CREDITS = 16;

class CreditsMenu : public State
{
public:
	CreditsMenu() = default;
	~CreditsMenu() override;

	void Init(GameData& _gameData) override;
	void Update(GameData& _gameData) override;
	void PollEvent(GameData& _gameData) override;
	void Display(sf::RenderWindow& _window) override;

private:
	void InitButton(GameData& _gameData);

	sf::Sprite background;
	std::unique_ptr<Button> quitButton = nullptr;
};

#endif // !CREDITS_MENU__HPP


