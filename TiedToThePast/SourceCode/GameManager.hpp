#ifndef GAME_MANAGER__HPP
#define GAME_MANAGER__HPP

#include "Common.hpp"
#include "GameData.hpp"
#include "State.hpp"
#include "SceneManager.hpp"

class GameManager
{
public:
	GameManager(void) = default;
	virtual ~GameManager(void);

	void Init(GameData& _gameData);
	void Update(GameData& _gameData);
	void PollEvent(GameData& _gameData);
	void Display(GameData& _gameData);
	void Run(GameData& _gameData);

private:
	Audio& audMng = Audio::Get();
	Screen& scrMng = Screen::Get();
	Keyboard& kbdMng = Keyboard::Get();
	GameClock& gcMng = GameClock::Get();
	SceneManager& scnMng = SceneManager::Get();
	FontsManager& fntMng = FontsManager::Get();
	LoadingScreen& ldgScr = LoadingScreen::Get();
	ShadersManager& sdrMng = ShadersManager::Get();
	TexturesManager& texMng = TexturesManager::Get();
	ControlsManager& ctrMng = ControlsManager::Get();

	void InitWindow(GameData& _gameData);
	void InitLoadingScreen(GameData& _gameData);
	void InitGlobalFonts(void);
	void InitGlobalSounds(void);
	void InitGlobalMusics(void);
	void InitGlobalShaders(void);
	void InitGlobalTextures(void);
};

#endif // !GAME_MANAGER__HPP
