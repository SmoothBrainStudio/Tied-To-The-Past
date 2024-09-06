#include "Definitions.hpp"
#include "GameOverMenu.hpp"
#include "SaveSystem.hpp"

GameOverMenu::GameOverMenu()
{

}

GameOverMenu::~GameOverMenu()
{
	TexturesManager& texturesManager = TexturesManager::Get();

	delete fade;
	fade = nullptr;

	texturesManager.Delete("QUIT_BUTTON");
	texturesManager.Delete("QUIT_BUTTON_HOVER");
	texturesManager.Delete("LOSE_BG");
	texturesManager.Delete("LOSE_TITLE");

	while (!items.empty())
	{
		delete items.back();
		items.pop_back();
	}

	load = nullptr;
	quit = nullptr;

	DrawManager::Get().DeleteSection("GAME_OVER");
}

void GameOverMenu::Init(GameData& _gameData)
{
	texturesManager.Add("Contents/Assets/Sprites/UI/MenuItems/quit_button.png", "QUIT_BUTTON");
	texturesManager.Add("Contents/Assets/Sprites/UI/MenuItems/quit_button_hover.png", "QUIT_BUTTON_HOVER");
	texturesManager.Add("Contents/Assets/Sprites/UI/Backgrounds/GameLose.png", "LOSE_BG");
	texturesManager.Add("Contents/Assets/Sprites/UI/Backgrounds/GameLoseTitle.png", "LOSE_TITLE");

	drawManager.AddSection("GAME_OVER");

	currentId = 0;

	background.setTexture(texturesManager["LOSE_BG"]);
	drawManager.AddObject("GAME_OVER", background);

	title.setTexture(texturesManager["LOSE_TITLE"]);
	drawManager.AddObject("GAME_OVER", title);
	title.setPosition(startTitlePos);

	InitButtons(_gameData);

	isFading = true;
	fade = new Fade(2.f, 85.f, false, true);
	audioMng.PlayMusic("GAME_OVER_MUSIC", true, 0.f);
	drawManager.AddObject("GAME_OVER", *fade);
}

void GameOverMenu::InitButtons(GameData& _gameData)
{
	// Section

	// Item create
	load = new Button;
	quit = new Button;

	// Vector of commonItems
	items.push_back(load);
	items.push_back(quit);

	// Add to draw manager
	drawManager.AddObject("GAME_OVER", *load);
	drawManager.AddObject("GAME_OVER", *quit);

	//
	// load
	//
	load->setPosition(sf::Vector2f(16, WORKING_HEIGHT - 248));
	load->SetTextureSprite(texturesManager["LOAD_GAME_BUTTON"]);
	load->SetID(0);
	load->SetOnClickFct([this]() -> void {
		Audio& audioManager = Audio::Get();
		SceneManager::Get().LoadScene(new InGame);
		SaveSystem::Get().SelectLastSave();

		audioManager.PlaySound("BUTTON_PRESSED");
		audioManager.StopAllMusics();
		audioManager.PlayMusic("EXPLORATION_MUSIC", true);
		});
	load->SetOnOutFct([this]() -> void {
		load->SetTextureSprite(texturesManager["LOAD_GAME_BUTTON"]);
		});
	load->SetOnOverFct([this]() -> void {
		load->SetTextureSprite(texturesManager["LOAD_GAME_BUTTON_HOVER"]);
		currentId = load->GetID();
		});
	//
	// quit
	//
	quit->setPosition(sf::Vector2f(16, WORKING_HEIGHT - 124));
	quit->SetTextureSprite(texturesManager["QUIT_BUTTON"]);
	quit->SetID(1);
	quit->SetOnClickFct([this]() -> void {
		Audio& audioManager = Audio::Get();
		SceneManager::Get().LoadScene(new MainMenu);

		audioManager.PlaySound("BUTTON_CANCEL");
		audioManager.StopAllMusics();
		audioManager.PlayMusic("SPOOKY_THEME", true);
		});
	quit->SetOnOutFct([this]() -> void {
		quit->SetTextureSprite(texturesManager["QUIT_BUTTON"]);
		});
	quit->SetOnOverFct([this]() -> void {
		quit->SetTextureSprite(texturesManager["QUIT_BUTTON_HOVER"]);
		currentId = quit->GetID();
		});
}


void GameOverMenu::Update(GameData& _gameData)
{
	title.setPosition(Easings::Lerp(startTitlePos, endTitlePos, std::min(Easings::OutBounce(timerTitle), 1.f)));

	timerTitle += GameClock::Get().GetDeltaTime() / 3.f;

	for (int i = 0; i < items.size(); i++)
	{
		items[i]->CallOnOut();
	}

	if (ControlsManager::Get().IsPressedDirection("UP"))
	{
		--currentId;

		if (currentId < 0)
		{
			currentId = (int)(items.size() - 1);
		}

		Audio::Get().PlaySoundNoSecurity("BUTTON_SWAP");
	}
	if (ControlsManager::Get().IsPressedDirection("DOWN"))
	{
		++currentId;

		if (currentId > items.size() - 1)
		{
			currentId = 0;
		}

		Audio::Get().PlaySoundNoSecurity("BUTTON_SWAP");
	}

	if (!items.empty())
	{
		items[currentId]->CallOnOver();
	}

	if (isFading)
	{
		fade->Update(GameClock::Get().GetDeltaTime(), "GAME_OVER_MUSIC");
		if (fade->GetTimer() >= fade->GetTimerMax())
		{
			isFading = false;
		}
	}
}

void GameOverMenu::PollEvent(GameData& _gameData)
{
	for (auto& it : items)
	{
		it->CallOnOut();
	}

	if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "UP"))
	{
		if (--currentId < 0)
		{
			currentId = (int)(items.size() - 1);
		}
		Audio::Get().PlaySoundNoSecurity("BUTTON_SWAP");
	}
	if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "DOWN"))
	{
		if (++currentId > items.size() - 1)
		{
			currentId = 0;
		}
		Audio::Get().PlaySoundNoSecurity("BUTTON_SWAP");
	}

	for (auto& it : items)
	{
		it->PollEvent(_gameData);
	}

	if (!items.empty())
	{
		items[currentId]->CallOnOver();
		if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "INTERACT")
			|| ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "INTERACT_2"))
		{
			items[currentId]->CallOnOut();
			items[currentId]->CallOnClick();
		}
	}
}

void GameOverMenu::Display(sf::RenderWindow& _window)
{
	_window.setView(_window.getDefaultView());
	_window.setMouseCursorVisible(ControlsManager::Get().GetControllerType() == KEYBOARD);
	DrawManager::Get().Draw(_window, "GAME_OVER");
}
