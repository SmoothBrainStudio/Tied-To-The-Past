#include "Definitions.hpp"
#include "GameManager.hpp"
#include "SaveSystem.hpp"

GameManager::~GameManager(void)
{
	// Fonts
	fntMng.Delete("DEFAULT");
	fntMng.Delete("SPOOKY");
	fntMng.Delete("FIGHTING");

	// Sounds
	audMng.DeleteSoundAndBuffer("WITCH_LAUGH");
	audMng.DeleteSoundAndBuffer("BUTTON_SWAP");
	audMng.DeleteSoundAndBuffer("BUTTON_PRESSED");
	audMng.DeleteSoundAndBuffer("BUTTON_CANCEL");
	audMng.DeleteSoundAndBuffer("CHANGE_TIE");
	audMng.DeleteSoundAndBuffer("SAVE_SUCCESS");
	audMng.DeleteSoundAndBuffer("USE_POTION");
	audMng.DeleteSoundAndBuffer("USE_POWER_POTION");
	audMng.DeleteSoundAndBuffer("USE_ANTIDOTE");
	audMng.DeleteSoundAndBuffer("USE_BOMB");
	audMng.DeleteSoundAndBuffer("PUNCH_WOOSH");
	audMng.DeleteSoundAndBuffer("PUNCH_HIT");
	audMng.DeleteSoundAndBuffer("OPEN_SELLING");
	audMng.DeleteSoundAndBuffer("CLOSE_SELLING");
	audMng.DeleteBuffer("NORMAL_WALK");
	audMng.DeleteBuffer("CANDLE_WALK");
	audMng.DeleteBuffer("IS_LEAKING");
	audMng.DeleteBuffer("LEAK_END");
	audMng.DeleteBuffer("LEAK_END_CANDLE");
	audMng.DeleteBuffer("FIREBALL_LAUNCH");
	audMng.DeleteBuffer("FIREBALL_PATROL");
	audMng.DeleteSoundAndBuffer("DIALOG_SOUND");

	// Musics
	audMng.DeleteMusic("SPOOKY_THEME");
	audMng.DeleteMusic("BOSS_MUSIC");
	audMng.DeleteMusic("FINAL_BOSS_MUSIC");
	audMng.DeleteMusic("EXPLORATION_MUSIC");
	audMng.DeleteMusic("FIGHT_MUSIC");
	audMng.DeleteMusic("GAME_OVER_MUSIC");
	audMng.DeleteMusic("GAME_WIN_MUSIC");

	// Shaders
	ShadersManager::Get().Delete("VOID_OUT");

	// Textures
	texMng.Delete("LEFT_ARROW");
	texMng.Delete("LEFT_ARROW_OVER");
	texMng.Delete("RIGHT_ARROW");
	texMng.Delete("RIGHT_ARROW_OVER");
	texMng.Delete("SAVE_GAME_BUTTON");
	texMng.Delete("SAVE_GAME_BUTTON_HOVER");
	texMng.Delete("LOAD_GAME_BUTTON");
	texMng.Delete("LOAD_GAME_BUTTON_HOVER");
	texMng.Delete("BACK_BUTTON");
	texMng.Delete("BACK_BUTTON_HOVER");
	DeleteFileTexture("Contents/Data/Textures/DialogsTextures.txt");
	texMng.Delete("NIGHT_LAYER_1");
	texMng.Delete("NIGHT_LAYER_2");
	texMng.Delete("NIGHT_LAYER_3");
	texMng.Delete("NIGHT_MOON");
	texMng.Delete("NIGHT_OWL");
	texMng.Delete("NIGHT_CLOUD_1");
	texMng.Delete("NIGHT_CLOUD_2");
	texMng.Delete("NIGHT_CLOUD_3");
	texMng.Delete("NIGHT_CLOUD_4");
	texMng.Delete("NIGHT_CLOUD_5");
	texMng.Delete("INTRO_BIGBOO");
	texMng.Delete("TUTO_BORDER");
	texMng.Delete("INFO_BTN");
	texMng.Delete("CURSOR");
	texMng.Delete("CURSOR_GREY");
	DeleteFileTexture("Contents/Data/Textures/NPCsTextureAnimation.txt");

}

#pragma region INIT FUNCTIONS

void GameManager::InitWindow(GameData& _gameData)
{
	scrMng.Init(_gameData.window);
	_gameData.window.setJoystickThreshold(JOYSTICK_MIN_STEP);
	_gameData.window.setKeyRepeatEnabled(false);

	TexturesManager::Get().Add("Contents/Assets/Sprites/Cursor/Cursor.png", "CURSOR");
	TexturesManager::Get().Add("Contents/Assets/Sprites/Cursor/CursorGrey.png", "CURSOR_GREY");
	_gameData.cursor.loadFromPixels(TexturesManager::Get()["CURSOR"].copyToImage().getPixelsPtr(), TexturesManager::Get()["CURSOR"].getSize(), sf::Vector2u(0, 0));
	_gameData.cursorGrey.loadFromPixels(TexturesManager::Get()["CURSOR_GREY"].copyToImage().getPixelsPtr(), TexturesManager::Get()["CURSOR_GREY"].getSize(), sf::Vector2u(0, 0));
	_gameData.window.setMouseCursor(_gameData.cursorGrey);
	_gameData.window.setMouseCursorVisible(false);

	audMng.Init();
}

