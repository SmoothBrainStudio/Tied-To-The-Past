#ifndef MAP__HPP
#define MAP__HPP

#include "Tiled.hpp"

#include "DrawManager.hpp"
#include "EventManager.hpp"
#include "LoadingScreen.hpp"

#include "EnemyManager.hpp"
#include "NPC_Manager.hpp"
#include "DialogManager.hpp"
#include "WarpManager.hpp"

#include "Player.hpp"
#include "WallRiddle.hpp"

struct InGameDatas;

class Map
{
private:
	static constexpr float TILE_SIZE = 56.f;

	using CollisionBoxArr = std::vector<sf::FloatRect>;
	using TilesetsArr = std::vector<Tiled::Tileset>;

	std::string name;
	CollisionBoxArr collisions;
	sf::Sprite background;

	std::vector<std::string> enemiesType;

	WarpManager warpManager;

	DialogManager dialogMng;

	// Chargement de la nouvelle map
	void WarpLoad(InGameDatas* _inGameDatas);

public:
	static void LoadPrefabs(void);
	static void DeloadPrefabs(void);

	Map(const std::string& _jsonKey, InGameDatas& _inGameDatas);
	// Unload and Load everything we need in game datas and exploration state
	void Load(const std::string& _jsonKey, InGameDatas& _inGameDatas);

	void Reset(InGameDatas& _inGameDatas);
	~Map();

	void LoadBoxes(const Json::Value& _mapData, const TilesetsArr& _tilesets, InGameDatas& _inGameDatas);
	void LoadEnemies(const Json::Value& _enemiesData, const TilesetsArr& _tilesets, EnemyManager& _enemyManager);
	void LoadNpcs(const Json::Value& _mapData, const TilesetsArr& _tilesets, NPC_Manager& _npcManager);

	void LoadProps(const Json::Value& _propsData, const TilesetsArr& _tilesets, InGameDatas& _inGameDatas);
	void LoadBackgroundProps(const Json::Value& _propsData, const TilesetsArr& _tilesets, InGameDatas& _inGameDatas);
	void LoadEntities(const Json::Value& _entitiesData, const TilesetsArr& _tilesets, InGameDatas& _inGameDatas);

	void Event(sf::Event& _event, InGameDatas& _inGameDatas);
	void Update(InGameDatas& _inGameDatas);

	void AddCollision(const Json::Value& _object);

	const std::vector<std::string>& GetEnemiesName(void) const;
	const CollisionBoxArr& GetCollisions(void) const;
	const std::string& GetName(void) const;
};

#endif // !MAP__HPP