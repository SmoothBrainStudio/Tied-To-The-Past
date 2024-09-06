#ifndef COLLISION_MANAGER__HPP
#define COLLISION_MANAGER__HPP

#include "Definitions.hpp"
static constexpr float FOOT_HEIGHT = 25.f;
static constexpr float SIDE_COLLISION_MULTIPLIER = 1.1f;

class Entity;

class CollisionManager
{
public:
	static const bool Check2Entities(Entity& _entity1, Entity& _entity2);
	static const bool Check2Sprites(sf::Sprite& _playableEntity, sf::Sprite& _sprite);

	// Replace on the edge of a rectangle if _entity collide _obstacleRect
	static bool StopMovementOnObstacleEdge(Entity& _entity, const sf::FloatRect& _obstacleRect);
};

#endif // !COLLISION_MANAGER__HPP