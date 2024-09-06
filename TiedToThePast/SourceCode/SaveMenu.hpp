#pragma once
#include "State.hpp"
#include "VerticalScrollMenu.hpp"
#include "SaveButton.hpp"
#include "Button.hpp"
#include "SceneManager.hpp"

class InGame;

class SaveMenu : public State
{
public:
	~SaveMenu() override;
	void UpdateSaves(GameData& _gameData);
	void SetInGame(InGame* _inGame);
	void Init(GameData& _gameData) override;
	void Update(GameData& _gameData) override;
	void PollEvent(GameData& _gameData) override;
	void Display(sf::RenderWindow& _window) override;

private:
	constexpr static float BUTTONS_FACTOR_X = 0.9f;
	constexpr static float BUTTONS_FACTOR_Y = 0.95f;

	void InitWidgets(GameData& _gameData);
	void AddButton(GameData& _gameData, const sf::String& _name);
	void AddNewSave(GameData& _gameData);
	void SelectSave(GameData& _gameData, const std::string& _name);
	
	InGame* inGame = nullptr;
	sf::Sprite background;
	VerticalScrollMenu saveList;
	sf::Sprite editSaveBackground;
	VerticalScrollMenu editSaveButtons;
	bool editingSave = false;
	float screenScaleFactor = 0.f;
};
