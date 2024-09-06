#include "Definitions.hpp"
#include "PauseMenu.hpp"
#include "InGame.hpp"

Pause::~Pause()
{
	while (!items.empty())
	{
		items.back().reset();
		items.pop_back();
	}

	DrawManager::Get().DeleteSection("PAUSE_MENU");

	texturesManager.Delete("RESUME_BUTTON");
	texturesManager.Delete("RESUME_BUTTON_HOVER");
	texturesManager.Delete("SETTINGS_BUTTON");
	texturesManager.Delete("SETTINGS_BUTTON_HOVER");
	texturesManager.Delete("QUIT_BUTTON");
	texturesManager.Delete("QUIT_BUTTON_HOVER");
}

void Pause::Init(GameData& _gameData)
{
	InitTexture(_gameData);
	InitButtons(_gameData);
}

void Pause::InitTexture(GameData& _gameData)
{
	texturesManager.Add("Contents/Assets/Sprites/UI/MenuItems/resume_button.png", "RESUME_BUTTON");
	texturesManager.Add("Contents/Assets/Sprites/UI/MenuItems/resume_button_hover.png", "RESUME_BUTTON_HOVER");
	texturesManager.Add("Contents/Assets/Sprites/UI/MenuItems/settings_button.png", "SETTINGS_BUTTON");
	texturesManager.Add("Contents/Assets/Sprites/UI/MenuItems/settings_button_hover.png", "SETTINGS_BUTTON_HOVER");
	texturesManager.Add("Contents/Assets/Sprites/UI/MenuItems/quit_button.png", "QUIT_BUTTON");
	texturesManager.Add("Contents/Assets/Sprites/UI/MenuItems/quit_button_hover.png", "QUIT_BUTTON_HOVER");
}

void Pause::InitButtons(GameData& _gameData)
{
	// Sections
	drawManager.AddSection("PAUSE_MENU");

	// Item create
	std::unique_ptr<Button> resume = std::make_unique<Button>();
	std::unique_ptr<Button> settings = std::make_unique<Button>();
	std::unique_ptr<Button> quit = std::make_unique<Button>();

	// Add to draw manager
	drawManager.AddObject("PAUSE_MENU", *resume);
	drawManager.AddObject("PAUSE_MENU", *settings);
	drawManager.AddObject("PAUSE_MENU", *quit);

	//
	// resume
	//
	resume->setPosition(sf::Vector2f(16, WORKING_HEIGHT - 372));
	resume->SetTextureSprite(texturesManager["RESUME_BUTTON"]);
	resume->SetID(0);
	resume->SetOnClickFct([this]() -> void {
		audioManager.PlaySoundNoSecurity("BUTTON_PRESSED");
		isDelete = true;
		nextSubState = SubStates::NULL_STATE;
		});
	resume->SetOnOutFct([this]() -> void {
		((Button*)items[0].get())->SetTextureSprite(texturesManager["RESUME_BUTTON"]);
		});
	resume->SetOnOverFct([this]() -> void {
		((Button*)items[0].get())->SetTextureSprite(texturesManager["RESUME_BUTTON_HOVER"]);
		currentId = items[0]->GetID();
		});
	//
	// settings
	//
	settings->setPosition(sf::Vector2f(16, WORKING_HEIGHT - 248));
	settings->SetTextureSprite(texturesManager["SETTINGS_BUTTON"]);
	settings->SetID(1);
	settings->SetOnClickFct([this]() -> void {
		Audio& audioManager = Audio::Get();
	audioManager.PlaySound("BUTTON_PRESSED");
	isDelete = true;
	nextSubState = SubStates::SETTINGS;
		});
	settings->SetOnOutFct([this]() -> void {
		((Button*)items[1].get())->SetTextureSprite(texturesManager["SETTINGS_BUTTON"]);
		});
	settings->SetOnOverFct([this]() -> void {
		((Button*)items[1].get())->SetTextureSprite(texturesManager["SETTINGS_BUTTON_HOVER"]);
		currentId = items[1]->GetID();
		});
	//
	// quit
	//
	quit->setPosition(sf::Vector2f(16, WORKING_HEIGHT - 124));
	quit->SetTextureSprite(texturesManager["QUIT_BUTTON"]);
	quit->SetID(2);
	quit->SetOnClickFct([this]() -> void {
		audioManager.PlaySound("BUTTON_CANCEL");
		audioManager.StopAllMusics();
		audioManager.PlayMusic("SPOOKY_THEME", true);
		SceneManager::Get().LoadScene(new MainMenu);
		});
	quit->SetOnOutFct([this]() -> void {
		((Button*)items[2].get())->SetTextureSprite(texturesManager["QUIT_BUTTON"]);
		});
	quit->SetOnOverFct([this]() -> void {
		((Button*)items[2].get())->SetTextureSprite(texturesManager["QUIT_BUTTON_HOVER"]);
		currentId = items[2]->GetID();
		});

	// Vector of Items
	items.push_back(std::move(resume));
	items.push_back(std::move(settings));
	items.push_back(std::move(quit));
}

void Pause::Update(GameData& _gameData)
{
	for (int i = 0; i < items.size(); i++)
	{
		items[i]->CallOnOut();
	}

	if (ControlsManager::Get().IsPressedDirection("UP"))
	{
		if (--currentId < 0)
		{
			currentId = (int)(items.size() - 1);
		}
		audioManager.PlaySoundNoSecurity("BUTTON_SWAP");
	}
	if (ControlsManager::Get().IsPressedDirection("DOWN"))
	{
		if (++currentId > items.size() - 1)
		{
			currentId = 0;
		}
		audioManager.PlaySoundNoSecurity("BUTTON_SWAP");
	}

	if (!items.empty())
	{
		items[currentId]->CallOnOver();
	}
}

void Pause::PollEvent(GameData& _gameData)
{
	for (int i = 0; i < items.size(); i++)
	{
		items[i]->CallOnOut();
	}

	if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "UP"))
	{
		if (--currentId < 0)
		{
			currentId = (int)(items.size() - 1);
		}
		audioManager.PlaySoundNoSecurity("BUTTON_SWAP");
	}
	if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "DOWN"))
	{
		if (++currentId > items.size() - 1)
		{
			currentId = 0;
		}
		audioManager.PlaySoundNoSecurity("BUTTON_SWAP");
	}

	for (int i = 0; i < items.size(); i++)
	{
		items[i]->PollEvent(_gameData);
	}

	if (!items.empty())
	{
		items[currentId]->CallOnOver();
		if (_gameData.gameEvent.type == sf::Event::KeyPressed && _gameData.gameEvent.key.code == sf::Keyboard::Enter
			|| ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "INTERACT"))
		{
			items[currentId]->CallOnOut();
			items[currentId]->CallOnClick();
			currentId = 0;
		}
	}
}

void Pause::Display(sf::RenderWindow& _window)
{
	_window.setView(_window.getDefaultView());
	DrawManager::Get().Draw(_window, "PAUSE_MENU");
}
