#ifndef GAMEDATA__HPP
#define GAMEDATA__HPP

#include "Common.hpp"

class GameData
{
public:
	GameData() = default;
	~GameData() = default;

	sf::Event gameEvent = sf::Event();
	sf::RenderWindow window;
	sf::Cursor cursor;
	sf::Cursor cursorGrey;
};

#endif