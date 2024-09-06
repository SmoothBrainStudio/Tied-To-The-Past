#include "Definitions.hpp"
#include "CollisionManager.hpp"
#include "Entity.hpp"
#include "Collisions.hpp"

const bool CollisionManager::Check2Entities(Entity& _entity1, Entity& _entity2)
{
	sf::FloatRect collideRect1 = _entity1.GetFeetCollisionBox();
	sf::FloatRect collideRect2 = _entity2.GetFeetCollisionBox();

	return collideRect1.intersects(collideRect2);
}

const bool CollisionManager::Check2Sprites(sf::Sprite& _playerSprite, sf::Sprite& _obstacleSprite)
{
	sf::FloatRect collideRect1 = _playerSprite.getGlobalBounds();
	sf::FloatRect collideRect2 = _obstacleSprite.getGlobalBounds();
	collideRect1.height = FOOT_HEIGHT;
	collideRect1.top += _playerSprite.getGlobalBounds().height - collideRect1.height;

	return (Collision::AABBs(collideRect1, collideRect2));
}

bool CollisionManager::StopMovementOnObstacleEdge(Entity& _entity, const sf::FloatRect& _obstacleRect)
{
	_entity.collideBox.setPosition(_entity.getPosition());

	if (Collision::AABBs(_entity.GetFeetCollisionBox(), _obstacleRect))
	{
		float wallBottom = _obstacleRect.top + _obstacleRect.height;
		float wallRight = _obstacleRect.left + _obstacleRect.width;
		const float collideOffset = _entity.GetSpeed() * GameClock::Get().GetDeltaTime() * SIDE_COLLISION_MULTIPLIER;
		// offset to replace entity out of the boxCollision to avoid boxCollision with close obstacle if moving in its direction
		const float placementOffset = 0.02f;

		if (_entity.getPosition().y - collideOffset < _obstacleRect.top)
		{
			_entity.setPosition(_entity.getPosition().x, _obstacleRect.top - placementOffset);
			_entity.collideBox.setPosition(_entity.getPosition());
		}
		if (_entity.GetFeetCollisionBox().top + collideOffset > wallBottom)
		{
			_entity.setPosition(_entity.getPosition().x, wallBottom + FOOT_HEIGHT + placementOffset);
			_entity.collideBox.setPosition(_entity.getPosition());
		}
		if (_entity.GetBoxRight() - collideOffset < _obstacleRect.left)
		{
			_entity.setPosition(_obstacleRect.left - _entity.GetFeetCollisionBox().width / 2 - placementOffset, _entity.getPosition().y);
			_entity.collideBox.setPosition(_entity.getPosition());
		}
		if (_entity.GetBoxLeft() + collideOffset > wallRight)
		{
			_entity.setPosition(wallRight + _entity.GetFeetCollisionBox().width / 2 + placementOffset, _entity.getPosition().y);
			_entity.collideBox.setPosition(_entity.getPosition());
		}

		return true;
	}

	return false;
}