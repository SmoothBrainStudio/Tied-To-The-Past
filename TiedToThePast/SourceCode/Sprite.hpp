#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "Definitions.hpp"

sf::Sprite CreateSprite(sf::Texture& _texture);
sf::Sprite CreateSpriteWithAnims(sf::Texture& _texture, const sf::Vector2u& _nbFramesXY, const bool& _isCentered = true);
void CenterSpriteWithAnims(sf::Sprite& _sprite, const sf::Vector2u& _nbFramesXY);
void CenterSprite(sf::Sprite& _sprite);

#endif // !SPRITE_HPP
