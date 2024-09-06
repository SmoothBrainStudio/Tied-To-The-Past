#include "EnemyManager.hpp"
#include "InGameDatas.hpp"

EnemyManager::EnemyManager()
{

}

void EnemyManager::Reset(void)
{
	for (auto& enemy : enemiesVector)
	{
		DrawManager::Get().DeleteObject("Exploration", enemy.get());
		enemy->~Enemy();
		enemy.reset();
	}

	enemiesVector.clear();
}

void EnemyManager::PollEvent(sf::Event& _event)
{
	
}

void EnemyManager::Update(InGameDatas& _inGameDatas)
{
	for (auto& it : enemiesVector)
	{
		it->Update(_inGameDatas);
	}
	ResolveEnemyPlayerCollision(_inGameDatas.player);
}

void EnemyManager::AddEnemyToMap(const Json::Value& _jsonFile)
{
	std::shared_ptr<Enemy> newEnemy = std::make_shared<Enemy>(_jsonFile);
	enemiesVector.push_back(std::move(newEnemy));
}

std::vector<std::shared_ptr<Enemy>>& EnemyManager::GetEnemiesVector(void)
{
	return enemiesVector;
}

void EnemyManager::EraseDeadEnemies()
{
	for (int i = static_cast<int>(enemiesVector.size()) - 1; i >= 0 ; i--)
	{
		if (!enemiesVector[i]->IsAlive())
		{
			enemiesVector[i]->~Enemy();
			enemiesVector[i].reset();
			std::swap(enemiesVector[i], enemiesVector.back());
			enemiesVector.pop_back();
		}
		else if (!enemiesVector[i])
		{
			enemiesVector[i].reset();
			std::swap(enemiesVector[i], enemiesVector.back());
			enemiesVector.pop_back();
		}
	}
}

bool EnemyManager::ResolveEnemyPlayerCollision(std::shared_ptr<Player>& _player)
{
	for (auto& it : enemiesVector)
	{
		if (CollisionManager::Check2Entities(*_player, *it))
		{
			return true;
		}
	}
	return false;
}

bool EnemyManager::ResolveEnemyPlayerCollision(std::shared_ptr<Player>& _player, std::shared_ptr<Enemy>& _collide)
{
	for (auto& it : enemiesVector)
	{
		if (CollisionManager::Check2Entities(*_player, *it))
		{
			_collide = it;
			return true;
		}
	}

	return false;
}

Enemy EnemyManager::CopyPrefabEnemyFighting(const std::string& _prefabKey)
{
	Enemy enemy = *Enemy::prefabs.find(_prefabKey)->second;
	return enemy;
}
