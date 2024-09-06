#include "Definitions.hpp"
#include "Tiled.hpp"

namespace Tiled
{
	const int GetTileId(const int& _id, const int& _firstgid)
	{
		return _id - _firstgid;
	}

	Json::Value GetTile(const std::vector<Tileset>& _tilesets, const int& _gid, const int& _firstgid)
	{
		for (int i = 0; i < _tilesets.size(); i++)
		{
			if (int(_tilesets[i].firstgid) <= _gid && (i == _tilesets.size() - 1 || _gid < int(_tilesets[size_t(i + 1)].firstgid)))
			{
				const int tileId = GetTileId(_gid, _tilesets[i].firstgid);

				for (auto& tile : _tilesets[i].data["tiles"])
				{
					if (tile["id"].asInt() == tileId)
					{
						return tile;
					}
				}

				Debug.error("Can't find tile in tileset with object id \"", _gid, "\"");
				throw;
			}
		}

		Debug.error("Can't get tile object with object id\"", _gid, "\"");
		throw;
	}

	const Json::Value MergeObjectTile(const Json::Value& _object, const GIDInfo& _gidInfo, const Json::Value& _tileObject)
	{
		Json::Value mergeJson;

		for (auto& property : _tileObject["properties"])
		{
			mergeJson[property["name"].asString()] = property["value"];
		}
		for (auto& object : _tileObject["objectgroup"]["objects"])
		{
			mergeJson["collision"].append("collide");
			mergeJson["collision"][mergeJson["collision"].size() - 1] = object;
		}
		// get image path
		std::string filePath = _tileObject["image"].asString();
		mergeJson["image"] = "Contents" + filePath.substr(filePath.find("/Assets/"));

		mergeJson["type"] = _tileObject["type"];

		for (auto& property : _object["properties"])
		{
			mergeJson[property["name"].asString()] = property["value"];
		}
		mergeJson["mapID"] = _object["id"];
		//mergeJson["uniqueID"] = name + mergeJson["mapID"].asString();
		mergeJson["name"] = _object["name"];
		mergeJson["x"] = _object["x"];
		mergeJson["y"] = _object["y"].asFloat() - _object["height"].asFloat();
		mergeJson["width"] = _object["width"];
		mergeJson["height"] = _object["height"];
		mergeJson["horizontal"] = _gidInfo.flipped_horizontally;
		mergeJson["vertical"] = _gidInfo.flipped_vertically;

		return mergeJson;
	}

	const Json::Value GetFullObject(const Json::Value& _object, const std::vector<Tileset>& _tilesets)
	{
		for (int i = 0; i < _tilesets.size(); i++)
		{
			GIDInfo gidInfo(_object["gid"].asUInt());
			if (_tilesets[i].firstgid <= gidInfo.globalTiled && (i == _tilesets.size() - 1 || gidInfo.globalTiled < _tilesets[(size_t)(i + 1)].firstgid))
			{
				const int tileId = GetTileId(gidInfo.globalTiled, _tilesets[i].firstgid);
				for (auto& tile : _tilesets[i].data["tiles"])
				{
					if (tile["id"].asInt() == tileId)
					{
						return MergeObjectTile(_object, gidInfo, tile);
					}
				}

				Debug.error("Can't find tile in tileset with object id \"", _object, "\"");
				throw;
			}
		}

		Debug.error("Can't get tile object \"", _object, "\"");
		throw;
	}

	sf::Sprite LoadSprite(const Json::Value& _spriteObject)
	{
		if (!TexturesManager::Get().IsFileInMap(_spriteObject["image"].asString()))
		{
			TexturesManager::Get().Add(_spriteObject["image"].asString(), _spriteObject["image"].asString());
		}

		sf::Sprite newSprite;
		sf::Vector2f propPos(_spriteObject["x"].asFloat() + _spriteObject["width"].asFloat() / 2.f, _spriteObject["y"].asFloat() + _spriteObject["height"].asFloat());
		
		newSprite.setTexture(TexturesManager::Get()[_spriteObject["image"].asString()]);

		propPos.y -= _spriteObject["vertical"].asBool() ? _spriteObject["height"].asFloat() : 0.f;
		const float vertical = _spriteObject["vertical"].asBool() ? -1.f : 1.f;
		const float horizontal = _spriteObject["horizontal"].asBool() ? -1.f : 1.f;
		newSprite.setScale(horizontal, vertical);
		newSprite.setOrigin(sf::Vector2f(newSprite.getGlobalBounds().width / 2.f, newSprite.getGlobalBounds().height));
		newSprite.setPosition(propPos);

		return newSprite;
	}

	AnimatedSprite LoadAnimatedSprite(const Json::Value& _spriteObject)
	{
		AnimatedSprite newAnimatedSprite;
		sf::Vector2f propPos(_spriteObject["x"].asFloat() + _spriteObject["width"].asFloat() / 2.f, _spriteObject["y"].asFloat() + _spriteObject["height"].asFloat());

		// get animatedImage path
		const std::string filePath = _spriteObject["animation"].asString();
		newAnimatedSprite = AnimatedSprite("Contents/Data/" + filePath.substr(filePath.find("Animations/")));

		propPos.y -= _spriteObject["vertical"].asBool() ? _spriteObject["height"].asFloat() : 0.f;
		const float vertical = _spriteObject["vertical"].asBool() ? -1.f : 1.f;
		const float horizontal = _spriteObject["horizontal"].asBool() ? -1.f : 1.f;
		newAnimatedSprite.setScale(horizontal, vertical);
		newAnimatedSprite.setOrigin(sf::Vector2f(newAnimatedSprite.GetSprite().getGlobalBounds().width / 2.f, newAnimatedSprite.GetSprite().getGlobalBounds().height));
		newAnimatedSprite.setPosition(propPos);

		return newAnimatedSprite;
	}

	const std::string LoadSoundFX(const Json::Value& _objectData)
	{
		std::string soundPath = _objectData["sound"].asString();
		if (soundPath.empty())
		{
			Debug.error("No data found in object to load FX : ", _objectData);
			throw;
		}

		std::string keySound = Audio::GenerateRdmName(15);
		soundPath = "Contents/Assets/" + soundPath.substr(soundPath.find("Audio/"));
		if (Audio::Get().IsFileAlreadyInMap(soundPath))
		{
			Audio::Get().AddSoundWithBuffer(soundPath, keySound);
		}
		else
		{
			Audio::Get().AddSoundAndBuffer(soundPath, soundPath, keySound);
		}
		
		return keySound;
	}
}