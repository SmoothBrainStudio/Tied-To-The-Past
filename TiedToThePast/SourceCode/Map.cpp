#include "Definitions.hpp"
#include "InGameDatas.hpp"
#include "InGame.hpp"
#include "Map.hpp"

void Map::LoadPrefabs(void)
{
	for (const auto& file : std::filesystem::directory_iterator("Contents/Data/Tiled/Maps/"))
	{
		JsonManager::Get().Add(file.path().string(), file.path().string());
	}
}

void Map::DeloadPrefabs(void)
{
	for (const auto& file : std::filesystem::directory_iterator("Contents/Data/Tiled/Maps/"))
	{
		JsonManager::Get().Delete(file.path().string());
	}
}

Map::Map(const std::string& _jsonKey, InGameDatas& _inGameDatas)
{
	DrawManager::Get().AddSection("MAP_BACKGROUND");

	Load(_jsonKey, _inGameDatas);
}

Map::~Map()
{
	TexturesManager::Get().Delete("MAP_BACKGROUND");
	DrawManager::Get().DeleteObject("MAP_BACKGROUND", &background);
	DrawManager::Get().DeleteSection("MAP_BACKGROUND");
	//loadThread.detach();
};

void Map::WarpLoad(InGameDatas* _inGameDatas)
{
	Warp warp = *warpManager.GetWarpCollide(); // make a copy because this pointer will be destroyed in Load()

	Reset(*_inGameDatas);
	Load(warp.jsonPath, *_inGameDatas);

	const Warp& warpNextRoom = warpManager[warp.idWarp];
	const sf::Vector2f newPos(warpNextRoom.collision.left + warpNextRoom.collision.width / 2.f, warpNextRoom.collision.top + warpNextRoom.collision.height / 2.f);
	_inGameDatas->player->setPosition(newPos);
	_inGameDatas->camera->SetCenterWithBorders(sf::Vector2f(newPos.x, newPos.y - _inGameDatas->player->GetGlobalAABB().height / 2.f));
}

void Map::Reset(InGameDatas& _inGameDatas)
{
	DrawManager& drawMng = DrawManager::Get();

	TexturesManager::Get().Delete("MAP_BACKGROUND");
	drawMng.DeleteObject("MAP_BACKGROUND", &background);
	drawMng.DeleteSection("MAP_BACKGROUND");
	drawMng.AddSection("MAP_BACKGROUND");

	collisions.clear();
	warpManager.Clear();
	enemiesType.clear();

	dialogMng.Clear();
	_inGameDatas.ResetExplorationObjects();
}

void Map::Load(const std::string& _jsonPath, InGameDatas& _inGameDatas)
{
	name = _jsonPath;
	const auto& mapJson = JsonManager::Get()[_jsonPath];

	_inGameDatas.camera->SetBorders(sf::Vector2f(0.f, 0.f), sf::Vector2f(mapJson["width"].asFloat() * TILE_SIZE, mapJson["height"].asFloat() * TILE_SIZE));

	// Load every tilesets used in map
	TilesetsArr tilesets;
	for (const auto& tileset : mapJson["tilesets"])
	{
		std::string filePath = tileset["source"].asString();
		filePath = ("Contents/Data/Tiled" + filePath.substr(filePath.find("/Tilesets/")));

		tilesets.push_back(Tiled::Tileset(tileset["firstgid"].asInt(), filePath));
	}

	for (const auto& layer : mapJson["layers"])
	{
		const auto& layerName = layer["name"].asString();

		if (layerName == "Boxes")
		{
			LoadBoxes(layer, tilesets, _inGameDatas);
		}
		else if (layerName == "Enemies")
		{
			LoadEnemies(layer, tilesets, *_inGameDatas.enemyManager);
		}
		else if (layerName == "Npcs")
		{
			LoadNpcs(layer, tilesets, *_inGameDatas.npcManager);
		}
		else if (layerName == "Entities")
		{
			LoadEntities(layer, tilesets, _inGameDatas);
		}
		else if (layerName == "Props")
		{
			LoadProps(layer, tilesets, _inGameDatas);
		}
		else if (layerName == "BackgroundProps")
		{
			LoadBackgroundProps(layer, tilesets, _inGameDatas);
		}
		else if (layerName == "Data")
		{
			for (auto& data : layer["objects"])
			{
				auto& dataType = data["name"];
				if (dataType == "SPAWN_POINT")
				{
					Player& player = *_inGameDatas.player;
					_inGameDatas.player->setPosition(data["x"].asFloat(), data["y"].asFloat());
					_inGameDatas.camera->SetCenterWithBorders(sf::Vector2f(player.getPosition().x, player.getPosition().y - player.GetGlobalAABB().height / 2.f));
				}
				else if (data["type"] == "LIGHT_POINT")
				{
					std::unique_ptr<LightPoint> light = std::make_unique<LightPoint>(data["properties"][0]["value"].asFloat(), data["properties"][1]["value"].asFloat());
					light->setPosition(data["x"].asFloat(), data["y"].asFloat());
					light->Reset();
					_inGameDatas.lightsPoint.push_back(std::move(light));
				}
			}
		}
		else if (layerName == "Background" && layer["image"].asString() != "") // check if there a background or not in the json
		{
			TexturesManager& texturesMng = TexturesManager::Get();
			const std::string imagePath = layer["image"].asString();
			const std::string imageCombatPath = layer["properties"][0]["value"].asString();
			const sf::Vector2f offSet(layer["offsetx"].asFloat(), layer["offsety"].asFloat());

			texturesMng.Add("Contents" + imagePath.substr(imagePath.find("/Assets/")), "MAP_BACKGROUND");
			DrawManager::Get().AddObject("MAP_BACKGROUND", background);

			texturesMng["BACKGROUND_FIGHTING"].loadFromFile("Contents" + imageCombatPath.substr(imageCombatPath.find("/Assets/")));

			background.setTexture(texturesMng["MAP_BACKGROUND"], true);
			background.setPosition(offSet);

			_inGameDatas.camera->SetBorders(offSet, sf::Vector2f(background.getTexture()->getSize()) + offSet);
		}
	}
}

