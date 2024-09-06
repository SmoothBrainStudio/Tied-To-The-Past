#include "InGameDatas.hpp"

void InGameDatas::Clear(void)
{
	player.reset();
	camera.reset();
	hud.reset();
	itemsOnGround.clear();
	map.reset();
	ResetExplorationObjects();
	enemyManager.reset();
	npcManager.reset();
}

void InGameDatas::ResetExplorationObjects(void)
{
	enemyManager->Reset();
	npcManager->Reset();

	itemsOnGround.clear();
	crateVect.clear();

	propsManager->Clear();
	savePoint.reset();
	candleRiddle.reset();
	wallsRiddle.clear();
	lightsPoint.clear();
}
