#ifndef SAVE_POINT__HPP
#define SAVE_POINT__HPP

#include "Tiled.hpp"
#include "Player.hpp"
#include "NPC_Manager.hpp"

class SavePoint
{
public:
	SavePoint(const Json::Value& _savePointData);
	~SavePoint(void);

	void Event(sf::Event& _event, Player& _player);
	void Update(Player& _player, NPC_Manager& _npcManager);

private:
	static constexpr float DISTANCE_INTERACT = 80.f;

	sf::Sprite sprite;
	sf::RectangleShape collideBox;

	std::string boolNeeded;

	bool wasCollide = false;
};

#endif // !SAVE_POINT__HPP