void GameManager::InitLoadingScreen(GameData& _gameData)
{
	ldgScr.Init();
	ldgScr.SetWindow(_gameData.window);
}

void GameManager::InitGlobalFonts(void)
{
	fntMng.Add("Contents/Assets/Fonts/arial.ttf", "DEFAULT");
	fntMng.Add("Contents/Assets/Fonts/Spooky.ttf", "SPOOKY");
	fntMng.Add("Contents/Assets/Fonts/Fighting.ttf", "FIGHTING");
}

void GameManager::InitGlobalSounds(void)
{
	// Buttons
	audMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Buttons/WitchLaugh.wav", "WITCH_LAUGH", 80.f);
	audMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Buttons/ButtonSwap.wav", "BUTTON_SWAP", 40.f);
	audMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Buttons/ButtonPressed.wav", "BUTTON_PRESSED", 70.f);
	audMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Buttons/ButtonCancel.wav", "BUTTON_CANCEL", 40.f);
	audMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Buttons/ChangeTie.wav", "CHANGE_TIE", 70.f);
	audMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Buttons/SaveSucces.wav", "SAVE_SUCCESS", 70.f);
	audMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Dialog.wav", "DIALOG_SOUND", 90.f);
	audMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/DialogShop.wav", "DIALOG_SHOP_SOUND", 90.f);
	audMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/PlayerDialog.wav", "DIALOG_PLAYER_SOUND", 90.f);

	// Items
	audMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Items/PotionDrink.wav", "USE_POTION", 90.f);
	audMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Items/PowerPotionDrink.wav", "USE_POWER_POTION", 80.f);
	audMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Items/AntidoteDrink.wav", "USE_ANTIDOTE", 40.f);
	audMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Items/Bomb.wav", "USE_BOMB", 70.f);
	audMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Items/OpenSelling.wav", "OPEN_SELLING");
	audMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Items/CloseSelling.wav", "CLOSE_SELLING");

	// Player general
	audMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Player/PunchWoosh.wav", "PUNCH_WOOSH");
	audMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Player/PunchHit.wav", "PUNCH_HIT", 60.f);
	audMng.AddBuffer("Contents/Assets/Audio/Sounds/Player/FootstepLoop.wav", "NORMAL_WALK");
	audMng.AddBuffer("Contents/Assets/Audio/Sounds/Player/FootstepCandle.wav", "CANDLE_WALK");
	audMng.AddSoundWithBuffer("NORMAL_WALK", "PLAYER_WALK");
	audMng.AddBuffer("Contents/Assets/Audio/Sounds/Player/IsLeaking.wav", "IS_LEAKING");
	audMng.AddBuffer("Contents/Assets/Audio/Sounds/Player/LeakEnd.wav", "LEAK_END");
	audMng.AddBuffer("Contents/Assets/Audio/Sounds/Player/LeakEndCandle.wav", "LEAK_END_CANDLE");
	audMng.AddSoundWithBuffer("IS_LEAKING", "LEAK", 40.f);

	// Fireball
	audMng.AddBuffer("Contents/Assets/Audio/Sounds/Enemies/FireballLaunch.wav", "FIREBALL_LAUNCH");
	audMng.AddBuffer("Contents/Assets/Audio/Sounds/Enemies/FireballPatrol.wav", "FIREBALL_PATROL");
}

void GameManager::InitGlobalMusics(void)
{
	audMng.AddMusic("Contents/Assets/Audio/Musics/Spooky.ogg", "SPOOKY_THEME");
	audMng.AddMusic("Contents/Assets/Audio/Musics/BossChief.ogg", "BOSS_MUSIC");
	audMng.AddMusic("Contents/Assets/Audio/Musics/BossCandle.ogg", "FINAL_BOSS_MUSIC");
	audMng.AddMusic("Contents/Assets/Audio/Musics/Exploration.ogg", "EXPLORATION_MUSIC");
	audMng.AddMusic("Contents/Assets/Audio/Musics/Fight.ogg", "FIGHT_MUSIC");
	audMng.AddMusic("Contents/Assets/Audio/Musics/GameOver.ogg", "GAME_OVER_MUSIC");
	audMng.AddMusic("Contents/Assets/Audio/Musics/GameWin.ogg", "GAME_WIN_MUSIC");
}

void GameManager::InitGlobalShaders(void)
{
	ShadersManager::Get().Add("Contents/Assets/Shaders/VoidOut.frag", "VOID_OUT");
}

