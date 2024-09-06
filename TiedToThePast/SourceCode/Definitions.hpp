#ifndef DEFINITIONS__HPP
#define DEFINITIONS__HPP

// other lib
#include <json/json.h>

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

// visual studio lib
#include <iostream>
#include <memory>
#include <functional>
#include <time.h>
#include <math.h>
#include <vector>
#include <array>
#include <deque>
#include <unordered_map>
#include <fstream>
#include <filesystem>
#include <cctype>
#include <thread>
#include <mutex>

// SFML lib
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

// DEBUG
#include "Debuger.hpp"
#define Debug DebugManager::Get()

// constants
static constexpr float PI = 3.1415927f;
static constexpr const char* PREFERENCES_PATH = "Contents/Data/Preferences/Preferences.json";


struct CameraBorder
{
	CameraBorder() = default;
	CameraBorder(const float& _offsetX, const float& _offsetY, const float& _maxX, const float& _maxY)
	{
		// alors envoie en référence ça fait 16 bit de trop sinon :nerd:
		offsetX = _offsetX;
		maxX = _maxX;

		offsetY = _offsetY;
		maxY = _maxY;
	}
	CameraBorder(const sf::Vector2f& _offset, const sf::Vector2f& _max)
	{
		// alors envoie en référence ça fait 16 bit de trop sinon :nerd:
		offsetX = _offset.x;
		maxX = _max.x;

		offsetY = _offset.y;
		maxY = _max.y;
	}

	float offsetX;
	float maxX;

	float offsetY;
	float maxY;
};

enum AttackType
{
    FIRE,
    PHYSIC,
	MAGIC,
    NB_ATTACK_TYPES
};

#endif // !DEFINITIONS__HPP