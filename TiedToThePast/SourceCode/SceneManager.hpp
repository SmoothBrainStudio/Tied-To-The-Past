#ifndef SCENE_MANAGER__HPP
#define SCENE_MANAGER__HPP

#include "Definitions.hpp"
#include "State.hpp"
#include "GameData.hpp"

// STATES
#include "MainMenu.hpp"
#include "PreMenu.hpp"
#include "Introduction.hpp"
#include "SettingsMenu.hpp"
#include "InGame.hpp"
#include "CreditsMenu.hpp"
#include "GameOverMenu.hpp"
#include "WinMenu.hpp"
// LOADING SCREEN
#include "LoadingScreen.hpp"

class SceneManager
{
private:
	SceneManager(void);

	State* nextSceneToLoad = nullptr;
	State* curScene = nullptr;

public:
	~SceneManager(void);
	SceneManager(const SceneManager&) = delete;
	SceneManager(SceneManager&&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;
	SceneManager& operator=(SceneManager&&) = delete;

	static SceneManager& Get(void);

	void ApplyScene(GameData& _gameData);
	void LoadScene(State* _scene);
	State* GetScene(void);
	const bool IsOpen(void);
	void Quit(void);
};

#endif // !SCENE_MANAGER__HPP