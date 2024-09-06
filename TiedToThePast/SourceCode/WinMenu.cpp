#include "Definitions.hpp"
#include "WinMenu.hpp"

WinMenu::WinMenu()
{
	TexturesManager::Get().Add("Contents/Assets/Sprites/UI/Backgrounds/GameWin.png", "WIN_BG");
	TexturesManager::Get().Add("Contents/Assets/Sprites/UI/Backgrounds/GameWinTitle.png", "WIN_TITLE");
}

WinMenu::~WinMenu()
{
	delete fade;
	fade = nullptr;

	DrawManager::Get().DeleteSection("WIN");
	TexturesManager::Get().Delete("WIN_BG");
	TexturesManager::Get().Delete("WIN_TITLE");
}

void WinMenu::Init(GameData& _gameData)
{
	TexturesManager& texturesManager = TexturesManager::Get();
	DrawManager& drawManager = DrawManager::Get();

	drawManager.AddSection("WIN");

	background.setTexture(texturesManager["WIN_BG"]);
	drawManager.AddObject("WIN", background);

	title.setTexture(texturesManager["WIN_TITLE"]);
	drawManager.AddObject("WIN", title);
	title.setPosition(startTitlePos);

	isFading = true;
	fade = new Fade(2.f, 100.f, false, true);
	audioMng.PlayMusic("GAME_WIN_MUSIC", true, 0.f);
	drawManager.AddObject("WIN", *fade);
}

void WinMenu::Update(GameData& _gameData)
{
	title.setPosition(Easings::Lerp(startTitlePos, endTitlePos, std::min(Easings::OutBounce(timerTitle), 1.f)));

	timerTitle += GameClock::Get().GetDeltaTime() / 3.f;

	if (isFading)
	{
		fade->Update(GameClock::Get().GetDeltaTime(), "GAME_WIN_MUSIC");
		if (fade->GetTimer() >= fade->GetTimerMax())
		{
			SceneManager::Get().LoadScene(new Introduction);
			isFading = false;
		}
		else if (fade->GetTimer() <= fade->GetTimerMin())
		{
			isFading = false;
		}
	}
}

void WinMenu::PollEvent(GameData& _gameData)
{
	if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "INTERACT") ||
		ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "INTERACT_2"))
	{
		fade->SetIsFadingIn(true);
		isFading = true;
	}
}

void WinMenu::Display(sf::RenderWindow& _window)
{
	DrawManager::Get().Draw(_window, "WIN");
}
