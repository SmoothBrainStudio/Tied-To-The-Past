#include "Definitions.hpp"
#include "CircularMenu.hpp"
#include "CircularSelect.hpp"

CircularMenu::CircularMenu(sf::Texture& _texture, const int& _nbPieces, const std::vector<std::string>& _tiesName)
{
	DrawManager::Get().AddSection("HUD_CIRCULAR");
	tieSelector = std::make_unique<CircularSelect>(_texture, _nbPieces, _tiesName);
	nextSubState = NULL_STATE;
}

CircularMenu::~CircularMenu()
{
	tieSelector.reset();
	DrawManager::Get().DeleteSection("HUD_CIRCULAR");
}

void CircularMenu::Init(GameData& _gameData)
{
}

void CircularMenu::Update(GameData& _gameData)
{
	tieSelector->Update(_gameData.window);

	if (!tieSelector->GetIsVisible())
	{
		isDelete = true;
	}
}

void CircularMenu::PollEvent(GameData& _gameData)
{
	tieSelector->PollEvent(_gameData.gameEvent);
}

void CircularMenu::Display(sf::RenderWindow& _window)
{
	DrawManager::Get().Draw(_window, "HUD_CIRCULAR");
}

std::unique_ptr<CircularSelect>& CircularMenu::GetTieSelector(void)
{
	return tieSelector;
}
