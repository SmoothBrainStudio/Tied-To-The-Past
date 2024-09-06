#ifndef PAUSE_MENU__HPP
#define PAUSE_MENU__HPP

#include "State.hpp"
#include "SceneManager.hpp"

static constexpr unsigned char NB_BUTTONS_PAUSE = 3;
static constexpr float HEIGHT_ELS_PAUSE = 108.f;
static constexpr unsigned char SPACING_BUTTON_PAUSE = 16;

class InGame;

class Pause : public State
{
public:
	Pause() = default;
	~Pause() override;

	void Init(GameData& _gameData) override;
	void Update(GameData& _gameData) override;
	void PollEvent(GameData& _gameData) override;
	void Display(sf::RenderWindow& _window) override;

private:
	Audio& audioManager = Audio::Get();
	TexturesManager& texturesManager = TexturesManager::Get();
	DrawManager& drawManager = DrawManager::Get();

	void InitTexture(GameData& _gameData);
	void InitButtons(GameData& _gameData);

	int currentId = 0;

	std::vector<std::unique_ptr<MenuItem>> items;
};

#endif // !PAUSE__HPP



