#ifndef TUTO_MENU__HPP
#define TUTO_MENU__HPP

#include "State.hpp"
#include "Tutorial.hpp"
//Copyright 2024 Property of Olivier Maurin.All rights reserved.

class TutoMenu : public State
{
public:
	TutoMenu(GameData& _gameData, const std::vector<std::string>& _tutoTextureKeys, const std::vector<std::string>& _descriptionPaths, const std::string& _tutoID);
	~TutoMenu() override;

	void Init(GameData& _gameData) override;
	void Update(GameData& _gameData) override;
	void PollEvent(GameData& _gameData) override;
	void Display(sf::RenderWindow& _window) override;
	std::unique_ptr<Tutorial>& GetTutoPages(void);

private:
	std::unique_ptr<Tutorial> tutoPages = nullptr;
	std::string tutoID;
};

#endif // !TUTO_MENU__HPP


