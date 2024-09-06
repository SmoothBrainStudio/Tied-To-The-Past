#include "Definitions.hpp"
#include "SavePoint.hpp"
#include "CollisionManager.hpp"

SavePoint::SavePoint(const Json::Value& _savePointData)
{
	sprite = Tiled::LoadSprite(_savePointData);
	DrawManager::Get().AddObject("Exploration", sprite);

	collideBox.setSize(sf::Vector2f(sprite.getGlobalBounds().width + 150.f, sprite.getGlobalBounds().height + 50.f));
	collideBox.setOrigin(collideBox.getGlobalBounds().width / 2.f, collideBox.getGlobalBounds().height / 2.f);
	collideBox.setPosition(sprite.getPosition());

	boolNeeded = _savePointData["boolNeeded"].asString();
}

SavePoint::~SavePoint(void)
{
	DrawManager::Get().DeleteObject("Exploration", &sprite);
}

void SavePoint::Event(sf::Event& _event, Player& _player)
{
	if (Collision::AABBs(_player.GetGlobalAABB(), collideBox.getGlobalBounds()) && (boolNeeded.empty() || BoolManager::Get()[boolNeeded]) && _player.GetState() != PlayerState::SPEAK)
	{
		if (ControlsManager::Get().IsHeldInput("INTERACT") || ControlsManager::Get().IsHeldInput("INTERACT_2"))
		{
			_player.SetEnablePhylactery(false);
			EventManager::Get()["GotoSaveMenu"];
		}
	}
}

void SavePoint::Update(Player& _player, NPC_Manager& _npcManager)
{
	if (Collision::AABBs(_player.GetGlobalAABB(), collideBox.getGlobalBounds()) && (boolNeeded.empty() || BoolManager::Get()[boolNeeded]) && _player.GetState() != PlayerState::SPEAK)
	{
		_player.SetEnablePhylactery(true, Player::PhylacteryType::SAVE);
		wasCollide = true;
	}
	else if (wasCollide)
	{
		_player.SetEnablePhylactery(false);
		wasCollide = false;
	}
}