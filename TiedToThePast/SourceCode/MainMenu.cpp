#include "Definitions.hpp"
#include "MainMenu.hpp"
#include "SaveSystem.hpp"

MainMenu::~MainMenu()
{
	TexturesManager& texturesManager = TexturesManager::Get();

	DrawManager::Get().DeleteSection("MAIN_MENU_TITLE");

	while (!buttons.empty())
	{
		delete buttons.back();
		buttons.pop_back();
	}

	while (!subStates.empty())
	{
		delete subStates.back();
		subStates.pop_back();
	}

	texturesManager.Delete("NEW_GAME_BUTTON");
	texturesManager.Delete("NEW_GAME_BUTTON_HOVER");
	texturesManager.Delete("SETTINGS_BUTTON");
	texturesManager.Delete("SETTINGS_BUTTON_HOVER");
	texturesManager.Delete("QUIT_BUTTON");
	texturesManager.Delete("QUIT_BUTTON_HOVER");
	texturesManager.Delete("CREDITS_BUTTON");
	texturesManager.Delete("CREDITS_BUTTON_HOVER");

	EventManager::Get().Delete("FADE_MENU");
}

void MainMenu::Init(GameData& _gameData)
{
	TexturesManager& texturesManager = TexturesManager::Get();
	DrawManager& drawManager = DrawManager::Get();

	// New game button
	texturesManager.Add("Contents/Assets/Sprites/UI/MenuItems/newGame_button.png", "NEW_GAME_BUTTON");
	texturesManager.Add("Contents/Assets/Sprites/UI/MenuItems/newGame_button_hover.png", "NEW_GAME_BUTTON_HOVER");
	// Settings button
	texturesManager.Add("Contents/Assets/Sprites/UI/MenuItems/settings_button.png", "SETTINGS_BUTTON");
	texturesManager.Add("Contents/Assets/Sprites/UI/MenuItems/settings_button_hover.png", "SETTINGS_BUTTON_HOVER");
	// Quit button
	texturesManager.Add("Contents/Assets/Sprites/UI/MenuItems/quit_button.png", "QUIT_BUTTON");
	texturesManager.Add("Contents/Assets/Sprites/UI/MenuItems/quit_button_hover.png", "QUIT_BUTTON_HOVER");
	// Credit button
	texturesManager.Add("Contents/Assets/Sprites/UI/MenuItems/credits_button.png", "CREDITS_BUTTON");
	texturesManager.Add("Contents/Assets/Sprites/UI/MenuItems/credits_button_hover.png", "CREDITS_BUTTON_HOVER");

	drawManager.AddSection("MAIN_MENU_TITLE");

	subStates.push_back(new SettingsMenu());
	subStates.push_back(new CreditsMenu());
	subStates.push_back(new SaveMenu());
	subStateID = SubStates::SETTINGS;

	for (auto& elem : subStates)
	{
		elem->Init(_gameData);
	}

	background.Init();
	drawManager.AddObject("MAIN_MENU_TITLE", background);

	saveAvailable = !SaveSystem::SavesEmpty();

	InitButtons(_gameData);

	fade = new Fade(2.f, 100.f, true, false);

	drawManager.AddObject("MAIN_MENU_TITLE", *fade);

	auto fadeMenu = [&](void) -> void { isGoingInGame = true; };

	EventManager::Get().Add("FADE_MENU", fadeMenu);
}

void MainMenu::Update(GameData& _gameData)
{
	background.Update();

	if (subState != nullptr)
	{
		subState->Update(_gameData);

		if (subState == subStates[2])
		{
			// Save menu state
			if (!SaveSystem::Get().GetSaveName().empty())
			{
				// Go to in game state once a boolNeeded is selected
				subState->isDelete = true;
				isDelete = true;
			}
		}

		if (subState->isDelete)
		{
			subState->isDelete = false;
			subState = nullptr;
		}
	}
	else if (!isGoingInGame)
	{
		for (int i = 0; i < buttons.size(); i++)
		{
			buttons[i]->CallOnOut();
		}

		if (ControlsManager::Get().IsPressedDirection("UP"))
		{
			--currentButton;

			if (currentButton == LOAD && !saveAvailable)
			{
				--currentButton;
			}

			if (currentButton < 0)
			{
				currentButton = (int)(buttons.size() - 1);
			}

			audioManager.PlaySoundNoSecurity("BUTTON_SWAP");
		}
		if (ControlsManager::Get().IsPressedDirection("DOWN"))
		{
			++currentButton;

			if (currentButton == LOAD && !saveAvailable)
			{
				++currentButton;
			}

			if (currentButton > buttons.size() - 1)
			{
				currentButton = 0;
			}

			audioManager.PlaySoundNoSecurity("BUTTON_SWAP");
		}

		if (!buttons.empty())
		{
			buttons[currentButton]->CallOnOver();
		}

		if (ControlsManager::Get().IsPressedDirection("RIGHT") && currentButton == QUIT)
		{
			++currentButton;
			audioManager.PlaySoundNoSecurity("BUTTON_SWAP");
		}

		if (ControlsManager::Get().IsPressedDirection("LEFT") && currentButton == CREDITS_BTN)
		{
			--currentButton;
			audioManager.PlaySoundNoSecurity("BUTTON_SWAP");
		}
	}

	if (!saveAvailable)
	{
		buttons[LOAD]->SetColor(sf::Color(255, 255, 255, 125));
	}
	else
	{
		buttons[LOAD]->SetColor(sf::Color(255, 255, 255, 200));
	}

	if (isGoingInGame && fade != nullptr)
	{
		fade->Update(GameClock::Get().GetDeltaTime(), "SPOOKY_THEME");
		if (fade->GetTimer() >= fade->GetTimerMax())
		{
			SceneManager::Get().LoadScene(new InGame);
		}
	}
}

