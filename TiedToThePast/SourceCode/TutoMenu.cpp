#include "Definitions.hpp"
#include "TutoMenu.hpp"

TutoMenu::TutoMenu(GameData& _gameData, const std::vector<std::string>& _tutoTextureKeys, const std::vector<std::string>& _descriptionPaths, const std::string& _tutoID)
{
	tutoID = _tutoID;
	DrawManager::Get().AddSection("TUTO_" + tutoID);
	tutoPages = std::make_unique<Tutorial>(_tutoTextureKeys, _descriptionPaths ,"TUTO_" + tutoID);

	DrawManager::Get().AddObject("TUTO_" + tutoID, *tutoPages);
	nextSubState = NULL_STATE;
}

TutoMenu::~TutoMenu()
{
	tutoPages.reset();
	DrawManager::Get().DeleteSection("TUTO_" + tutoID);
}

void TutoMenu::Init(GameData& _gameData)
{
}

void TutoMenu::Update(GameData& _gameData)
{
	tutoPages->Update(_gameData);
}

void TutoMenu::PollEvent(GameData& _gameData)
{
	tutoPages->PollEvent(_gameData);
}

void TutoMenu::Display(sf::RenderWindow& _window)
{
	DrawManager::Get().Draw(_window, "TUTO_" + tutoID);
}

std::unique_ptr<Tutorial>& TutoMenu::GetTutoPages(void)
{
	return tutoPages;
}
