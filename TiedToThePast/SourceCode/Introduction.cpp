#include "Definitions.hpp"
#include "Introduction.hpp"

#pragma region CONSTRUCTOR(S) & DECONSTRUCTOR(S)

Introduction::~Introduction()
{
	// Backgrounds
	texMng.Delete("DAY_BACKGROUND");

	// Texts
	texMng.Delete("TITLE");
	texMng.Delete("PRESS");

	// Lightning
	texMng.Delete("LIGHTNING_0");
	texMng.Delete("LIGHTNING_1");
	texMng.Delete("LIGHTNING_2");
	texMng.Delete("LIGHTNING_3");
	texMng.Delete("LIGHTNING_4");
	texMng.Delete("LIGHTNING_5");
	texMng.Delete("LIGHTNING_6");
	texMng.Delete("LIGHTNING_7");
	texMng.Delete("LIGHTNING_8");
	texMng.Delete("LIGHTNING_9");

	audMng.DeleteSound("THUNDER");
	audMng.DeleteSound("BIRDS_THEMES");
	audMng.DeleteSound("OWL_THEMES");

	shdMng.Delete("WAVE");

	delete fade;
	fade = nullptr;

	drwMng.DeleteSection("INTRODUCTION_DAY");
	drwMng.DeleteSection("INTRODUCTION_THUNDER_1");
	drwMng.DeleteSection("INTRODUCTION_THUNDER_2");
	drwMng.DeleteSection("INTRODUCTION_NIGHT");
}

#pragma endregion

#pragma region INIT

void Introduction::Init(GameData& _gameData)
{
	InitTextures();
	InitAudios();
	InitShaders();

	// text -> title and press
	title.setTexture(texMng["TITLE"]);
	press.setTexture(texMng["PRESS"]);
	CenterSprite(title);
	CenterSprite(press);
	title.setPosition(WORKING_WIDTH / 2.f, WORKING_HEIGHT / 2.f - press.getGlobalBounds().height / 2.f);
	press.setPosition(WORKING_WIDTH / 2.f, WORKING_HEIGHT / 2.f + title.getGlobalBounds().height / 2.f);

	// Backgrounds
	blackColor.create(WORKING_WIDTH, WORKING_HEIGHT, sf::Color::Black);
	whiteColor.create(WORKING_WIDTH, WORKING_HEIGHT, sf::Color::White);
	textureColor.loadFromImage(blackColor);

	dayBackground.setTexture(texMng["DAY_BACKGROUND"], true);
	colorBackground.setTexture(textureColor, true);

	// Members init
	curUpdate = &Introduction::DayUpdate;

	lightning = AnimatedSprite("Contents/Data/Animations/Other/lightning.json");
	sf::FloatRect rect = lightning.GetSprite().getGlobalBounds();
	lightning.setOrigin(rect.left + rect.width / 2.f, 0);
	lightning.setPosition(WORKING_WIDTH / 2.f, 0);

	totalTime = 0.f;
	entitiesTime = 0.f;
	waitTime = Maths::PreciseRand(WAIT_LILBOO_MAX, WAIT_LILBOO_MIN);
	introductionState = IntroductionState::DAY;
	thunderState = ThunderState::THUNDER_ANIM;

	nightBackground.Init();

	// Day clouds init
	dayClouds[0].setTexture(texMng["DAY_CLOUD_1"], true);
	dayClouds[1].setTexture(texMng["DAY_CLOUD_1"], true);
	dayClouds[2].setTexture(texMng["DAY_CLOUD_2"], true);
	dayClouds[3].setTexture(texMng["DAY_CLOUD_3"], true);
	dayClouds[4].setTexture(texMng["DAY_CLOUD_4"], true);
	dayClouds[5].setTexture(texMng["DAY_CLOUD_4"], true);
	dayClouds[6].setTexture(texMng["DAY_CLOUD_5"], true);
	dayClouds[7].setTexture(texMng["DAY_CLOUD_5"], true);

	// Display
	drwMng.AddSection("INTRODUCTION_DAY");
	drwMng.AddObject("INTRODUCTION_DAY", dayBackground);
	for (int i = 0; i < 8; i++)
	{
		drwMng.AddObject("INTRODUCTION_DAY", dayClouds[i]);
	}

	drwMng.AddSection("INTRODUCTION_THUNDER_1");
	drwMng.AddObject("INTRODUCTION_THUNDER_1", dayBackground);
	drwMng.AddObject("INTRODUCTION_THUNDER_1", lightning);

	drwMng.AddSection("INTRODUCTION_THUNDER_2");
	drwMng.AddObject("INTRODUCTION_THUNDER_2", colorBackground);

	drwMng.AddSection("INTRODUCTION_NIGHT");
	drwMng.AddObject("INTRODUCTION_NIGHT", nightBackground);
	drwMng.AddObject("INTRODUCTION_NIGHT", title, true, &shdMng["WAVE"]);
	drwMng.AddObject("INTRODUCTION_NIGHT", press);

	// Fade
	fade = new Fade(3.f, 100.f, false, true);
	drwMng.AddObject("INTRODUCTION_DAY", *fade);
}