void GameManager::InitGlobalTextures(void)
{
	texMng.Add("Contents/Assets/Sprites/UI/MenuItems/arrow_left.png", "LEFT_ARROW");
	texMng.Add("Contents/Assets/Sprites/UI/MenuItems/arrow_left_over.png", "LEFT_ARROW_OVER");
	texMng.Add("Contents/Assets/Sprites/UI/MenuItems/arrow_right.png", "RIGHT_ARROW");
	texMng.Add("Contents/Assets/Sprites/UI/MenuItems/arrow_right_over.png", "RIGHT_ARROW_OVER");
	texMng.Add("Contents/Assets/Sprites/UI/MenuItems/SaveGameButton.png", "SAVE_GAME_BUTTON");
	texMng.Add("Contents/Assets/Sprites/UI/MenuItems/SaveGameButtonHovered.png", "SAVE_GAME_BUTTON_HOVER");
	texMng.Add("Contents/Assets/Sprites/UI/MenuItems/loadGame_button.png", "LOAD_GAME_BUTTON");
	texMng.Add("Contents/Assets/Sprites/UI/MenuItems/loadGame_button_hover.png", "LOAD_GAME_BUTTON_HOVER");
	texMng.Add("Contents/Assets/Sprites/UI/MenuItems/back_button.png", "BACK_BUTTON");
	texMng.Add("Contents/Assets/Sprites/UI/MenuItems/back_button_hover.png", "BACK_BUTTON_HOVER");
	texMng.Add("Contents/Assets/Sprites/UI/MenuItems/tuto_border.png", "TUTO_BORDER");
	texMng.Add("Contents/Assets/Sprites/UI/Tuto/infos.png", "INFO_BTN");

	InitFileTexture("Contents/Data/Textures/DialogsTextures.txt");

	// Background animated
	texMng.Add("Contents/Assets/Sprites/UI/Introduction/Background_With_Lights.png", "NIGHT_LAYER_1");
	texMng.Add("Contents/Assets/Sprites/UI/Introduction/House_Layer.png", "NIGHT_LAYER_2");
	texMng.Add("Contents/Assets/Sprites/UI/Introduction/Front_Layer.png", "NIGHT_LAYER_3");

	texMng.Add("Contents/Assets/Sprites/UI/Introduction/Moon.png", "NIGHT_MOON");
	texMng.Add("Contents/Assets/Sprites/UI/Introduction/Owl.png", "NIGHT_OWL");
	texMng.Add("Contents/Assets/Sprites/UI/Introduction/Clouds_night/Cloud_1.png", "NIGHT_CLOUD_1");
	texMng.Add("Contents/Assets/Sprites/UI/Introduction/Clouds_night/Cloud_2.png", "NIGHT_CLOUD_2");
	texMng.Add("Contents/Assets/Sprites/UI/Introduction/Clouds_night/Cloud_3.png", "NIGHT_CLOUD_3");
	texMng.Add("Contents/Assets/Sprites/UI/Introduction/Clouds_night/Cloud_4.png", "NIGHT_CLOUD_4");
	texMng.Add("Contents/Assets/Sprites/UI/Introduction/Clouds_night/Cloud_5.png", "NIGHT_CLOUD_5");

	texMng.Add("Contents/Assets/Sprites/UI/Introduction/BigBoo.png", "INTRO_BIGBOO");
	InitFileTexture("Contents/Data/Textures/NPCsTextureAnimation.txt");
}

#pragma endregion

void GameManager::Init(GameData& _gameData)
{
	InitWindow(_gameData);
	InitLoadingScreen(_gameData);

	ldgScr.SetLoading(true); // Start Loading screen

	InitGlobalFonts();
	InitGlobalSounds();
	InitGlobalMusics();
	InitGlobalShaders();
	InitGlobalTextures();

	ldgScr.SetLoading(false); // Stop Loading Screen
	scnMng.LoadScene(new PreMenu); // Load start scene
	gcMng.Update(); // Restart clock to avoid fade problems on PreMenu
}

void GameManager::PollEvent(GameData& _gameData)
{
	while (_gameData.window.pollEvent(_gameData.gameEvent))
	{
		if (_gameData.gameEvent.type == sf::Event::Closed)
		{
			_gameData.window.close();
		}

		if (scnMng.IsOpen())
		{
			scnMng.GetScene()->PollEvent(_gameData);
		}
	}
}

void GameManager::Update(GameData& _gameData)
{
	gcMng.Update();
	kbdMng.Update();
	ctrMng.Update();

	if (scnMng.IsOpen())
	{
		scnMng.GetScene()->Update(_gameData);
	}
}

void GameManager::Display(GameData& _gameData)
{
	_gameData.window.clear(sf::Color::Black);

	if (scnMng.IsOpen())
	{
		scnMng.GetScene()->Display(_gameData.window);
	}

	_gameData.window.display();
}

void GameManager::Run(GameData& _gameData)
{
	while (_gameData.window.isOpen())
	{
		scnMng.ApplyScene(_gameData);
		if (!scnMng.IsOpen())
		{
			_gameData.window.close();
		}

		PollEvent(_gameData);
		Update(_gameData);
		Display(_gameData);
	}

	scnMng.Quit();
}
