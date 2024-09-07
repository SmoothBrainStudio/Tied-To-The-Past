#ifndef CIRCULAR_MENU__HPP
#define CIRCULAR_MENU__HPP
#include "State.hpp"
#include "CircularSelect.hpp"
//Copyright 2024 Property of Olivier Maurin.All rights reserved.

class CircularMenu : public State
{
public:
	CircularMenu(sf::Texture& _texture, const int& _nbPieces, const std::vector<std::string>& _tiesName);
	~CircularMenu() override;

	void Init(GameData& _gameData) override;
	void Update(GameData& _gameData) override;
	void PollEvent(GameData& _gameData) override;
	void Display(sf::RenderWindow& _window) override;

	std::unique_ptr<CircularSelect>& GetTieSelector(void);

private:
	std::unique_ptr<CircularSelect> tieSelector = nullptr;
};

#endif // !CIRCULAR_MENU__HPP



