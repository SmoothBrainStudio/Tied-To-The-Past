#include "Definitions.hpp"
#include "LoadingScreen.hpp"

LoadingScreen& LoadingScreen::Get(void)
{
	static LoadingScreen instance;
	return instance;
}

void LoadingScreen::Init(void)
{
	InitTextures();
	InitShaders();
	InitAnimations();

	playerPos = sf::Vector2f(50.f, Screen::Get().GetHeight());
}

void LoadingScreen::InitTextures(void)
{
	texMng.Add("Contents/Assets/Sprites/UI/Backgrounds/LoadingScreen.png", "LOADING_SCREEN");
	InitFileTexture("Contents/Data/Textures/PlayerTextureAnimation.txt");

	loadingScreen.setTexture(TexturesManager::Get()["LOADING_SCREEN"], true);
	//loadingScreen.setScale(Screen::Get().Scale((sf::Vector2f)loadingScreen.getScale()));
}

void LoadingScreen::InitShaders(void)
{
	shader.loadFromFile("Contents/Assets/Shaders/Wave.frag", sf::Shader::Fragment);
}

void LoadingScreen::InitAnimations(void)
{
	player[WALKING].LoadFromFile("Contents/Data/Animations/Entities/Player/Boxer/move_down.json");
	player[WALKING].setOrigin(sf::Vector2f(player[WALKING].GetSprite().getGlobalBounds().width, player[WALKING].GetSprite().getGlobalBounds().height));
	player[WALKING].setScale(-1, 1.f);
	player[WALKING].Play();

	player[DAMAGE].LoadFromFile("Contents/Data/Animations/Entities/Player/Boxer/damage_front.json");
	player[DAMAGE].setOrigin(sf::Vector2f(player[DAMAGE].GetSprite().getGlobalBounds().width, player[DAMAGE].GetSprite().getGlobalBounds().height));
	player[DAMAGE].setScale(-1, 1.f);
	player[DAMAGE].Play();
}

void LoadingScreen::Display(void)
{
	mutex.lock();
	Keyboard::Get().Clear();
	playerState = WALKING;
	moveX = -10.f;
	alpha = 0;

	playerPos.y = Screen::Get().GetHeight();
	player[DAMAGE].setScale(Screen::Get().Scale(player[DAMAGE].getScale()));
	player[WALKING].setScale(Screen::Get().Scale(player[WALKING].getScale()));
	loadingScreen.setScale(Screen::Get().Scale((sf::Vector2f)loadingScreen.getScale()));

	window->setActive(true);

	while (loading && window->isOpen())
	{
		GameClock::Get().Update();
		timer += GameClock::Get().GetDeltaTime();
		Keyboard::Get().Update();
		
		if (playerState == DAMAGE && moveX > 0.f)
		{
			playerState = WALKING;
		}
		if (Keyboard::Get().IsAnyKeyPressed() && playerState != DAMAGE && moveX > 0.f)
		{
			playerState = DAMAGE;
			moveX = -MOVE_MAX;
		}
		if (playerPos.x > Screen::Get().GetWidth() + MOVE_MAX)
		{
			playerPos.x = -MOVE_MAX;
		}

		alpha += 3;
		player[playerState].SetColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(Maths::Clamp(static_cast<float>(alpha), 0.f, 255.f))));
		moveX += MOVE_MAX * GameClock::Get().GetDeltaTime() * 2;
		moveX = Maths::Clamp(moveX, -MOVE_MAX, MOVE_MAX);
		playerPos.x += moveX * GameClock::Get().GetDeltaTime();
		player[playerState].Update();
		player[playerState].setPosition(playerPos);

		shader.setUniform("time", timer);
		window->setView(window->getDefaultView());
		window->clear(sf::Color::Black);

		window->draw(loadingScreen, &shader);
		window->draw(player[playerState]);

		window->display();
	}

	Keyboard::Get().Clear();
	player[DAMAGE].setScale(initialScale);
	player[WALKING].setScale(initialScale);
	loadingScreen.setScale(initialScaleLoading);

	window->setActive(false);
	mutex.unlock();
}

void LoadingScreen::SetWindow(sf::RenderWindow& _window)
{
	window = &_window;
}

void LoadingScreen::SetLoading(const bool& _loading)
{
	if (window == nullptr)
	{
		Debug.warning("WINDOW NULLPTR, CAN'T LAUNCH THREAD");
		return;
	}
	
	const bool lastLoading = loading;
	loading = _loading;
	
	if (loading && !lastLoading)
	{
		window->setActive(false);
		displayThread = std::thread(std::bind(&LoadingScreen::Display, this));
	}
	else if (!loading && lastLoading)
	{
		displayThread.join(); // thanks openGL
		window->setActive(true);
	}
	else if (lastLoading)
	{
		Debug.warning("LOADING SCREEN ALREADY LOAD : CAN'T CREATE A THIRD THREAD");
	}
	else if (!lastLoading)
	{
		Debug.warning("LOADING SCREEN ALREADY DELOAD : CAN'T DESTROY NON EXISTANT THREAD");
	}
}
