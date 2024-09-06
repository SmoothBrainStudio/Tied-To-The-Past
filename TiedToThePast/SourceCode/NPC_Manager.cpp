#include "Definitions.hpp"
#include "NPC_Manager.hpp"

void NPC_Manager::Reset()
{
	for (auto& npc : NPCVector)
	{
		DrawManager::Get().DeleteObject("Exploration", npc.get());
	}
	NPCVector.clear();
}

void NPC_Manager::Update(std::shared_ptr<Player>& _player)
{
	const float dt = GameClock::Get().GetDeltaTime();
	for (auto& it : NPCVector)
	{
		it->Update(_player);
		UpdateSpeakingPossibility(it, _player);
	}
	ResolveNPCPlayerCollision(_player);
	DrawOrNotPhilactery(_player);
}

void NPC_Manager::Interact(sf::Event& _event, std::shared_ptr<Player>& _player)
{
	for (auto& it : NPCVector)
	{
		it->Interact(_event, _player);
	}
}

std::vector<std::shared_ptr<NPC>>& NPC_Manager::GetNPCVector(void)
{
	return NPCVector;
}

void NPC_Manager::ResolveNPCPlayerCollision(std::shared_ptr<Player>& _player)
{
	for (auto& it : NPCVector)
	{
		if (it->CanCollide())
		{
			CollisionManager::StopMovementOnObstacleEdge(*_player, it->GetFeetCollisionBox());
		}
	}
}

void NPC_Manager::UpdateSpeakingPossibility(std::shared_ptr<NPC>& _NPC, std::shared_ptr<Player>& _player)
{
	if (Maths::GetSquaredLength(_NPC->getPosition(), _player->getPosition()) < DISTANCE_FOR_SPEAK * DISTANCE_FOR_SPEAK * 10)
	{
		_NPC->SetCanSpeak(true);
		_player->SetEnablePhylactery(true);
		wasCollide = true;
	}
	else if (wasCollide)
	{
		_NPC->SetCanSpeak(false);
		_player->SetEnablePhylactery(false);
		wasCollide = false;
	}
}

const bool NPC_Manager::IsAnyOneCanSpeak(void) const
{
	for (auto& it : NPCVector)
	{
		if (it->GetIfCanSpeak())
		{
			return true;
		}
	}
	return false;
}

void NPC_Manager::DrawOrNotPhilactery(std::shared_ptr<Player>& _player)
{
	//if (IsAnyOneCanSpeak())
	//{
	//	DrawManager::Get().SetObjectVisible("ExplorationUnSortables", &_player->GetPhylactery(), true);
	//}
	//else
	//{
	//	DrawManager::Get().SetObjectVisible("ExplorationUnSortables", &_player->GetPhylactery(), false);
	//}
}
