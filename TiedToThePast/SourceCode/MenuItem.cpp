#include "MenuItem.hpp"

MenuItem::~MenuItem()
{
}

void MenuItem::Update(GameData& _gameData) 
{
}

void MenuItem::PollEvent(GameData& _gameData) 
{
}

void MenuItem::SetEnable(bool _bool) 
{ 
	enable = _bool;
}

void MenuItem::SetVisible(bool _bool) 
{ 
	visible = _bool; 
}

void MenuItem::SetID(const unsigned int& _id)
{
	id = _id;
}

bool MenuItem::GetEnable() 
{
	return enable; 
}

bool MenuItem::GetVisible() 
{
	return visible;
}

const unsigned int MenuItem::GetID()
{
	return id;
}

sf::FloatRect MenuItem::GetRect() const 
{ 
	return { 0.f, 0.f, 0.f, 0.f }; 
}

const bool& MenuItem::GetWasOver(void) const 
{
	return wasOver; 
}