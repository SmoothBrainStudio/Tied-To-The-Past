#pragma once
#include "MenuNPC.hpp"

class SellerNPC : public MenuNPC
{
public:
	SellerNPC(const sf::Vector2f& _pos, const float& _speed, const std::string& _animFile);
	~SellerNPC();
	void Interact(sf::Event& _event, std::shared_ptr<Player>& _player) override;

private:
	constexpr static sf::Int16 MIN_CANDIES_TO_TALK = 0;
};
