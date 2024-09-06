#include "Definitions.hpp"
#include "HUD.hpp"

HUD::HUD()
{
	Init();
}

HUD::~HUD()
{
	for (auto& bar : bars)
	{
		bar.reset();
		bar = nullptr;
	}

	DrawManager::Get().DeleteSection("HUD");
	DrawManager::Get().DeleteSection("HUD_CANDIES");
	ShadersManager::Get().Delete("WHITE_BLINK");
}

void HUD::Init(void)
{
	TexturesManager& textureM = TexturesManager::Get();
	DrawManager& drawM = DrawManager::Get();

	drawM.AddSection("HUD");
	drawM.AddSection("HUD_CANDIES");
	ShadersManager::Get().Add("Contents/Assets/Shaders/WhiteBlink.frag", "WHITE_BLINK");

	sf::Color color = sf::Color(255, 255, 255, alpha);

	this->inventoryButton.setTexture(textureM["INVENTORY_ICON"]);
	this->inventoryButton.setOrigin(0.f, this->inventoryButton.getGlobalBounds().height * 0.5f);
	this->inventoryButton.setPosition(25.f, 50.f);
	this->inventoryButton.setColor(color);
	this->inventoryKeySprite.setColor(color);

	this->circularMenuButton.setTexture(textureM["CIRCULAR_MENU_ICON"]);
	this->circularMenuButton.setOrigin(0.f, this->circularMenuButton.getGlobalBounds().height * 0.5f);
	this->circularMenuButton.setPosition(25.f, this->inventoryButton.getPosition().y + this->inventoryButton.getGlobalBounds().height + 25.f);
	this->circularMenuButton.setColor(color);
	this->circularMenuKeySprite.setColor(color);

	this->sweets.setTexture(textureM["CANDY"]);
	this->sweets.setOrigin(this->sweets.getGlobalBounds().width * 0.5f, this->sweets.getGlobalBounds().height * 0.5f);
	this->sweets.setPosition(25 + this->sweets.getGlobalBounds().width * 0.5f, WORKING_HEIGHT - this->sweets.getGlobalBounds().height * 0.5f - 25);

	this->nbSweets.setFont(FontsManager::Get()["SPOOKY"]);
	this->nbSweets.setPosition(sf::Vector2f(this->sweets.getPosition().x + 45, this->sweets.getPosition().y - 20));
	this->nbSweets.setCharacterSize(35);

	SetOutlined(this->nbSweets, 2);
	sweetTimerAnimation.SetNewTimerDuration(0.25f);
	drawM.AddObject("HUD_CANDIES", this->sweets, true, &ShadersManager::Get()["WHITE_BLINK"]);
	drawM.AddObject("HUD_CANDIES", this->nbSweets);
	drawM.AddObject("HUD", this->inventoryButton);
	drawM.AddObject("HUD", this->circularMenuButton);
	drawM.AddObject("HUD", this->inventoryKeySprite);
	drawM.AddObject("HUD", this->circularMenuKeySprite);
	this->SetInputsTextures();
}

void HUD::PollEvent(GameData& _gameData)
{
	if (_gameData.gameEvent.type == sf::Event::JoystickConnected || _gameData.gameEvent.type == sf::Event::JoystickDisconnected)
	{
		this->SetInputsTextures();
	}
}

void HUD::Update(Player& _player)
{
	float dt = GameClock::Get().GetDeltaTime();

	if (ControlsManager::Get().HasInputChanged())
	{
		this->SetInputsTextures();
	}

	this->nbSweets.setString(std::to_string(_player.GetPossessedSweets()));

	if (_player.gotSweets)
	{
		sweetTimerAnimation.Restart();
		sweetTimerAnimation.Run();
		_player.gotSweets = false;
	}

	if (sweetTimerAnimation.GetState() == Timer::State::RUNNING)
	{
		sweetTimerAnimation.Update();

		float factor = Easings::Square(sweetTimerAnimation.GetTimePassed() / sweetTimerAnimation.GetTimerDuration());
		ShadersManager::Get()["WHITE_BLINK"].setUniform("timer", factor);
		sweets.setScale(1.f + factor * 0.7f, 1.f + factor * 0.7f);
		if (sweetTimerAnimation.IsOver())
		{
			ShadersManager::Get()["WHITE_BLINK"].setUniform("timer", 0.f);
			sweets.setScale(1.f, 1.f);
		}
	}
}


void HUD::Display(sf::RenderWindow& _window)
{
	DrawManager::Get().Draw(_window, "HUD");
	DrawManager::Get().Draw(_window, "HUD_CANDIES");
}

void HUD::SetInputsTextures()
{
	this->inventoryKeySprite.setTexture(ControlsManager::Get().GetTexture("INVENTORY"));
	this->inventoryKeySprite.setOrigin(0.f, this->inventoryKeySprite.getTexture()->getSize().y * 0.5f);
	this->inventoryKeySprite.setPosition(this->inventoryButton.getPosition().x + this->inventoryButton.getGlobalBounds().width + 10.f, this->inventoryButton.getPosition().y);

	this->circularMenuKeySprite.setTexture(ControlsManager::Get().GetTexture("CIRCULAR_MENU"));
	this->circularMenuKeySprite.setOrigin(0.f, this->circularMenuKeySprite.getTexture()->getSize().y * 0.5f);
	this->circularMenuKeySprite.setPosition(this->inventoryKeySprite.getPosition().x, this->circularMenuButton.getPosition().y);
}