void Map::LoadBoxes(const Json::Value& _mapData, const TilesetsArr& _tilesets, InGameDatas& _inGameDatas)
{
	for (const auto& data : _mapData["objects"])
	{
		Json::Value boxJson = Tiled::GetFullObject(data, _tilesets);
		const auto& boxType = boxJson["type"];

		// les objets dans tiled ont leur origine en bas à gauche d'où le y - height pour le top
		sf::FloatRect collideBox(boxJson["x"].asFloat(), boxJson["y"].asFloat(), boxJson["width"].asFloat(), boxJson["height"].asFloat());

		AddCollision(boxJson);
		/*else if (boxType == "EVENT_BOX")
		{
		}*/
		if (boxType == "COLLIDE_BOX")
		{
			collisions.push_back(collideBox);
		}
		else if (boxType == "WARP_BOX")
		{
			warpManager.AddWarp(Warp(boxJson, collideBox));
		}
		else if (boxType == "REWORK_DIALOG_BOX")
		{
			boxJson["uniqueID"] = name + boxJson["mapID"].asString();
			dialogMng.AddDialog(boxJson);
		}
	}
}

void Map::LoadEnemies(const Json::Value& _enemiesData, const TilesetsArr& _tilesets, EnemyManager& _enemyManager)
{
	for (auto& data : _enemiesData["objects"])
	{
		Json::Value enemyJson = Tiled::GetFullObject(data, _tilesets);
		const auto& enemyName = enemyJson["type"].asString();

		if (enemyJson["unique"].asBool())
		{
			enemyJson["uniqueID"] = name + enemyJson["mapID"].asString();
		}

		if (enemyJson["uniqueID"].empty() || !BoolManager::Get()[enemyJson["uniqueID"].asString()])
		{
			// check if an enemy of the this type is already load or not and put in enemiesType vector if not
			if (std::find(enemiesType.begin(), enemiesType.end(), enemyName) == enemiesType.end())
			{
				enemiesType.push_back(enemyName);
			}

			_enemyManager.AddEnemyToMap(enemyJson);
		}
	}
}

void Map::LoadNpcs(const Json::Value& _mapData, const TilesetsArr& _tilesets, NPC_Manager& _npcManager)
{
	for (const auto& data : _mapData["objects"])
	{
		Json::Value npcJson = Tiled::GetFullObject(data, _tilesets);
		const std::string type = npcJson["type"].asString();

		if (type == "LIL_BOO" && !BoolManager::Get()[name + npcJson["mapID"].asString()])
		{
			npcJson["uniqueID"] = name + npcJson["mapID"].asString();
			dialogMng.SetLilboo(npcJson);
			BoolManager::Get()[name + npcJson["mapID"].asString()] = true;
		}
		else if (type == "SHOP")
		{
			sf::Vector2f pos(npcJson["x"].asFloat() + npcJson["width"].asFloat() / 2.f, npcJson["y"].asFloat() + npcJson["height"].asFloat());
			_npcManager.AddNPC<SellerNPC>(pos, 100.f, "Alejandro/OnMap.json");
		}
	}
}

