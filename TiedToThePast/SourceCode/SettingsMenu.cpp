#include "Definitions.hpp"
#include "SettingsMenu.hpp"

SettingsMenu::SettingsMenu() :
	currentId(0), titleBackground(nullptr), title(nullptr), currentSubState(SubMenuState::NONE), styleText(nullptr)
{
	currentUpdate = static_cast<PtrSubState>(&SettingsMenu::UpdateCommon);
	currentPollEvent = static_cast<PtrSubState>(&SettingsMenu::PollEventCommon);
}

SettingsMenu::~SettingsMenu()
{
	titleBackground.reset();
	title.reset();

	styleText.reset();
	fpsText.reset();
	antialiasingText.reset();
	resolutionText.reset();

	soundText.reset();
	musicText.reset();

	while (!controlsItems.empty())
	{
		controlsItems.back().reset();
		controlsItems.pop_back();
	}

	commonItems.clear();
	videoItems.clear();
	audioItems.clear();
	controlsItems.clear();

	texMng.Delete("SETTING_TITLE");
	texMng.Delete("VIDEO_BUTTON");
	texMng.Delete("VIDEO_BUTTON_HOVER");
	texMng.Delete("AUDIO_BUTTON");
	texMng.Delete("AUDIO_BUTTON_HOVER");
	texMng.Delete("CONTROLS_BUTTON");
	texMng.Delete("CONTROLS_BUTTON_HOVER");
	texMng.Delete("RESET_BUTTON");
	texMng.Delete("RESET_BUTTON_HOVER");
	texMng.Delete("BAR_BACK");
	texMng.Delete("BAR");
	texMng.Delete("BAR_BORDER");
	texMng.Delete("BAR_BORDER_HOVER");
	texMng.Delete("BAR_SELECTOR");
	texMng.Delete("CHECKBOX");
	texMng.Delete("CHECKBOX_HOVER");
	texMng.Delete("CHECK");

	drwMng.DeleteSection("SETTINGS_COMMON");
	drwMng.DeleteSection("SETTINGS_VIDEO");
	drwMng.DeleteSection("SETTINGS_AUDIO");
	drwMng.DeleteSection("SETTINGS_CONTROLS");
}

void SettingsMenu::Init(GameData& _gameData)
{
	InitTextures(_gameData);
	InitCommonSettings(_gameData);
	InitVideoSettings(_gameData);
	InitAudioSettings(_gameData);
	InitControlsSettings(_gameData);
}

void SettingsMenu::Update(GameData& _gameData)
{
	background.Update();
	(this->*currentUpdate)(_gameData);
}

void SettingsMenu::PollEvent(GameData& _gameData)
{
	(this->*currentPollEvent)(_gameData);

	// Pollevent of all commonItems
	for (int i = 0; i < commonItems.size(); i++)
	{
		commonItems[i]->PollEvent(_gameData);
	}

	// Return on left buttons
	if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "CANCEL") ||
		ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "CANCEL_2") ||
		(_gameData.gameEvent.type == sf::Event::KeyPressed && _gameData.gameEvent.key.code == sf::Keyboard::Escape))
	{
		if (currentSubState == SubMenuState::NONE)
		{
			Back(_gameData);
		}
		EnterInNewSubState(SubMenuState::NONE);
	}
}

void SettingsMenu::Display(sf::RenderWindow& _window)
{
	drwMng.Draw(_window, "SETTINGS_COMMON");

	switch (currentSubState)
	{
	case SettingsMenu::SubMenuState::VIDEO:
		drwMng.Draw(_window, "SETTINGS_VIDEO");
		break;
	case SettingsMenu::SubMenuState::AUDIO:
		drwMng.Draw(_window, "SETTINGS_AUDIO");
		break;
	case SettingsMenu::SubMenuState::CONTROLS:
		drwMng.Draw(_window, "SETTINGS_CONTROLS");
		break;
	default:
		break;
	}
}

void SettingsMenu::InitTextures(GameData& _gameData)
{
	texMng.Add("Contents/Assets/Sprites/UI/MenuItems/setting_title.png", "SETTING_TITLE");
	texMng.Add("Contents/Assets/Sprites/UI/MenuItems/video_button.png", "VIDEO_BUTTON");
	texMng.Add("Contents/Assets/Sprites/UI/MenuItems/video_button_hover.png", "VIDEO_BUTTON_HOVER");
	texMng.Add("Contents/Assets/Sprites/UI/MenuItems/audio_button.png", "AUDIO_BUTTON");
	texMng.Add("Contents/Assets/Sprites/UI/MenuItems/audio_button_hover.png", "AUDIO_BUTTON_HOVER");
	texMng.Add("Contents/Assets/Sprites/UI/MenuItems/controls_button.png", "CONTROLS_BUTTON");
	texMng.Add("Contents/Assets/Sprites/UI/MenuItems/controls_button_hover.png", "CONTROLS_BUTTON_HOVER");
	texMng.Add("Contents/Assets/Sprites/UI/MenuItems/reset_button.png", "RESET_BUTTON");
	texMng.Add("Contents/Assets/Sprites/UI/MenuItems/reset_button_hover.png", "RESET_BUTTON_HOVER");
	texMng.Add("Contents/Assets/Sprites/UI/MenuItems/Back.png", "BAR_BACK");
	texMng.Add("Contents/Assets/Sprites/UI/MenuItems/Bar.png", "BAR");
	texMng.Add("Contents/Assets/Sprites/UI/MenuItems/Border.png", "BAR_BORDER");
	texMng.Add("Contents/Assets/Sprites/UI/MenuItems/Border_hover.png", "BAR_BORDER_HOVER");
	texMng.Add("Contents/Assets/Sprites/UI/MenuItems/SelectorSlider.png", "BAR_SELECTOR");
	texMng.Add("Contents/Assets/Sprites/UI/MenuItems/Checkbox.png", "CHECKBOX");
	texMng.Add("Contents/Assets/Sprites/UI/MenuItems/Checkbox_hover.png", "CHECKBOX_HOVER");
	texMng.Add("Contents/Assets/Sprites/UI/MenuItems/Check.png", "CHECK");
}