void Introduction::InitTextures()
{
	// Backgrounds
	texMng.Add("Contents/Assets/Sprites/UI/Backgrounds/DayIntro.png", "DAY_BACKGROUND");
	texMng.Add("Contents/Assets/Sprites/UI/Introduction/Clouds_day/Cloud_1.png", "DAY_CLOUD_1");
	texMng.Add("Contents/Assets/Sprites/UI/Introduction/Clouds_day/Cloud_2.png", "DAY_CLOUD_2");
	texMng.Add("Contents/Assets/Sprites/UI/Introduction/Clouds_day/Cloud_3.png", "DAY_CLOUD_3");
	texMng.Add("Contents/Assets/Sprites/UI/Introduction/Clouds_day/Cloud_4.png", "DAY_CLOUD_4");
	texMng.Add("Contents/Assets/Sprites/UI/Introduction/Clouds_day/Cloud_5.png", "DAY_CLOUD_5");

	// Texts
	texMng.Add("Contents/Assets/Sprites/UI/Others/TitleGame.png", "TITLE");
	texMng.Add("Contents/Assets/Sprites/UI/Others/PressStart.png", "PRESS");

	// Lightning
	texMng.Add("Contents/Assets/Sprites/Other/Lightning/Lightning_Frame_0.png", "LIGHTNING_0");
	texMng.Add("Contents/Assets/Sprites/Other/Lightning/Lightning_Frame_1.png", "LIGHTNING_1");
	texMng.Add("Contents/Assets/Sprites/Other/Lightning/Lightning_Frame_2.png", "LIGHTNING_2");
	texMng.Add("Contents/Assets/Sprites/Other/Lightning/Lightning_Frame_3.png", "LIGHTNING_3");
	texMng.Add("Contents/Assets/Sprites/Other/Lightning/Lightning_Frame_4.png", "LIGHTNING_4");
	texMng.Add("Contents/Assets/Sprites/Other/Lightning/Lightning_Frame_5.png", "LIGHTNING_5");
	texMng.Add("Contents/Assets/Sprites/Other/Lightning/Lightning_Frame_6.png", "LIGHTNING_6");
	texMng.Add("Contents/Assets/Sprites/Other/Lightning/Lightning_Frame_7.png", "LIGHTNING_7");
	texMng.Add("Contents/Assets/Sprites/Other/Lightning/Lightning_Frame_8.png", "LIGHTNING_8");
	texMng.Add("Contents/Assets/Sprites/Other/Lightning/Lightning_Frame_9.png", "LIGHTNING_9");
}

void Introduction::InitAudios()
{
	audMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Thunder.wav", "THUNDER");
	audMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/BirdsAmbiance.wav", "BIRDS_THEMES");
	audMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/OwlAmbiance.wav", "OWL_THEMES");
}

void Introduction::InitShaders()
{
	shdMng.Add("Contents/Assets/Shaders/Wave.frag", "WAVE");
}

#pragma endregion

#pragma region DISPLAY

void Introduction::Display(sf::RenderWindow& _window)
{
	switch (introductionState)
	{
	case Introduction::IntroductionState::DAY:
		drwMng.Draw(_window, "INTRODUCTION_DAY");
		break;
	case Introduction::IntroductionState::THUNDER:
		switch (thunderState)
		{
		case Introduction::ThunderState::THUNDER_ANIM:
			drwMng.Draw(_window, "INTRODUCTION_THUNDER_1");
			break;
		case Introduction::ThunderState::THUNDER1:
		case Introduction::ThunderState::THUNDER2:
			drwMng.Draw(_window, "INTRODUCTION_THUNDER_2");
			break;
		default:
			break;
		}
		break;
	case Introduction::IntroductionState::NIGHT:
		drwMng.Draw(_window, "INTRODUCTION_NIGHT");
		break;
	default:
		break;
	}
}

#pragma endregion

#pragma region UPDATE

void Introduction::Update(GameData& _gameData)
{
	// Time
	totalTime += glMng.GetDeltaTime();

	// Update
	(this->*curUpdate)();
}

