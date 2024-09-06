#pragma once
#include "NPC.hpp"

class MenuNPC : public NPC
{
public:
	MenuNPC();
	MenuNPC(const sf::Vector2f& _pos, const float& _speed, const std::string& _animFile);
	void Interact(sf::Event& _event, std::shared_ptr<Player>& _player) override;
	void SetEventKey(const std::string& _key);

	std::string eventKey;
};