void SettingsMenu::InitCommonSettings(GameData& _gameData)
{
	// Section
	drwMng.AddSection("SETTINGS_COMMON");

	// Item create
	titleBackground = std::make_unique<sf::Sprite>();
	title = std::make_unique<sf::Text>();
	std::unique_ptr<Button> videoButton = std::make_unique<Button>();
	std::unique_ptr<Button> audioButton = std::make_unique<Button>();
	std::unique_ptr<Button> controlButton = std::make_unique<Button>();
	std::unique_ptr<Button> backButton = std::make_unique<Button>();

	// Add to draw manager
	drwMng.AddObject("SETTINGS_COMMON", background);
	drwMng.AddObject("SETTINGS_COMMON", *titleBackground);
	drwMng.AddObject("SETTINGS_COMMON", *title);
	drwMng.AddObject("SETTINGS_COMMON", *videoButton);
	drwMng.AddObject("SETTINGS_COMMON", *audioButton);
	drwMng.AddObject("SETTINGS_COMMON", *controlButton);
	drwMng.AddObject("SETTINGS_COMMON", *backButton);

	//
	// background
	//
	background.Init();
	//
	// titleBackground
	//
	titleBackground->setTexture(texMng["SETTING_TITLE"], true);
	titleBackground->setPosition(16, 16);
	//
	// title
	//
	*title = CreateText(fntMng["SPOOKY"], "SETTINGS", 60, sf::Color(36, 127, 101), true);
	SetCenteredText(*title);
	sf::FloatRect rectBg = titleBackground->getGlobalBounds();
	sf::Vector2f posBg = titleBackground->getPosition();
	title->setPosition(posBg.x + rectBg.width / 2.f, posBg.y + rectBg.height / 2.f);
	//
	// videoButton
	//
	videoButton->setPosition(sf::Vector2f(16, 140));
	videoButton->SetTextureSprite(texMng["VIDEO_BUTTON"]);
	videoButton->SetID(0);
	videoButton->SetOnClickFct([this]() -> void {
		title->setString("VIDEO SETTINGS");
		SetCenteredText(*title);
		EnterInNewSubState(SubMenuState::VIDEO);
		audMng.PlaySoundNoSecurity("BUTTON_PRESSED");
		currentId = 4;
		});
	videoButton->SetOnOutFct([&]() -> void {
		((Button*)commonItems[0].get())->SetTextureSprite(texMng["VIDEO_BUTTON"]);
		});
	videoButton->SetOnOverFct([&]() -> void {
		((Button*)commonItems[0].get())->SetTextureSprite(texMng["VIDEO_BUTTON_HOVER"]);
		currentId = commonItems[0]->GetID();
		});
	//
	// audioButton
	//
	audioButton->setPosition(sf::Vector2f(16, 264));
	audioButton->SetTextureSprite(texMng["AUDIO_BUTTON"]);
	audioButton->SetID(1);
	audioButton->SetOnClickFct([&]() -> void {
		title->setString("AUDIO SETTINGS");
		SetCenteredText(*title);
		EnterInNewSubState(SubMenuState::AUDIO);
		audMng.PlaySoundNoSecurity("BUTTON_PRESSED");
		currentId = 4;
		});
	audioButton->SetOnOutFct([&]() -> void {
		((Button*)commonItems[1].get())->SetTextureSprite(texMng["AUDIO_BUTTON"]);
		});
	audioButton->SetOnOverFct([&]() -> void {
		((Button*)commonItems[1].get())->SetTextureSprite(texMng["AUDIO_BUTTON_HOVER"]);
		currentId = commonItems[1]->GetID();
		});
	//
	// controlButton
	//
	controlButton->setPosition(sf::Vector2f(16, 388));
	controlButton->SetTextureSprite(texMng["CONTROLS_BUTTON"]);
	controlButton->SetID(2);
	controlButton->SetOnClickFct([&]() -> void {
		title->setString("CONTROLS SETTINGS");
		SetCenteredText(*title);
		EnterInNewSubState(SubMenuState::CONTROLS);
		audMng.PlaySoundNoSecurity("BUTTON_PRESSED");
		});
	controlButton->SetOnOutFct([&]() -> void {
		((Button*)commonItems[2].get())->SetTextureSprite(texMng["CONTROLS_BUTTON"]);
		});
	controlButton->SetOnOverFct([&]() -> void {
		((Button*)commonItems[2].get())->SetTextureSprite(texMng["CONTROLS_BUTTON_HOVER"]);
		currentId = commonItems[2]->GetID();
		});
	//
	// backButton
	//
	backButton->setPosition(sf::Vector2f(16, 956));
	backButton->SetTextureSprite(texMng["BACK_BUTTON"]);
	backButton->SetID(3);
	backButton->SetOnClickFct([this, &_gameData]() -> void {
		Back(_gameData);
		});
	backButton->SetOnOutFct([&]() -> void {
		((Button*)commonItems[3].get())->SetTextureSprite(texMng["BACK_BUTTON"]);
		});
	backButton->SetOnOverFct([&]() -> void {
		((Button*)commonItems[3].get())->SetTextureSprite(texMng["BACK_BUTTON_HOVER"]);
		currentId = commonItems[3]->GetID();
		});


	// Vector of commonItems
	commonItems.push_back(std::move(videoButton));
	commonItems.push_back(std::move(audioButton));
	commonItems.push_back(std::move(controlButton));
	commonItems.push_back(std::move(backButton));
}

