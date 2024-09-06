#include "Definitions.hpp"
#include "PreMenu.hpp"

PreMenu::~PreMenu()
{
	texturesManager.Delete("CREAJEUX");
	texturesManager.Delete("SMOOTH_BRAIN");
}

void PreMenu::Init(GameData& _gameData)
{
	texturesManager.Add("Contents/Assets/Sprites/UI/Logos/logoCreajeux.png", "CREAJEUX");
	texturesManager.Add("Contents/Assets/Sprites/UI/Logos/logoSmothBrain.png", "SMOOTH_BRAIN");

	ChangeTexture(texturesManager["CREAJEUX"]);
}

void PreMenu::Update(GameData& _gameData)
{
	// Time
	totalTime += GameClock::Get().GetDeltaTime();
	totalTime = Maths::Clamp(totalTime, 0, DURATION_DEFAULT_PREMENU);

	// Color and sprite
	sf::Color nColor = sf::Color::White;
	nColor.a = static_cast<sf::Uint8>(Easings::InOutOutInSine(totalTime / DURATION_DEFAULT_PREMENU) * 255.f);

	sprite.setColor(nColor);

	// Next step (when timer is out)
	if (totalTime >= DURATION_DEFAULT_PREMENU)
	{
		if (state == State::CREAJEUX)
		{
			totalTime = 0.f;
			state = State::SMOOTHBRAIN;
			ChangeTexture(texturesManager["SMOOTH_BRAIN"]);
		}
		else if (state == State::SMOOTHBRAIN)
		{
			totalTime = 0.f;
			state = State::NONE;
			SceneManager::Get().LoadScene(new Introduction);
		}
	}
}

void PreMenu::PollEvent(GameData& _gameData)
{
	if (_gameData.gameEvent.type == sf::Event::MouseButtonPressed || 
		_gameData.gameEvent.type == sf::Event::KeyPressed || 
		_gameData.gameEvent.type == sf::Event::JoystickButtonPressed)
	{
		totalTime = DURATION_DEFAULT_PREMENU;
	}
}

void PreMenu::Display(sf::RenderWindow& _window)
{
	_window.setMouseCursorVisible(false);
	_window.draw(sprite);
}

void PreMenu::ChangeTexture(const sf::Texture& _texture)
{
	// Init tempory variable
	sf::Vector2f newScale(0, 0);
	sf::Vector2f screenSize(scrMng.GetWidth(), scrMng.GetHeight());
	sf::Vector2f textureSize(_texture.getSize());

	// Change texture
	sprite.setTexture(_texture, true);

	// Center sprite
	sprite.setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);
	sprite.setPosition(screenSize.x / 2.f, screenSize.y / 2.f);

	// Rescale to adapt to screen
	newScale.y = screenSize.y / textureSize.y;
	newScale.x = newScale.y;
	sprite.setScale(scrMng.Scale(newScale));
}
