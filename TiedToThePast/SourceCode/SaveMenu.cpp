#include "Definitions.hpp"
#include "SaveMenu.hpp"
#include "SaveSystem.hpp"

SaveMenu::~SaveMenu()
{
	inGame = nullptr;
	DrawManager::Get().DeleteSection("SAVE_MENU");
	TexturesManager::Get().Delete("SAVE_BACKGROUND");
	TexturesManager::Get().Delete("HOURGLASS");
	TexturesManager::Get().Delete("TRASH_CAN_CLOSED");
	TexturesManager::Get().Delete("TRASH_CAN_OPENED");
}

void SaveMenu::UpdateSaves(GameData& _gameData)
{
	this->saveList.Clear();
	std::vector<std::string> saveNames = SaveSystem::Get().GetAllSaveNames();
	for (std::string& saveName : saveNames)
	{
		AddButton(_gameData, saveName);
	}

	if (this->inGame)
	{
		// No new boolNeeded button if on main menu
		float buttonHeight = this->saveList.GetRect().height / 3.f;
		SaveButton* button = new SaveButton({ this->saveList.GetRect().width, buttonHeight }, "New save");
		button->setPosition(0.f, this->saveList.GetNbItems() * buttonHeight);
		button->SetOnOverFct(std::bind(&SaveButton::SetSelected, button, true));
		button->SetOnOutFct(std::bind(&SaveButton::SetSelected, button, false));
		button->SetOnClickFct(std::bind(&SaveMenu::AddNewSave, this, std::ref(_gameData)));
		this->saveList.AddItem(std::move(button));
	}
}

void SaveMenu::SetInGame(InGame* _inGame)
{
	this->inGame = _inGame;
}

void SaveMenu::Init(GameData& _gameData)
{
	// Save list

	TexturesManager::Get().Add("Contents/Assets/Sprites/UI/Save/Background.png", "SAVE_BACKGROUND");
	TexturesManager::Get().Add("Contents/Assets/Sprites/UI/Save/Hourglass.png", "HOURGLASS");
	TexturesManager::Get().Add("Contents/Assets/Sprites/UI/Save/TrashCanClosed.png", "TRASH_CAN_CLOSED");
	TexturesManager::Get().Add("Contents/Assets/Sprites/UI/Save/TrashCanOpened.png", "TRASH_CAN_OPENED");

	this->InitWidgets(_gameData);

	DrawManager::Get().AddSection("SAVE_MENU");
	DrawManager::Get().AddObject("SAVE_MENU", this->background);
	DrawManager::Get().AddObject("SAVE_MENU", this->saveList);
	DrawManager::Get().AddObject("SAVE_MENU", this->editSaveBackground);
	DrawManager::Get().AddObject("SAVE_MENU", this->editSaveButtons);
}

void SaveMenu::Update(GameData& _gameData)
{
	if (this->screenScaleFactor != Screen::Get().GetScaleFactor())
	{
		this->screenScaleFactor = Screen::Get().GetScaleFactor();
		this->InitWidgets(_gameData);
	}

	if (this->editingSave)
	{
		this->editSaveButtons.Update(_gameData);
	}
	else
	{
		this->saveList.Update(_gameData);
	}

	if (this->inGame)
	{
		this->inGame->inGameDatas.player->AddHP(10000);
	}
}

void SaveMenu::PollEvent(GameData& _gameData)
{
	if (_gameData.gameEvent.type == sf::Event::KeyReleased
		|| _gameData.gameEvent.type == sf::Event::TextEntered)
	{
		return;
	}

	ControlsManager& controlsMng = ControlsManager::Get();
	if (this->editingSave)
	{
		if (controlsMng.IsPressedInput(_gameData.gameEvent, "CANCEL") || controlsMng.IsPressedInput(_gameData.gameEvent, "CANCEL_2"))
		{
			this->editingSave = false;
			this->isDelete = false;
		}
		else
		{
			this->editSaveButtons.PollEvent(_gameData);
		}
	}
	else
	{
		if (controlsMng.IsPressedInput(_gameData.gameEvent, "CANCEL") || controlsMng.IsPressedInput(_gameData.gameEvent, "CANCEL_2"))
		{
			this->isDelete = true;
		}
		else
		{
			this->saveList.PollEvent(_gameData);
		}
	}
}

