#ifndef TILED__HPP
#define TILED__HPP

#include "JsonManager.hpp"
#include "TexturesManager.hpp"
#include "AnimatedSprite.hpp"

namespace Tiled
{
	constexpr unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
	constexpr unsigned FLIPPED_VERTICALLY_FLAG = 0x40000000;
	constexpr unsigned FLIPPED_DIAGONALLY_FLAG = 0x20000000;
	constexpr unsigned ROTATED_HEXAGONAL_120_FLAG = 0x10000000;

	struct Tileset
	{
		Tileset() = default;
		Tileset(const int& _firstgid, const std::string& _filePath)
		{
			firstgid = _firstgid;
			data = JsonManager::LoadFromFile(_filePath);
		}

		unsigned firstgid = 1;		// first id in map
		Json::Value data;	// all tiles info
	};

	struct GIDInfo
	{
		GIDInfo(const unsigned& _gid)
		{
			flipped_horizontally = (_gid & FLIPPED_HORIZONTALLY_FLAG);
			flipped_vertically = (_gid & FLIPPED_VERTICALLY_FLAG);

			globalTiled = _gid;
			globalTiled &= ~(FLIPPED_HORIZONTALLY_FLAG |
				FLIPPED_VERTICALLY_FLAG |
				FLIPPED_DIAGONALLY_FLAG |
				ROTATED_HEXAGONAL_120_FLAG);
		}

		bool flipped_horizontally;
		bool flipped_vertically;

		unsigned globalTiled;
	};

	// most generic and important variable of a tiled object
	struct Object
	{
		sf::Vector2f pos;
		sf::Vector2f size;

		std::string name;
		std::string type;

		std::string imagePath;
	};

	const int GetTileId(const int& _id, const int& _firstgid);
	Json::Value GetTile(const std::vector<Tileset>& _tilesets, const int& _gid, const int& _firstgid);

	const Json::Value MergeObjectTile(const Json::Value& _object, const GIDInfo& _gidInfo, const Json::Value& _tileObject);

	const Json::Value GetFullObject(const Json::Value& _object, const std::vector<Tileset>& _tilesets);

	sf::Sprite LoadSprite(const Json::Value& _spriteObject);
	AnimatedSprite LoadAnimatedSprite(const Json::Value& _spriteObject);
	const std::string LoadSoundFX(const Json::Value& _objectData);
}

#endif // !TILED__HPP