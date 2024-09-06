#ifndef WALL_RIDDLE__HPP
#define WALL_RIDDLE__HPP

#define LARIO
#include "Player.hpp"
#include "Tiled.hpp"

class WallRiddle
{
public:
	WallRiddle(const Json::Value& _data);

	~WallRiddle();

	void Update(Player& _player);

	const sf::FloatRect GetCollisionBox(void) const;

	const bool& IsDestroyed(void) const;

private:
	std::string uniqueID = "NONE";
	bool destroyed = false;
	sf::Sprite sprite;
	bool wasCollide = false;
};

#endif // !WALL_RIDDLE__HPP