void SaveMenu::Display(sf::RenderWindow& _window)
{
	DrawManager::Get().SetObjectVisible("SAVE_MENU", &this->editSaveBackground, this->editingSave);
	DrawManager::Get().SetObjectVisible("SAVE_MENU", &this->editSaveButtons, this->editingSave);
	DrawManager::Get().Draw(_window, "SAVE_MENU");
}

void SaveMenu::InitWidgets(GameData& _gameData)
{
	this->background.setTexture(TexturesManager::Get()["SAVE_BACKGROUND"]);
	this->background.setOrigin(this->background.getGlobalBounds().width * 0.5f, this->background.getGlobalBounds().height * 0.5f);
	this->background.setPosition(WORKING_WIDTH * 0.5f, WORKING_HEIGHT * 0.5f);

	this->saveList.Clear();
	this->saveList.SetLooping(false);
	this->saveList.SetSize({ this->background.getGlobalBounds().width * 0.9f, this->background.getGlobalBounds().height * 0.85f });
	this->saveList.setOrigin(this->saveList.GetRect().width * 0.5f, this->saveList.GetRect().height * 0.5f);
	this->saveList.setPosition(WORKING_WIDTH * 0.5f, WORKING_HEIGHT * 0.5f);

	// Edit menu
	this->editSaveBackground.setTexture(TexturesManager::Get()["SAVE_BACKGROUND"]);
	this->editSaveBackground.setOrigin((sf::Vector2f)this->editSaveBackground.getTexture()->getSize() * 0.5f);
	this->editSaveBackground.setScale(0.75f, 0.75f);
	this->editSaveBackground.setPosition(WORKING_WIDTH * 0.5f, WORKING_HEIGHT * 0.5f);

	this->editSaveButtons.Clear();
	this->editSaveButtons.SetLooping(false);
	this->editSaveButtons.SetSize({ this->editSaveBackground.getGlobalBounds().width, this->editSaveBackground.getGlobalBounds().height });
	this->editSaveButtons.setOrigin(this->editSaveButtons.GetRect().width * 0.5f, this->editSaveButtons.GetRect().height * 0.5f);
	this->editSaveButtons.setPosition(WORKING_WIDTH * 0.5f, WORKING_HEIGHT * 0.5f);

	Button* saveButton = new Button();
	saveButton->SetTextureSprite(TexturesManager::Get()["SAVE_GAME_BUTTON"]);
	sf::FloatRect saveButtonRect = saveButton->GetRect();
	saveButton->setOrigin(saveButtonRect.width * 0.5f, saveButtonRect.height * 0.5f);
	saveButton->setPosition(this->editSaveButtons.GetRect().width * 0.5f, this->editSaveButtons.GetRect().height * 0.25f);
	saveButton->SetOnOverFct([saveButton] { saveButton->SetTextureSprite(TexturesManager::Get()["SAVE_GAME_BUTTON_HOVER"]); });
	saveButton->SetOnOutFct([saveButton] { saveButton->SetTextureSprite(TexturesManager::Get()["SAVE_GAME_BUTTON"]); });
	saveButton->SetOnClickFct([this]
		{
			SaveSystem::Get().SaveGame(this->inGame);
			this->editingSave = false;
		});
	this->editSaveButtons.AddItem(std::move(saveButton));

	Button* loadButton = new Button();
	loadButton->SetTextureSprite(TexturesManager::Get()["LOAD_GAME_BUTTON"]);
	sf::FloatRect loadButtonRect = loadButton->GetRect();
	loadButton->setOrigin(loadButtonRect.width * 0.5f, loadButtonRect.height * 0.5f);
	loadButton->setPosition(this->editSaveButtons.GetRect().width * 0.5f, this->editSaveButtons.GetRect().height * 0.5f);
	loadButton->SetOnOverFct([loadButton]() {loadButton->SetTextureSprite(TexturesManager::Get()["LOAD_GAME_BUTTON_HOVER"]); });
	loadButton->SetOnOutFct([loadButton]() {loadButton->SetTextureSprite(TexturesManager::Get()["LOAD_GAME_BUTTON"]); });
	loadButton->SetOnClickFct([this]
		{
			Audio::Get().PlaySound("BUTTON_PRESSED");
			SaveSystem::Get().LoadGame(this->inGame);
			this->editingSave = false;
			this->isDelete = true;
		});
	this->editSaveButtons.AddItem(std::move(loadButton));

	Button* removeButton = new Button();
	removeButton->SetTextureSprite(TexturesManager::Get()["TRASH_CAN_CLOSED"]);
	sf::FloatRect removeButtonRect = removeButton->GetRect();
	removeButton->setOrigin(removeButtonRect.width * 0.5f, removeButtonRect.height * 0.5f);
	removeButton->setPosition(this->editSaveButtons.GetRect().width * 0.5f, this->editSaveButtons.GetRect().height * 0.75f);
	removeButton->SetOnOverFct([removeButton]() { removeButton->SetTextureSprite(TexturesManager::Get()["TRASH_CAN_OPENED"]); });
	removeButton->SetOnOutFct([removeButton]() { removeButton->SetTextureSprite(TexturesManager::Get()["TRASH_CAN_CLOSED"]); });
	removeButton->SetOnClickFct([this, &_gameData]
		{
			Audio::Get().PlaySound("BUTTON_CANCEL");
			SaveSystem::Get().RemoveSave();
			this->UpdateSaves(_gameData);
			this->editingSave = false;
		});
	this->editSaveButtons.AddItem(std::move(removeButton));
	this->UpdateSaves(_gameData);
}


