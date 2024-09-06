#include "Definitions.hpp"
#include "CreditsMenu.hpp"

CreditsMenu::~CreditsMenu()
{
	TexturesManager& texturesManager = TexturesManager::Get();

	texturesManager.Delete("CREDITS_BG");
	quitButton.reset();

	DrawManager::Get().DeleteSection("CREDITS");
}

void CreditsMenu::Init(GameData& _gameData)
{
	TexturesManager& texturesManager = TexturesManager::Get();
	DrawManager& drawManager = DrawManager::Get();

	texturesManager.Add("Contents/Assets/Sprites/UI/Backgrounds/Credits.png", "CREDITS_BG");
	background.setTexture(texturesManager["CREDITS_BG"]);

	drawManager.AddSection("CREDITS");
	drawManager.AddObject("CREDITS", background);

	InitButton(_gameData);
}

void CreditsMenu::Update(GameData& _gameData)
{
}

void CreditsMenu::PollEvent(GameData& _gameData)
{
	quitButton->PollEvent(_gameData);

	quitButton->CallOnOver();

	if (_gameData.gameEvent.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)
		|| ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "INTERACT"))
	{
		quitButton->CallOnOut();
		quitButton->CallOnClick();
		Audio::Get().PlaySound("BUTTON_CANCEL");
	}

	if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "CANCEL") ||
		ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "CANCEL_2"))
	{
		quitButton->CallOnOut();
		quitButton->CallOnClick();
		Audio::Get().PlaySound("BUTTON_CANCEL");
	}
}

void CreditsMenu::Display(sf::RenderWindow& _window)
{
	DrawManager::Get().Draw(_window, "CREDITS");
}

void CreditsMenu::InitButton(GameData& _gameData)
{
	TexturesManager& texturesManager = TexturesManager::Get();

	sf::Vector2f pos = sf::Vector2f((float)(WORKING_WIDTH - texturesManager["BACK_BUTTON"].getSize().x - SPACING_BUTTON_CREDITS),
		(float)(WORKING_HEIGHT - texturesManager["BACK_BUTTON"].getSize().y - SPACING_BUTTON_CREDITS));

	quitButton = std::make_unique<Button>
		(
			pos,
			texturesManager["BACK_BUTTON"]
			);

	quitButton->SetOnClickFct([this, &_gameData]
		{
			Audio::Get().PlaySound("BUTTON_CANCEL");
			isDelete = true;
		});

	quitButton->SetOnOverFct([this]
		{
			TexturesManager& texturesManager = TexturesManager::Get();
	quitButton->SetTextureSprite(texturesManager["BACK_BUTTON_HOVER"]);
		});

	quitButton->SetOnOutFct([this]
		{
			TexturesManager& texturesManager = TexturesManager::Get();
	quitButton->SetTextureSprite(texturesManager["BACK_BUTTON"]);
		});

	DrawManager::Get().AddObject("CREDITS", *quitButton);
}
