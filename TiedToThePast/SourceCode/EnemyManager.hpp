#ifndef ENEMY_MANAGER__HPP
#define ENEMY_MANAGER__HPP

#include "Common.hpp"
#include "Enemy.hpp"

class EnemyManager
{
public:

	EnemyManager();
	~EnemyManager() = default;

	void Reset(void);

	void PollEvent(sf::Event& _event);
	void Update(InGameDatas& _inGameDatas);
	// add enemy for map
	void AddEnemyToMap(const Json::Value& _jsonFile);
	std::vector<std::shared_ptr<Enemy>>& GetEnemiesVector(void);
	void EraseDeadEnemies();

	bool ResolveEnemyPlayerCollision(std::shared_ptr<Player>& _player);
	bool ResolveEnemyPlayerCollision(std::shared_ptr<Player>& _player, std::shared_ptr<Enemy>& _collide);

	Enemy CopyPrefabEnemyFighting(const std::string& _prefabKey);

private:
	std::vector<std::shared_ptr<Enemy>> enemiesVector;
};

#endif // !ENEMY_MANAGER__HPP