void SettingsMenu::InitVideoSettings(GameData& _gameData)
{
	// Section
	drwMng.AddSection("SETTINGS_VIDEO");

	// Item create
	styleText = std::make_unique<sf::Text>();
	fpsText = std::make_unique<sf::Text>();
	antialiasingText = std::make_unique<sf::Text>();
	resolutionText = std::make_unique<sf::Text>();
	std::unique_ptr<SelectBox<unsigned int>> styleSelect = std::make_unique<SelectBox<unsigned int>>();
	std::unique_ptr<SelectBox<unsigned int>> fpsSelect = std::make_unique<SelectBox<unsigned int>>();
	std::unique_ptr<SelectBox<unsigned int>> antialiasingSelect = std::make_unique<SelectBox<unsigned int>>();
	std::unique_ptr<SelectBox<sf::Vector2f>> resolutionSelect = std::make_unique<SelectBox<sf::Vector2f>>();
	std::unique_ptr<Checkbox> checkbox = std::make_unique<Checkbox>();
	std::unique_ptr<Checkbox> disableHelp = std::make_unique<Checkbox>();

	// Add to draw manager
	drwMng.AddObject("SETTINGS_VIDEO", *styleText);
	drwMng.AddObject("SETTINGS_VIDEO", *styleSelect);
	drwMng.AddObject("SETTINGS_VIDEO", *fpsText);
	drwMng.AddObject("SETTINGS_VIDEO", *fpsSelect);
	drwMng.AddObject("SETTINGS_VIDEO", *antialiasingText);
	drwMng.AddObject("SETTINGS_VIDEO", *antialiasingSelect);
	drwMng.AddObject("SETTINGS_VIDEO", *resolutionText);
	drwMng.AddObject("SETTINGS_VIDEO", *resolutionSelect);
	drwMng.AddObject("SETTINGS_VIDEO", *checkbox);
	drwMng.AddObject("SETTINGS_VIDEO", *disableHelp);

	//
	// styleText
	//
	*styleText = CreateText(fntMng["SPOOKY"], "Window", 42, sf::Color::White);
	SetOutlined(*styleText, 2);
	styleText->setPosition(416, 140);
	//
	// styleSelect
	//
	styleSelect->setPosition(416, 196);
	styleSelect->SetBackTexture(texMng["BAR_BACK"]);
	styleSelect->SetBorderTexture(texMng["BAR_BORDER"]);
	styleSelect->SetID(4);
	styleSelect->AddOption("Windowed", sf::Style::Titlebar | sf::Style::Close);
	styleSelect->AddOption("Fullscreen", sf::Style::Fullscreen);
	styleSelect->AddOption("Borderless", sf::Style::None);
	styleSelect->SetValueByCurValue(scrMng.GetStyle());
	styleSelect->SetOnOutFct([&]() -> void {
		((SelectBox<unsigned int>*)videoItems[0].get())->SetBorderTexture(texMng["BAR_BORDER"]);
		});
	styleSelect->SetOnOverFct([&]() -> void {
		audMng.PlaySoundNoSecurity("BUTTON_SWAP");
		((SelectBox<unsigned int>*)videoItems[0].get())->SetBorderTexture(texMng["BAR_BORDER_HOVER"]);
		});
	styleSelect->SetOnChangeFct([&]() -> void {
		scrMng.SetStyle(((SelectBox<unsigned int>*)videoItems[0].get())->GetValue());
		videoModified = true;
		audMng.PlaySoundNoSecurity("BUTTON_PRESSED");
		});
	//
	// fpsText
	//
	*fpsText = CreateText(fntMng["SPOOKY"], "Frames Per Second", 42, sf::Color::White);
	SetOutlined(*fpsText, 2);
	fpsText->setPosition(416, 320);
	//
	// fpsSelect
	//
	fpsSelect->setPosition(416, 376);
	fpsSelect->SetBackTexture(texMng["BAR_BACK"]);
	fpsSelect->SetBorderTexture(texMng["BAR_BORDER"]);
	fpsSelect->SetID(5);
	fpsSelect->AddOption("30 FPS", 30);
	fpsSelect->AddOption("60 FPS", 60);
	fpsSelect->AddOption("120 FPS", 120);
	fpsSelect->AddOption("144 FPS", 144);
	fpsSelect->AddOption("240 FPS", 240);
	fpsSelect->AddOption("360 FPS", 360);
	fpsSelect->AddOption("Unlimited FPS", 0);
	fpsSelect->SetValueByCurValue(scrMng.GetFramerateLimit());
	fpsSelect->SetOnOutFct([&]() -> void {
		((SelectBox<unsigned int>*)videoItems[1].get())->SetBorderTexture(texMng["BAR_BORDER"]);
		});
	fpsSelect->SetOnOverFct([&]() -> void {
		audMng.PlaySoundNoSecurity("BUTTON_SWAP");
		((SelectBox<unsigned int>*)videoItems[1].get())->SetBorderTexture(texMng["BAR_BORDER_HOVER"]);
		});
	fpsSelect->SetOnChangeFct([&]() -> void {
		scrMng.SetFramerateLimit(((SelectBox<unsigned int>*)videoItems[1].get())->GetValue());
		videoModified = true;
		audMng.PlaySoundNoSecurity("BUTTON_PRESSED");
		});
	//
	// antialiasingText
	//
	*antialiasingText = CreateText(fntMng["SPOOKY"], "Anti Aliasing", 42, sf::Color::White);
	SetOutlined(*antialiasingText, 2);
	antialiasingText->setPosition(416, 500);
	//
	// antialiasingSelect
	//
	antialiasingSelect->setPosition(416, 556);
	antialiasingSelect->SetBackTexture(texMng["BAR_BACK"]);
	antialiasingSelect->SetBorderTexture(texMng["BAR_BORDER"]);
	antialiasingSelect->SetID(6);
	antialiasingSelect->AddOption("Disable", 0);
	antialiasingSelect->AddOption("x2", 2);
	antialiasingSelect->AddOption("x4", 4);
	antialiasingSelect->AddOption("x8", 8);
	antialiasingSelect->SetValueByCurValue(scrMng.GetAntiAliasing());
	antialiasingSelect->SetOnOutFct([&]() -> void {
		((SelectBox<unsigned int>*)videoItems[2].get())->SetBorderTexture(texMng["BAR_BORDER"]);
		});
	antialiasingSelect->SetOnOverFct([&]() -> void {
		audMng.PlaySoundNoSecurity("BUTTON_SWAP");
		((SelectBox<unsigned int>*)videoItems[2].get())->SetBorderTexture(texMng["BAR_BORDER_HOVER"]);
		});
	antialiasingSelect->SetOnChangeFct([&]() -> void {
		scrMng.SetAntiAliasing(((SelectBox<unsigned int>*)videoItems[2].get())->GetValue());
		videoModified = true;
		audMng.PlaySoundNoSecurity("BUTTON_PRESSED");
		});
	//
	// resolutionText
	//
	*resolutionText = CreateText(fntMng["SPOOKY"], "Resolution", 42, sf::Color::White);
	SetOutlined(*resolutionText, 2);
	resolutionText->setPosition(416, 680);
	//
	// resolutionSelect
	//
	resolutionSelect->setPosition(416, 736);
	resolutionSelect->SetBackTexture(texMng["BAR_BACK"]);
	resolutionSelect->SetBorderTexture(texMng["BAR_BORDER"]);
	resolutionSelect->SetID(7);
	resolutionSelect->AddOption("1024 x 576", sf::Vector2f(1024, 576));
	resolutionSelect->AddOption("1280 x 720", sf::Vector2f(1280, 720));
	resolutionSelect->AddOption("1366 x 768", sf::Vector2f(1366, 768));
	resolutionSelect->AddOption("1600 x 900", sf::Vector2f(1600, 900));
	resolutionSelect->AddOption("1920 x 1080", sf::Vector2f(1920, 1080));
	resolutionSelect->AddOption("2560 x 1440", sf::Vector2f(2560, 1440));
	resolutionSelect->AddOption("3840 x 2160", sf::Vector2f(3840, 2160));
	resolutionSelect->SetValueByCurValue(sf::Vector2f(scrMng.GetWidth(), scrMng.GetHeight()));
	resolutionSelect->SetOnOutFct([&]() -> void {
		((SelectBox<sf::Vector2f>*)videoItems[3].get())->SetBorderTexture(texMng["BAR_BORDER"]);
		});
	resolutionSelect->SetOnOverFct([&]() -> void {
		audMng.PlaySoundNoSecurity("BUTTON_SWAP");
		((SelectBox<sf::Vector2f>*)videoItems[3].get())->SetBorderTexture(texMng["BAR_BORDER_HOVER"]);
		});
	resolutionSelect->SetOnChangeFct([&]() -> void {
		sf::Vector2f resolution = ((SelectBox<sf::Vector2f>*)videoItems[3].get())->GetValue();

		scrMng.SetVideoMode(std::min((unsigned int)resolution.x, sf::VideoMode::getFullscreenModes()[0].width), std::min((unsigned int)resolution.y, sf::VideoMode::getFullscreenModes()[0].height));

		videoModified = true;
		audMng.PlaySoundNoSecurity("BUTTON_PRESSED");
		});
	//
	// checkbox
	//
	checkbox->setPosition(432, 876);
	checkbox->SetCheckboxTexture(texMng["CHECKBOX"]);
	checkbox->SetCheckTexture(texMng["CHECK"]);
	checkbox->SetText("V-Sync");
	checkbox->SetID(8);
	checkbox->SetValue(scrMng.GetVerticalSync());
	checkbox->SetOnOverFct([&]() -> void {
		((Checkbox*)videoItems[4].get())->SetCheckboxTexture(texMng["CHECKBOX_HOVER"]);
		});
	checkbox->SetOnOutFct([&]() -> void {
		audMng.PlaySoundNoSecurity("BUTTON_SWAP");
		((Checkbox*)videoItems[4].get())->SetCheckboxTexture(texMng["CHECKBOX"]);
		});
	checkbox->SetOnChangeFct([&]() -> void {
		videoModified = true;
		scrMng.SetVerticalSync(((Checkbox*)videoItems[4].get())->IsChecked());
		});
	//
	// enableHelp
	//
	disableHelp->setPosition(632, 876);
	disableHelp->SetCheckboxTexture(texMng["CHECKBOX"]);
	disableHelp->SetCheckTexture(texMng["CHECK"]);
	disableHelp->SetText("Disable Battle Aid");
	disableHelp->SetID(9);
	disableHelp->SetValue(BoolManager::Get()["FightAid"]);
	disableHelp->SetOnOverFct([&]() -> void {
		((Checkbox*)videoItems[5].get())->SetCheckboxTexture(texMng["CHECKBOX_HOVER"]);
		});
	disableHelp->SetOnOutFct([&]() -> void {
		audMng.PlaySoundNoSecurity("BUTTON_SWAP");
		((Checkbox*)videoItems[5].get())->SetCheckboxTexture(texMng["CHECKBOX"]);
		});
	disableHelp->SetOnChangeFct([&]() -> void {
		BoolManager::Get()["FightAid"] = ((Checkbox*)videoItems[5].get())->IsChecked();
		});

	// Vector of commonItems
	videoItems.push_back(std::move(styleSelect));
	videoItems.push_back(std::move(fpsSelect));
	videoItems.push_back(std::move(antialiasingSelect));
	videoItems.push_back(std::move(resolutionSelect));
	videoItems.push_back(std::move(checkbox));
	videoItems.push_back(std::move(disableHelp));
}