void Map::LoadProps(const Json::Value& _propsData, const TilesetsArr& _tilesets, InGameDatas& _inGameDatas)
{
	for (const auto& data : _propsData["objects"])
	{
		const auto propJson = Tiled::GetFullObject(data, _tilesets);
		AddCollision(propJson);
		_inGameDatas.propsManager->Add(propJson, "Exploration");
	}
}
void Map::LoadBackgroundProps(const Json::Value& _propsData, const TilesetsArr& _tilesets, InGameDatas& _inGameDatas)
{
	for (const auto& data : _propsData["objects"])
	{
		const auto propJson = Tiled::GetFullObject(data, _tilesets);
		_inGameDatas.propsManager->Add(propJson, "MAP_BACKGROUND", true);
	}
}

void Map::LoadEntities(const Json::Value& _entitiesData, const TilesetsArr& _tilesets, InGameDatas& _inGameDatas)
{
	std::vector<Json::Value> candles;

	for (const auto& data : _entitiesData["objects"])
	{
		Json::Value entityJson = Tiled::GetFullObject(data, _tilesets);
		const auto& boxType = entityJson["type"];
		AddCollision(entityJson);

		const auto& entityName = entityJson["type"].asString();

		if (entityName == "ITEM_BOX" && !BoolManager::Get()[name + entityJson["mapID"].asString()])
		{
			entityJson["uniqueID"] = name + entityJson["mapID"].asString();
			_inGameDatas.crateVect.push_back(std::make_unique<Crate>(entityJson));
		}
		else if (entityName == "CANDLE" && !BoolManager::Get()[entityJson["name"].asString()])
		{
			candles.push_back(entityJson);
			std::sort(candles.begin(), candles.end(), [](const Json::Value& _candle1, const Json::Value& _candle2) -> bool { return _candle1["order"].asInt() < _candle2["order"].asInt(); });
		}
		else if (entityName == "WALL" && !BoolManager::Get()[name + entityJson["mapID"].asString()])
		{

			entityJson["uniqueID"] = name + entityJson["mapID"].asString();
			_inGameDatas.wallsRiddle.push_back(std::make_unique<WallRiddle>(entityJson));
		}
		else if (entityName == "SAVE")
		{
			_inGameDatas.savePoint = std::make_unique<SavePoint>(entityJson);
		}
	}

	if (candles.size())
	{
		_inGameDatas.candleRiddle = std::make_unique<CandleRiddle>(candles, _inGameDatas.camera->GetBorders());
	}
}

void Map::Event(sf::Event& _event, InGameDatas& _inGameDatas)
{
	dialogMng.Event(_event, *_inGameDatas.player);
	warpManager.PollEvent(_event, *_inGameDatas.player);
}

void Map::Update(InGameDatas& _inGameDatas)
{
	for (auto& wall : _inGameDatas.wallsRiddle)
	{
		wall->Update(*_inGameDatas.player);
	}

	dialogMng.Update(*_inGameDatas.player);

	//lilBoo.Update(_inGameDatas);

	warpManager.Update(*_inGameDatas.player);

	if (warpManager.GetWarpCollide())
	{
		// multithread :sunglasses:
		LoadingScreen::Get().SetLoading(true);
		WarpLoad(&_inGameDatas);
		LoadingScreen::Get().SetLoading(false);
	}
}

void Map::AddCollision(const Json::Value& _object)
{
	for (const auto& collide : _object["collision"])
	{
		if (collide["type"] == "COLLIDE_BOX")
		{
			sf::FloatRect collideBox(_object["x"].asFloat() + collide["x"].asFloat(), _object["y"].asFloat() + collide["y"].asFloat(), collide["width"].asFloat(), collide["height"].asFloat());
			collisions.push_back(collideBox);
		}
	}
}

const std::vector<std::string>& Map::GetEnemiesName() const
{
	return enemiesType;
}

const Map::CollisionBoxArr& Map::GetCollisions() const
{
	return collisions;
}

const std::string& Map::GetName() const
{
	return name;
}