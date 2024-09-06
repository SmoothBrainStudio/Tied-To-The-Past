#include "Definitions.hpp"
#include "SceneManager.hpp"

SceneManager::SceneManager(void) : nextSceneToLoad(nullptr), curScene(nullptr)
{

}

SceneManager::~SceneManager(void)
{
	if (nextSceneToLoad != nullptr)
	{
		delete nextSceneToLoad;
		nextSceneToLoad = nullptr;
	}

	delete curScene;
	curScene = nullptr;
}

SceneManager& SceneManager::Get(void)
{
	static SceneManager instance;
	return instance;
}

void SceneManager::ApplyScene(GameData& _gameData)
{
	if (nextSceneToLoad == nullptr) return;

	// Delete previous state if exist
	if (curScene != nullptr) delete curScene;

	// Load new scene
	_gameData.window.setMouseCursor(_gameData.cursorGrey);
	LoadingScreen::Get().SetLoading(true);
	curScene = std::move(nextSceneToLoad);
	nextSceneToLoad = nullptr;
	curScene->Init(_gameData);
	LoadingScreen::Get().SetLoading(false);
}

void SceneManager::LoadScene(State* _scene)
{
	// error throw
	if (_scene == nullptr)
	{
		Debug.error("The Scene you tried to open is nullptr");
		throw;
	}
	if (nextSceneToLoad != nullptr)
	{
		Debug.warning("Two scene are trying to load at the same time : delete last next scene");
		delete nextSceneToLoad;
		nextSceneToLoad = nullptr;
	}

	nextSceneToLoad = _scene;
}

State* SceneManager::GetScene(void)
{
	return curScene;
}

const bool SceneManager::IsOpen(void)
{
	return (curScene != nullptr);
}

void SceneManager::Quit(void)
{
	delete curScene;
	curScene = nullptr;
}