void SettingsMenu::InitAudioSettings(GameData& _gameData)
{
	// Section
	drwMng.AddSection("SETTINGS_AUDIO");

	// Item create
	soundText = std::make_unique<sf::Text>();
	std::unique_ptr<Bar> soundSlider = std::make_unique<Bar>();
	musicText = std::make_unique<sf::Text>();
	std::unique_ptr<Bar> musicSlider = std::make_unique<Bar>();

	// Add to draw manager
	drwMng.AddObject("SETTINGS_AUDIO", *soundText);
	drwMng.AddObject("SETTINGS_AUDIO", *soundSlider);
	drwMng.AddObject("SETTINGS_AUDIO", *musicText);
	drwMng.AddObject("SETTINGS_AUDIO", *musicSlider);

	//
	// soundText
	//
	*soundText = CreateText(fntMng["SPOOKY"], "Sound volume", 42, sf::Color::White);
	SetOutlined(*soundText, 2);
	soundText->setPosition(416, 140);
	//
	// soundSlider
	//
	soundSlider->setPosition(416, 202);
	soundSlider->SetID(4);
	soundSlider->SetValue(static_cast<const short>(audMng.GetSoundMultiplier() * static_cast<float>(soundSlider->GetMax()) - static_cast<float>(soundSlider->GetMin())));
	soundSlider->SetBackSprite(texMng["BAR_BACK"]);
	soundSlider->SetBarSprite(texMng["BAR"]);
	soundSlider->SetOutlineSprite(texMng["BAR_BORDER"]);
	soundSlider->SetSelectorSprite(texMng["BAR_SELECTOR"]);
	soundSlider->SetOnClickReleaseFct([this]() -> void {
		audMng.PlaySoundNoSecurity("BUTTON_PRESSED");
		});
	soundSlider->SetWhileClickFct([&]() -> void {
		sf::Vector2f mousePos = (sf::Vector2f)Screen::Get().GetMousePosition(_gameData.window);
		float currentVolume = Maths::Clamp((((mousePos.x - ((Bar*)audioItems[0].get())->getPosition().x) / ((Bar*)audioItems[0].get())->GetRect().width) * ((Bar*)audioItems[0].get())->GetMax()), ((Bar*)audioItems[0].get())->GetMin(), ((Bar*)audioItems[0].get())->GetMax());
		((Bar*)audioItems[0].get())->SetValue(static_cast<const short>(currentVolume));
		});
	soundSlider->SetOnOutFct([&]() -> void {
		((Bar*)audioItems[0].get())->SetOutlineSprite(texMng["BAR_BORDER"]);
		});
	soundSlider->SetOnOverFct([&]() -> void {
		currentId = audioItems[0]->GetID();
		((Bar*)audioItems[0].get())->SetOutlineSprite(texMng["BAR_BORDER_HOVER"]);
		});
	soundSlider->SetOnChangeFct([&]() -> void {
		audMng.SetSoundMultiplier((float)((Bar*)audioItems[0].get())->GetValue() / (float)((Bar*)audioItems[0].get())->GetMax());
		});
	//
	// musicText
	//
	*musicText = CreateText(fntMng["SPOOKY"], "Music volume", 42, sf::Color::White);
	SetOutlined(*musicText, 2);
	musicText->setPosition(416, 334);
	//
	// musicSlider
	//
	musicSlider->setPosition(416, 404);
	musicSlider->SetID(5);
	musicSlider->SetValue(static_cast<const short>(audMng.GetMusicMultiplier() * musicSlider->GetMax() - musicSlider->GetMin()));
	musicSlider->SetBackSprite(texMng["BAR_BACK"]);
	musicSlider->SetBarSprite(texMng["BAR"]);
	musicSlider->SetOutlineSprite(texMng["BAR_BORDER"]);
	musicSlider->SetSelectorSprite(texMng["BAR_SELECTOR"]);
	musicSlider->SetOnClickReleaseFct([this]() -> void {
		//audMng.PlaySoundNoSecurity("BUTTON_PRESSED");
		});
	musicSlider->SetWhileClickFct([&]() -> void {
		sf::Vector2f mousePos = (sf::Vector2f)Screen::Get().GetMousePosition(_gameData.window);
		float currentVolume = Maths::Clamp((((mousePos.x - ((Bar*)audioItems[1].get())->getPosition().x) / ((Bar*)audioItems[1].get())->GetRect().width) * ((Bar*)audioItems[1].get())->GetMax()), ((Bar*)audioItems[1].get())->GetMin(), ((Bar*)audioItems[1].get())->GetMax());
		((Bar*)audioItems[1].get())->SetValue(static_cast<const short>(currentVolume));
		});
	musicSlider->SetOnOutFct([&]() -> void {
		((Bar*)audioItems[1].get())->SetOutlineSprite(texMng["BAR_BORDER"]);
		});
	musicSlider->SetOnOverFct([&]() -> void {
		currentId = audioItems[1]->GetID();
		((Bar*)audioItems[1].get())->SetOutlineSprite(texMng["BAR_BORDER_HOVER"]);
		});
	musicSlider->SetOnChangeFct([&]() -> void {
		audMng.SetMusicMultiplier((float)((Bar*)audioItems[1].get())->GetValue() / (float)((Bar*)audioItems[1].get())->GetMax());
		});

	// Vector of commonItems
	audioItems.push_back(std::move(soundSlider));
	audioItems.push_back(std::move(musicSlider));
}

