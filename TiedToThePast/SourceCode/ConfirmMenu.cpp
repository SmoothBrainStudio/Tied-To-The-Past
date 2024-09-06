#include "Definitions.hpp"
#include "ConfirmMenu.hpp"

ConfirmMenu::ConfirmMenu()
{
	SetOnClickFct([] {});
	SetOnOverFct([] {});
	SetOnOutFct([] {});

	TexturesManager::Get().Add("Contents/Assets/Sprites/UI/Backgrounds/ConfirmMenu.png", "CONFIRM_MENU_BACKGROUND");

	this->background.setTexture(TexturesManager::Get()["CONFIRM_MENU_BACKGROUND"]);
	this->background.setOrigin(SIZE_X * 0.5f, SIZE_Y * 0.5f);
	this->background.setPosition(WORKING_WIDTH * 0.5f, WORKING_HEIGHT * 0.5f);

	this->text.setFont(FontsManager::Get()["DEFAULT"]);
	this->text.setCharacterSize(40);
	this->SetText("Confirm ?");

	this->UpdateKeyTextures();

	CenterSprite(this->confirmSprite);
	this->confirmSprite.setPosition(POS_X + 0.25f * SIZE_X, POS_Y + 0.75f * SIZE_Y);
	this->confirmSprite.setColor(sf::Color::Green);

	CenterSprite(this->cancelSprite);
	this->cancelSprite.setPosition(POS_X + 0.75f * SIZE_X, POS_Y + 0.75f * SIZE_Y);
	this->cancelSprite.setColor(sf::Color::Red);
}

ConfirmMenu::~ConfirmMenu()
{
	TexturesManager::Get().Delete("CONFIRM_MENU_BACKGROUND");
}

void ConfirmMenu::SetText(const sf::String& _text)
{
	this->text.setString(_text);
	SetCenteredText(this->text);
	this->text.setPosition(WORKING_WIDTH * 0.5f, POS_Y + SIZE_Y * 0.33f);
}

void ConfirmMenu::Reset()
{
	this->isFinished = false;
	this->isConfirmed = false;
	this->UpdateKeyTextures();
}

void ConfirmMenu::Close()
{
	this->isFinished = true;
	this->isConfirmed = false;
	this->UpdateKeyTextures();
}

bool ConfirmMenu::IsFinished() const
{
	return this->isFinished;
}

bool ConfirmMenu::IsConfirmed() const
{
	return this->isConfirmed;
}

void ConfirmMenu::Confirm()
{
	this->isFinished = true;
	this->isConfirmed = true;
	Audio::Get().PlaySoundNoSecurity("ITEM_SELL");
}

void ConfirmMenu::Cancel()
{
	this->isFinished = true;
	this->isConfirmed = false;
	Audio::Get().PlaySoundNoSecurity("BUTTON_CANCEL");
}

void ConfirmMenu::PollEvent(GameData& _gameData)
{
	ControlsManager& controlsMng = ControlsManager::Get();
	if (controlsMng.IsPressedInput(_gameData.gameEvent, "INTERACT") || controlsMng.IsPressedInput(_gameData.gameEvent, "INTERACT_2"))
	{
		this->Confirm();
	}
	else if (controlsMng.IsPressedInput(_gameData.gameEvent, "CANCEL") || controlsMng.IsPressedInput(_gameData.gameEvent, "CANCEL_2"))
	{
		this->Cancel();
	}
	else if (_gameData.gameEvent.type == sf::Event::MouseMoved)
	{
		sf::Vector2f mousePos = Screen::Get().GetMousePosition(_gameData.window);
		if (this->confirmSprite.getGlobalBounds().contains(mousePos))
		{
			this->confirmSprite.setColor(sf::Color(0, 255, 0, 200));
		}
		else if (this->cancelSprite.getGlobalBounds().contains(mousePos))
		{
			this->confirmSprite.setColor(sf::Color::Green);
			this->cancelSprite.setColor(sf::Color(255, 0, 0, 200));
		}
		else
		{
			this->confirmSprite.setColor(sf::Color::Green);
			this->cancelSprite.setColor(sf::Color::Red);
		}
	}
	else if (_gameData.gameEvent.type == sf::Event::MouseButtonPressed)
	{
		sf::Vector2f mousePos = Screen::Get().GetMousePosition(_gameData.window);
		if (this->confirmSprite.getGlobalBounds().contains(mousePos))
		{
			this->Confirm();
		}
		else if (this->cancelSprite.getGlobalBounds().contains(mousePos))
		{
			this->Cancel();
		}
	}
	else if (_gameData.gameEvent.type == sf::Event::JoystickConnected || _gameData.gameEvent.type == sf::Event::JoystickDisconnected)
	{
		this->UpdateKeyTextures();
	}
}

sf::FloatRect ConfirmMenu::GetRect() const
{
	sf::FloatRect globalBounds = this->background.getGlobalBounds();
	globalBounds.left -= this->getPosition().x;
	globalBounds.top -= this->getPosition().y;
	return globalBounds;
}

void ConfirmMenu::UpdateKeyTextures()
{
	ControlsManager& controlsMng = ControlsManager::Get();
	this->confirmSprite.setTexture(controlsMng.GetTexture("INTERACT"));
	this->cancelSprite.setTexture(controlsMng.GetTexture("CANCEL"));
}

void ConfirmMenu::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
{
	_states.transform *= this->getTransform();
	_target.draw(this->background, _states);
	_target.draw(this->text, _states);
	_target.draw(this->confirmSprite, _states);
	_target.draw(this->cancelSprite, _states);
}