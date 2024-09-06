#pragma once
#include "Item.hpp"

class ItemOnGround : public sf::Drawable, public sf::Transformable
{
public:
	ItemOnGround(const sf::Vector2f& _position, const std::string& _itemId);
	~ItemOnGround();

	void Update();

	const std::string& GetItemId() const;
	sf::FloatRect GetRect() const;
	bool IsClose(const sf::Vector2f& _position);

private:
	GameClock& glMng = GameClock::Get();

	constexpr static float DISTANCE_TO_MOVE = 250.f;
	constexpr static float SPEED = 20000.f;

	void draw(sf::RenderTarget& _target, sf::RenderStates _states) const override;

	sf::Sprite sprite;
	sf::CircleShape shadow;
	std::string itemId;
	float time;
};