void SettingsMenu::InitControlsSettings(GameData& _gameData)
{
	// Section
	drwMng.AddSection("SETTINGS_CONTROLS");

	// Item create
	Button* resetControls = new Button();
	
	//
	// resetControls
	//
	resetControls->setPosition(416, 956);
	resetControls->SetTextureSprite(texMng["RESET_BUTTON"]);
	resetControls->SetOnClickFct([this, &_gameData]() -> void {
		Audio& audioManager = Audio::Get();
		audioManager.PlaySound("BUTTON_PRESSED");
		ControlsManager::Get().ResetInputs();
		ReloadInput(_gameData);
		});
	resetControls->SetOnOutFct([&]() -> void {
			((Button*)controlsButtons->GetItems().back())->SetTextureSprite(texMng["RESET_BUTTON"]);
		});
	resetControls->SetOnOverFct([&]() -> void {
			((Button*)controlsButtons->GetItems().back())->SetTextureSprite(texMng["RESET_BUTTON_HOVER"]);
			currentId = ((Button*)controlsButtons->GetItems().back())->GetID();
		});
	//
	// Chelouterie
	//
	sf::Vector2f topLeft(416, 124);
	size_t index = 0;
	controlsButtons = std::make_unique<VerticalScrollMenu>();
	controlsButtons->SetSize({ WORKING_WIDTH * WORKING_WIDTH / Screen::Get().GetWidth(), WORKING_HEIGHT * WORKING_HEIGHT / Screen::Get().GetHeight() });
	controlsButtons->SetLooping(false);
	drwMng.AddObject("SETTINGS_CONTROLS", *controlsButtons);

	for (auto& [key, input] : ctrlMng.GetInputs())
	{
		if ((input.isKeyboardOnly && ControlsManager::Get().GetControllerType() == CONTROLLER)
			|| (input.isControllerOnly && ControlsManager::Get().GetControllerType() == KEYBOARD))
			continue;

		const sf::Texture& texture = ctrlMng.GetTexture(key);
		sf::Vector2u textureSize = texture.getSize();
		sf::Vector2f position = topLeft + sf::Vector2f(float(index % 2) * (textureSize.x + 600.f), float((index / 2) * textureSize.y));

		std::string id = key;
		ImageButton* button = new ImageButton();
		std::unique_ptr<sf::Text> text = std::make_unique<sf::Text>();
		
		button->setPosition(position);
		button->SetTexture(texture);
		button->SetOnOverFct([this, id]() -> void {
			waitedInputName = id;
			});
		button->SetOnClickFct([this, id]() -> void {
				waitedInputName = id;
			});

		*text = CreateText(fntMng["SPOOKY"], input.title);
		SetCenteredText(*text);
		text->setOrigin(0.f, text->getOrigin().y);
		text->setPosition(position + sf::Vector2f(static_cast<float>(textureSize.x + 16u), static_cast<float>(textureSize.y * 0.5f)));

		controlsButtons->AddItem(std::move(button));
		drwMng.AddObject("SETTINGS_CONTROLS", *text);
		controlsItems.push_back(std::move(text));

		++index;
	}

	// Vector of controlsItem
	controlsButtons->AddItem(std::move(resetControls));
}