void MainMenu::PollEvent(GameData& _gameData)
{
	if (subState != nullptr)
	{
		subState->PollEvent(_gameData);
	}
	else if (!isGoingInGame)
	{
		for (int i = 0; i < buttons.size(); i++)
		{
			buttons[i]->CallOnOut();
		}

		if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "UP"))
		{
			--currentButton;

			if (currentButton == LOAD && !saveAvailable)
			{
				--currentButton;
			}

			if (currentButton < 0)
			{
				currentButton = (int)(buttons.size() - 1);
			}

			audioManager.PlaySoundNoSecurity("BUTTON_SWAP");
		}
		if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "DOWN"))
		{
			++currentButton;

			if (currentButton == LOAD && !saveAvailable)
			{
				++currentButton;
			}

			if (currentButton > buttons.size() - 1)
			{
				currentButton = 0;
			}

			audioManager.PlaySoundNoSecurity("BUTTON_SWAP");
		}

		if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "RIGHT") && currentButton == QUIT)
		{
			++currentButton;
			audioManager.PlaySoundNoSecurity("BUTTON_SWAP");
		}

		if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "LEFT") && currentButton == CREDITS_BTN)
		{
			--currentButton;
			audioManager.PlaySoundNoSecurity("BUTTON_SWAP");
		}

		for (int i = 0; i < buttons.size(); i++)
		{
			if (!(i == LOAD && !saveAvailable))
			{
				buttons[i]->PollEvent(_gameData);
			}
		}

		if (!buttons.empty())
		{
			buttons[currentButton]->CallOnOver();
			if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "INTERACT_2")
				|| ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "INTERACT"))
			{
				buttons[currentButton]->CallOnOut();
				buttons[currentButton]->CallOnClick();
				//currentButton = 0;
			}
		}

	}
}

void MainMenu::Display(sf::RenderWindow& _window)
{
	_window.setView(_window.getDefaultView());
	_window.setMouseCursorVisible(ControlsManager::Get().GetControllerType() == KEYBOARD);
	DrawManager::Get().Draw(_window, "MAIN_MENU_TITLE");
	if (subState != nullptr)
	{
		subState->Display(_window);
	}
}

void MainMenu::InitButtons(GameData& _gameData)
{
	TexturesManager& texturesManager = TexturesManager::Get();
	DrawManager& drawManager = DrawManager::Get();

	// Buttons
	buttons.resize(BUTTON_NBR_MAIN_MENU);

	std::array<std::string, BUTTON_NBR_MAIN_MENU> texts =
	{
		"NEW_GAME_BUTTON",
		"LOAD_GAME_BUTTON",
		"SETTINGS_BUTTON",
		"QUIT_BUTTON",
		"CREDITS_BUTTON"
	};

	for (short int i = BUTTON_NBR_MAIN_MENU - 2; i >= 0; i--)
	{
		sf::Vector2f pos = sf::Vector2f(SPACING_BUTTON, WORKING_HEIGHT - (HEIGHT_ELS + SPACING_BUTTON) * ((BUTTON_NBR_MAIN_MENU - 1) - i));

		buttons[i] = new Button(pos, texturesManager[texts[i]]);
	}

	buttons[CREDITS_BTN] = new Button(
		sf::Vector2f((float)(WORKING_WIDTH - texturesManager["LOAD_GAME_BUTTON"].getSize().x - SPACING_BUTTON),
			(float)(WORKING_HEIGHT - texturesManager["LOAD_GAME_BUTTON"].getSize().y - SPACING_BUTTON)),
		texturesManager[texts[CREDITS_BTN]]);

	// Edit function(s) btn //
	buttons[PLAY]->SetOnClickFct([this, &_gameData] // Game
		{
			Audio& audioManager = Audio::Get();

			audioManager.PlaySound("WITCH_LAUGH");
			isGoingInGame = true;
		});

	buttons[LOAD]->SetOnClickFct([this, &_gameData] // Load
		{
			Audio& audioManager = Audio::Get();
			audioManager.PlaySound("BUTTON_PRESSED");

			subState = subStates[2];
			((SaveMenu*)subState)->UpdateSaves(_gameData);
		});

	buttons[SETTINGS]->SetOnClickFct([this] // Settings
		{
			Audio& audioManager = Audio::Get();
			audioManager.PlaySound("BUTTON_PRESSED");
			subState = subStates[0];
		});

	buttons[QUIT]->SetOnClickFct([this] // Close
		{
			Audio& audioManager = Audio::Get();
			audioManager.PlaySound("BUTTON_CANCEL");

			SceneManager::Get().Quit();
		});

	buttons[CREDITS_BTN]->SetOnClickFct([this] // Credits
		{
			Audio& audioManager = Audio::Get();
			audioManager.PlaySound("BUTTON_PRESSED");
			subState = subStates[1];
		});

	for (unsigned short int i = 0; i < buttons.size(); i++)
	{
		buttons[i]->SetOnOverFct([&, this, i, texts]
			{
				currentButton = (char)i;
				buttons[i]->SetTextureSprite(texturesManager[texts[i] + "_HOVER"]);
			});
	}

	for (unsigned short int i = 0; i < buttons.size(); i++)
	{
		buttons[i]->SetOnOutFct([&, this, i, texts]
			{
				buttons[i]->SetTextureSprite(texturesManager[texts[i]]);
			});
	}
	//////////////////////////

	for (auto& it : buttons)
	{
		DrawManager::Get().AddObject("MAIN_MENU_TITLE", *it);
	}



	for (int i = 0; i < BUTTON_NBR_MAIN_MENU; i++)
	{
		buttons[i]->GetSprite().setColor(sf::Color(255, 255, 255, 200));
	}
}