void Introduction::DayUpdate()
{
	totalTime = Maths::Clamp(totalTime, 0, DURATION_DEFAULT_INTRO);

	// Calcul day background position
	float factor = totalTime / DURATION_DEFAULT_INTRO;
	float heightVal = (float)dayBackground.getGlobalBounds().height * dayBackground.getScale().y - WORKING_HEIGHT;
	int posY = int(Easings::InOutSine(factor) * heightVal);

	sf::Vector2f toMove = sf::Vector2f(0.f, -(float)posY);
	dayClouds[0].setPosition(POSITION_CLOUD_1 + toMove + sf::Vector2f(sin(totalTime + 4) * 10, 0.f));
	dayClouds[1].setPosition(POSITION_CLOUD_2 + toMove + sf::Vector2f(sin(totalTime + 6) * 12, 0.f));
	dayClouds[2].setPosition(POSITION_CLOUD_3 + toMove + sf::Vector2f(sin(totalTime + 8) * 14, 0.f));
	dayClouds[3].setPosition(POSITION_CLOUD_4 + toMove + sf::Vector2f(sin(totalTime + 10) * 16, 0.f));
	dayClouds[4].setPosition(POSITION_CLOUD_5 + toMove + sf::Vector2f(sin(totalTime + 12) * 18, 0.f));
	dayClouds[5].setPosition(POSITION_CLOUD_6 + toMove + sf::Vector2f(sin(totalTime + 14) * 20, 0.f));
	dayClouds[6].setPosition(POSITION_CLOUD_7 + toMove + sf::Vector2f(sin(totalTime + 16) * 22, 0.f));
	dayClouds[7].setPosition(POSITION_CLOUD_8 + toMove + sf::Vector2f(sin(totalTime + 18) * 24, 0.f));

	dayBackground.setPosition(toMove);

	audMng.PlaySound("BIRDS_THEMES");

	// On over
	if (totalTime == DURATION_DEFAULT_INTRO) NextToThunder();

	// Fade
	if (fade->GetTimer() > 0.f)
	{
		fade->Update(GameClock::Get().GetDeltaTime());
	}
}

void Introduction::ThunderUpdate()
{
	lightning.Update();

	if (thunderState == ThunderState::THUNDER_ANIM)
	{
		if (lightning.GetStatus() == AnimatedSprite::Status::STOPPED)
		{
			thunderState = ThunderState::THUNDER1;
			audMng.PlaySound("THUNDER");
			totalTime = 0.f;
		}
	}
	else if (thunderState == ThunderState::THUNDER1)
	{
		totalTime = Maths::Clamp(totalTime, 0, DURATION_FLASH);

		if (totalTime == DURATION_FLASH)
		{
			// Change state
			thunderState = ThunderState::THUNDER2;
			audMng.PlayMusic("SPOOKY_THEME", true);

			// Reset
			textureColor.loadFromImage(whiteColor);
			totalTime = 0.f;
		}
	}
	else if (thunderState == ThunderState::THUNDER2)
	{
		totalTime = Maths::Clamp(totalTime, 0, DURATION_FLASH);
		if (totalTime == DURATION_FLASH) NextToNight();
	}
}

void Introduction::NightUpdate()
{
	float opacity = (cos(2 * totalTime) / 2.f + 0.5f) * 255.f;
	press.setColor(sf::Color(255, 255, 255, (sf::Uint8)opacity));
	shdMng["WAVE"].setUniform("time", totalTime);

	nightBackground.Update();
}

#pragma endregion

#pragma region POLLEVENT

void Introduction::PollEvent(GameData& _gameData)
{
	if (_gameData.gameEvent.type == sf::Event::MouseButtonPressed || _gameData.gameEvent.type == sf::Event::KeyPressed || _gameData.gameEvent.type == sf::Event::JoystickButtonPressed)
	{
		if (introductionState == IntroductionState::NIGHT)
		{
			SceneManager::Get().LoadScene(new MainMenu);
			audMng.StopAllSounds();
		}
		else
		{
			curUpdate = &Introduction::NightUpdate;
			introductionState = IntroductionState::NIGHT;

			audMng.StopAllSounds();
			audMng.PlayMusic("SPOOKY_THEME", true);
		}
	}
}

#pragma endregion

#pragma region NEXTS

void Introduction::NextToThunder()
{
	// Change state
	curUpdate = &Introduction::ThunderUpdate;
	introductionState = IntroductionState::THUNDER;

	lightning.Play();

	// Reset
	totalTime = 0;
	audMng.StopAllSounds();
}

void Introduction::NextToNight()
{
	curUpdate = &Introduction::NightUpdate;
	introductionState = IntroductionState::NIGHT;

	audMng.PlaySound("OWL_THEMES");

	totalTime = 0;
}

#pragma endregion