void SettingsMenu::UpdateCommon(GameData& _gameData)
{
	for (auto& item : commonItems)
	{
		if (item->GetID() == currentId)
		{
			item->CallOnOver();
		}
		else
		{
			item->CallOnOut();
		}
	}

	if (ControlsManager::Get().IsPressedDirection("UP"))
	{
		if (--currentId < 0)
		{
			currentId = static_cast<int>(commonItems.size() - 1);
		}
		audMng.PlaySoundNoSecurity("BUTTON_SWAP");
	}
	if (ControlsManager::Get().IsPressedDirection("DOWN"))
	{
		if (++currentId > commonItems.size() - 1)
		{
			currentId = 0;
		}
		audMng.PlaySoundNoSecurity("BUTTON_SWAP");
	}
}

void SettingsMenu::UpdateVideo(GameData& _gameData)
{
	for (auto& item : videoItems)
	{
		item->Update(_gameData);

		if (item->GetID() == currentId)
		{
			item->CallOnOver();
		}
		else
		{
			item->CallOnOut();
		}
	}

	if (ControlsManager::Get().IsPressedDirection("UP"))
	{
		if (--currentId < 4)
		{
			currentId = static_cast<int>(videoItems.size() + 3);
		}
		audMng.PlaySoundNoSecurity("BUTTON_SWAP");
	}
	if (ControlsManager::Get().IsPressedDirection("DOWN"))
	{
		if (++currentId > videoItems.size() + 3)
		{
			currentId = 4;
		}
		audMng.PlaySoundNoSecurity("BUTTON_SWAP");
	}
	if (ControlsManager::Get().IsPressedDirection("LEFT"))
	{
		for (auto& item2 : videoItems)
		{
			if (item2->GetID() == currentId)
			{
				// If item is a select box
				if (item2->className == "select")
				{
					((SelectBox<unsigned int>*)item2.get())->Previous();
				}
				break;
			}
		}
	}
	if (ControlsManager::Get().IsPressedDirection("RIGHT"))
	{
		for (auto& item2 : videoItems)
		{
			if (item2->GetID() == currentId)
			{
				if (item2->className == "select")
				{
					((SelectBox<unsigned int>*)item2.get())->Next();
				}
				break;
			}
		}
	}
}

