#ifndef IN_GAME_DATAS__HPP
#define IN_GAME_DATAS__HPP

#include "Player.hpp"
#include "EnemyManager.hpp"
#include "NPC_Manager.hpp"
#include "HUD.hpp"
#include "Camera.hpp"
#include "Map.hpp"
#include "CandleRiddle.hpp"
#include "Crate.hpp"
#include "ItemOnGround.hpp"
#include "CircularSelect.hpp"
#include "PropsManager.hpp"
#include "SavePoint.hpp"

struct InGameDatas
{
	void Clear(void);

	// reset all objects involving the map
	void ResetExplorationObjects(void);

	std::unique_ptr<Map> map = nullptr;
	std::shared_ptr<Player> player = nullptr;
	std::unique_ptr<HUD> hud = nullptr;
	std::unique_ptr<Camera> camera = nullptr;

	std::unique_ptr<EnemyManager> enemyManager = nullptr;
	std::unique_ptr<NPC_Manager> npcManager = nullptr;

	std::vector<std::unique_ptr<ItemOnGround>> itemsOnGround;
	std::vector<std::unique_ptr<Crate>> crateVect;

	std::unique_ptr<PropsManager> propsManager;
	std::unique_ptr<SavePoint> savePoint = nullptr;
	std::unique_ptr<CandleRiddle> candleRiddle = nullptr;
	std::vector<std::unique_ptr<WallRiddle>> wallsRiddle;
	std::vector<std::unique_ptr<LightPoint>> lightsPoint;

	bool tutoActive = false;
};

#endif // !IN_GAME_DATAS__HPP

