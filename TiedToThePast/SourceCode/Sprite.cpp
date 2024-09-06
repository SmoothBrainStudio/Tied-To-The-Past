#include "Definitions.hpp"
#include "Sprite.hpp"

sf::Sprite CreateSprite(sf::Texture& _texture)
{
	sf::Sprite result;
	result.setTexture(_texture);
	return result;
}

sf::Sprite CreateSpriteWithAnims(sf::Texture& _texture, const sf::Vector2u& _nbFramesXY, const bool& _isCentered)
{
	sf::Sprite result;
	result.setTexture(_texture);
	if (_isCentered)
	{
		CenterSpriteWithAnims(result, _nbFramesXY);
	}
	return result;
}

void CenterSpriteWithAnims(sf::Sprite& _sprite, const sf::Vector2u& _nbFramesXY)
{
	sf::FloatRect rectSprite = _sprite.getGlobalBounds();
	_sprite.setOrigin(rectSprite.width / 2.f * _nbFramesXY.x, rectSprite.height / 2.f * _nbFramesXY.y);
}

void CenterSprite(sf::Sprite& _sprite)
{
	sf::FloatRect rectSprite = _sprite.getGlobalBounds();
	_sprite.setOrigin(rectSprite.left + rectSprite.width / 2.f, rectSprite.top + rectSprite.height / 2.f);
}