void SettingsMenu::UpdateAudio(GameData& _gameData)
{
	for (auto& item : audioItems)
	{
		item->Update(_gameData);

		if (item->GetID() == currentId)
		{
			item->CallOnOver();
		}
		else
		{
			item->CallOnOut();
		}
	}

	if (ControlsManager::Get().IsPressedDirection("UP"))
	{
		if (--currentId < 4)
		{
			currentId = static_cast<int>(audioItems.size() + 3);
		}
		audMng.PlaySoundNoSecurity("BUTTON_SWAP");
	}
	if (ControlsManager::Get().IsPressedDirection("DOWN"))
	{
		if (++currentId > audioItems.size() + 3)
		{
			currentId = 4;
		}
		audMng.PlaySoundNoSecurity("BUTTON_SWAP");
	}
	if (ControlsManager::Get().IsPressedDirection("LEFT"))
	{
		for (auto& item2 : audioItems)
		{
			if (item2->GetID() == currentId)
			{
				if (item2->className == "bar")
				{
					((Bar*)item2.get())->SetValue(((Bar*)item2.get())->GetValue() - 5);
				}
				break;
			}
		}
	}
	if (ControlsManager::Get().IsPressedDirection("RIGHT"))
	{
		for (auto& item2 : audioItems)
		{
			if (item2->GetID() == currentId)
			{
				if (item2->className == "bar")
				{
					((Bar*)item2.get())->SetValue(((Bar*)item2.get())->GetValue() + 5);
				}
				break;
			}
		}
	}
}

void SettingsMenu::UpdateControls(GameData& _gameData)
{
	controlsButtons->Update(_gameData);
}

void SettingsMenu::PollEventCommon(GameData& _gameData)
{
	// On keyboard or controller inputs pressed
	if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "UP"))
	{
		if (--currentId < 0)
		{
			currentId = static_cast<int>(commonItems.size() - 1);
		}
		audMng.PlaySoundNoSecurity("BUTTON_SWAP");
	}
	if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "DOWN"))
	{
		if (++currentId > commonItems.size() - 1)
		{
			currentId = 0;
		}
		audMng.PlaySoundNoSecurity("BUTTON_SWAP");
	}
	if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "INTERACT")
		|| ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "INTERACT_2"))
	{
		for (int i = 0; i < commonItems.size(); i++)
		{
			if (commonItems[i]->GetID() == currentId)
			{
				commonItems[i]->CallOnClick();
				if (i != 3)
				{
					audMng.PlaySoundNoSecurity("BUTTON_PRESSED");
				}
				break;
			}
		}
	}
}

void SettingsMenu::PollEventVideo(GameData& _gameData)
{
	for (int i = 0; i < videoItems.size(); i++)
	{
		if (videoItems[i]->GetRect().contains(Screen::Get().GetMousePosition(_gameData.window)) && ControlsManager::Get().GetControllerType() == KEYBOARD && currentId != i + 4)
		{
			currentId = i + 4;
			Audio::Get().PlaySoundNoSecurity("BUTTON_SWAP");
		}
	}

	// Pollevent of all videoItems
	for (int i = 0; i < videoItems.size(); i++)
	{
		videoItems[i]->PollEvent(_gameData);
	}

	// On keyboard or controller inputs pressed
	if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "UP"))
	{
		if (--currentId < 4)
		{
			currentId = static_cast<int>(videoItems.size() + 3);
		}
		audMng.PlaySoundNoSecurity("BUTTON_SWAP");
	}
	if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "DOWN"))
	{
		if (++currentId > videoItems.size() + 3)
		{
			currentId = 4;
		}
		audMng.PlaySoundNoSecurity("BUTTON_SWAP");
	}
	if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "LEFT"))
	{
		for (int i = 0; i < videoItems.size(); i++)
		{
			if (videoItems[i]->GetID() == currentId)
			{
				if (videoItems[i]->className == "select")
				{
					((SelectBox<unsigned int>*)videoItems[i].get())->Previous();
					videoItems[i]->CallOnChange();
				}
				break;
			}
		}
		audMng.PlaySoundNoSecurity("BUTTON_PRESSED");
	}
	if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "RIGHT"))
	{
		for (int i = 0; i < videoItems.size(); i++)
		{
			if (videoItems[i]->GetID() == currentId)
			{
				if (videoItems[i]->className == "select")
				{
					((SelectBox<unsigned int>*)videoItems[i].get())->Next();
					videoItems[i]->CallOnChange();
				}
				break;
			}
		}
		audMng.PlaySoundNoSecurity("BUTTON_PRESSED");
	}
	if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "INTERACT")
		|| ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "INTERACT_2"))
	{
		for (int i = 0; i < videoItems.size(); i++)
		{
			if (videoItems[i]->GetID() == currentId)
			{
				if (videoItems[i]->className == "checkbox")
				{
					((Checkbox*)videoItems[i].get())->SetValue(!((Checkbox*)videoItems[i].get())->IsChecked());
				}
				break;
			}
		}
		audMng.PlaySoundNoSecurity("BUTTON_PRESSED");
	}
}

