#include "Definitions.hpp"
#include "WarpManager.hpp"

Warp& WarpManager::operator[](const int& _id)
{
	for (auto& warp : warps)
	{
		if (warp.id == _id)
		{
			return warp;
		}
	}
	
	Debug.error("Can't find warp with id ", _id);
	throw;
}

void WarpManager::Clear()
{
	warps.clear();
}

void WarpManager::AddWarp(const Warp& _warp)
{
	warps.push_back(_warp);
}

void WarpManager::PollEvent(sf::Event& _event, Player& _player)
{
	for (auto& warp : warps)
	{
		warp.Event(_event, _player);
	}
}

void WarpManager::Update(Player& _player)
{
	for (auto& warp : warps)
	{
		warp.Update(_player);
	}
}

Warp* WarpManager::GetWarpCollide()
{
	for (auto& warp : warps)
	{
		if (warp.IsEnter())
		{
			return &warp;
		}
	}

	return nullptr;
}