void SaveMenu::AddButton(GameData& _gameData, const sf::String& _name)
{
	float buttonHeight = this->saveList.GetRect().height / 3.f;
	SaveButton* button = new SaveButton({ this->saveList.GetRect().width, buttonHeight }, _name);
	button->setPosition(0.f, buttonHeight * this->saveList.GetNbItems());
	button->SetOnClickFct([this, _name, &_gameData]
		{
			Audio::Get().PlaySound("BUTTON_PRESSED");
			this->SelectSave(_gameData, _name);
		});
	button->SetOnOverFct(std::bind(&SaveButton::SetSelected, button, true));
	button->SetOnOutFct(std::bind(&SaveButton::SetSelected, button, false));
	this->saveList.AddItem(std::move(button));
}

void SaveMenu::AddNewSave(GameData& _gameData)
{
	if (SaveSystem::GetAllSaveNames().size() < 10)
	{
		std::string name = "Save 0";
		sf::Uint32 index = 0;
		while (SaveSystem::Get().CheckIfSaveExists(name))
		{
			++index;
			name = "Save " + std::to_string(index);
		}
		SaveSystem::Get().SetSaveName(name);
		SaveSystem::Get().SaveGame(this->inGame);
		this->UpdateSaves(_gameData);
	}
}

void SaveMenu::SelectSave(GameData& _gameData, const std::string& _name)
{
	SaveSystem::Get().SetSaveName(_name);
	if (this->inGame != nullptr)
	{
		// Open editing menu if in game
		this->editingSave = true;
		this->editSaveButtons.SetSelectedItemIndex(0);
	}
	else
	{
		//SceneManager::Get().LoadScene(_gameData, new InGame);
		EventManager::Get()["FADE_MENU"];
	}
}