void SettingsMenu::PollEventAudio(GameData& _gameData)
{
	for (int i = 0; i < audioItems.size(); i++)
	{
		if (audioItems[i]->GetRect().contains(Screen::Get().GetMousePosition(_gameData.window)) && ControlsManager::Get().GetControllerType() == KEYBOARD && currentId != i + 4)
		{
			currentId = i + 4;
			Audio::Get().PlaySoundNoSecurity("BUTTON_SWAP");
		}
	}

	// Pollevent of all audioItems
	for (int i = 0; i < audioItems.size(); i++)
	{
		audioItems[i]->PollEvent(_gameData);
	}

	// On keyboard or controller inputs pressed
	if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "UP"))
	{
		if (--currentId < 4)
		{
			currentId = static_cast<int>(audioItems.size() + 3);
		}
		audMng.PlaySoundNoSecurity("BUTTON_SWAP");
	}
	if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "DOWN"))
	{
		if (++currentId > audioItems.size() + 3)
		{
			currentId = 4;
		}
		audMng.PlaySoundNoSecurity("BUTTON_SWAP");
	}
	if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "LEFT"))
	{
		for (int i = 0; i < audioItems.size(); i++)
		{
			if (audioItems[i]->GetID() == currentId)
			{
				((Bar*)audioItems[i].get())->SetValue(((Bar*)audioItems[i].get())->GetValue() - 5);
				break;
			}
		}
		audMng.PlaySoundNoSecurity("BUTTON_PRESSED");
	}
	if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "RIGHT"))
	{
		for (int i = 0; i < audioItems.size(); i++)
		{
			if (audioItems[i]->GetID() == currentId)
			{
				((Bar*)audioItems[i].get())->SetValue(((Bar*)audioItems[i].get())->GetValue() + 5);
				break;
			}
		}
		audMng.PlaySoundNoSecurity("BUTTON_PRESSED");
	}
}

void SettingsMenu::PollEventControls(GameData& _gameData)
{
	if (IsNewInput(_gameData))
	{
		return;
	}

	// Pollevent of all controlsItems
	controlsButtons->PollEvent(_gameData);
}

void SettingsMenu::EnterInNewSubState(const SubMenuState& _subState)
{
	currentId = 0;

	switch (_subState)
	{
	case SettingsMenu::SubMenuState::NONE:
		switch (currentSubState)
		{
		case SettingsMenu::SubMenuState::VIDEO:
			currentId = (int)SubMenuState::VIDEO - 1;
			break;
		case SettingsMenu::SubMenuState::AUDIO:
			currentId = (int)SubMenuState::AUDIO - 1;
			break;
		case SettingsMenu::SubMenuState::CONTROLS:
			currentId = (int)SubMenuState::CONTROLS - 1;
			break;
		default:
			currentId = 0;
			break;
		}
		currentSubState = SubMenuState::NONE;
		currentUpdate = static_cast<PtrSubState>(&SettingsMenu::UpdateCommon);
		currentPollEvent = static_cast<PtrSubState>(&SettingsMenu::PollEventCommon);
		audMng.PlaySoundNoSecurity("BUTTON_CANCEL");
		break;
	case SettingsMenu::SubMenuState::VIDEO:
		currentSubState = SubMenuState::VIDEO;
		currentUpdate = static_cast<PtrSubState>(&SettingsMenu::UpdateVideo);
		currentPollEvent = static_cast<PtrSubState>(&SettingsMenu::PollEventVideo);
		break;
	case SettingsMenu::SubMenuState::AUDIO:
		currentSubState = SubMenuState::AUDIO;
		currentUpdate = static_cast<PtrSubState>(&SettingsMenu::UpdateAudio);
		currentPollEvent = static_cast<PtrSubState>(&SettingsMenu::PollEventAudio);
		break;
	case SettingsMenu::SubMenuState::CONTROLS:
		currentSubState = SubMenuState::CONTROLS;
		currentUpdate = static_cast<PtrSubState>(&SettingsMenu::UpdateControls);
		currentPollEvent = static_cast<PtrSubState>(&SettingsMenu::PollEventControls);
		break;
	default:
		break;
	}

	for (int i = 0; i < commonItems.size(); i++)
	{
		commonItems[i]->CallOnOut();
	}
}

void SettingsMenu::ReloadInput(GameData& _gameData)
{
	DrawManager::Get().DeleteSection("SETTINGS_CONTROLS");
	waitedInputName.clear();
	controlsItems.clear();
	controlsButtons->Clear();
	
	InitControlsSettings(_gameData);
}

bool SettingsMenu::IsNewInput(GameData& _gameData)
{
	if (waitedInputName.empty())
	{
		return false;
	}

	if (ctrlMng.GetControllerType() == KEYBOARD && _gameData.gameEvent.type == sf::Event::KeyPressed)
	{
		ctrlMng.SetKeyboardTouch(waitedInputName, _gameData.gameEvent.key.code);
		ReloadInput(_gameData);
		return true;
	}
	else if (ctrlMng.GetControllerType() == CONTROLLER && _gameData.gameEvent.type == sf::Event::JoystickButtonPressed)
	{
		ctrlMng.SetControllerButton(waitedInputName, (ControllerButton)_gameData.gameEvent.joystickButton.button);
		ReloadInput(_gameData);
		return true;
	}
	else if (_gameData.gameEvent.type == sf::Event::JoystickConnected || _gameData.gameEvent.type == sf::Event::JoystickDisconnected)
	{
		ReloadInput(_gameData);
		return true;
	}
	return false;
}

void SettingsMenu::Back(GameData& _gameData)
{
	if (videoModified)
	{
		scrMng.Apply();
		scrMng.Reload(_gameData.window);
	}

	videoModified = false;

	isDelete = true;
	EnterInNewSubState(SubMenuState::NONE);
	audMng.PlaySoundNoSecurity("BUTTON_CANCEL");
}


