#ifndef COMMON__HPP
#define COMMON__HPP

#include "Definitions.hpp"

// Commons librairies
#include "AnimatedSprite.hpp"
#include "Text.hpp"
#include "PieChart.hpp"

// Maths librairies
#include "Maths.hpp"
#include "Collisions.hpp"
#include "Easings.hpp"

// Static librairies
#include "Mouse.hpp"
#include "Keyboard.hpp"
#include "Timer.hpp"
#include "FontsManager.hpp"
#include "TexturesManager.hpp"
#include "DrawManager.hpp"
#include "Audio.hpp"
#include "Sprite.hpp"
#include "LightPoint.hpp"
#include "Color.hpp"
#include "TexturesManager.hpp"
#include "ShadersManager.hpp"
#include "ControlsManager.hpp"
#include "JsonManager.hpp"
#include "EventManager.hpp"
#include "CollisionManager.hpp"
#include "BoolManager.hpp"

// Common Functions
std::vector<std::string> GetAllFilesInFolder(const std::string& _pathToFolder);
void DrawPixel(sf::Image& _image, const sf::Vector2i& _pos, const sf::Color& _color);
std::string GetNameWithoutExtension(const std::string& _fileName);
void InitFileTexture(const std::string& _path);
void DeleteFileTexture(const std::string& _path);

static const sf::Vector2f ENEMIES_START_POS[3]{ sf::Vector2f(2125.f, 800.f), sf::Vector2f(2225.f, 950.f), sf::Vector2f(2025.f, 650.f) };
static const sf::Vector2f PLAYER_START_POS(-105.f, 800.f);

static const sf::Vector2f PLAYER_FIGHT_POS(485.f, 800.f);
static const sf::Vector2f ENEMIES_FIGHT_POS[3]{ sf::Vector2f(1595.f, 800.f), sf::Vector2f(1745.f, 950.f), sf::Vector2f(1445.f, 650.f) };

enum CostumeState
{
	BASIC,
	BOXER,
	CANDLE,
	MUSIC,

	NB_COSTUMES
};

#endif