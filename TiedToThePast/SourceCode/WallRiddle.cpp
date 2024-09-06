#include "Definitions.hpp"
#include "WallRiddle.hpp"

WallRiddle::WallRiddle(const Json::Value& _data)
{
	uniqueID = _data["uniqueID"].asString();
	sprite = Tiled::LoadSprite(_data);
	DrawManager::Get().AddObject("Exploration", sprite);
	Audio::Get().AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Player/PunchWall.wav", "PUNCH_WALL");
}

WallRiddle::~WallRiddle()
{
	DrawManager::Get().DeleteObject("Exploration", &sprite);
	Audio::Get().DeleteSoundAndBuffer("PUNCH_WALL");

}

void WallRiddle::Update(Player& _player)
{
	if (_player.GetCostumeState() == CostumeState::BOXER && _player.GetCurTie()->GetLevel() > 1 &&
		Collision::AABBs(_player.GetPunchCollider().getGlobalBounds(), GetCollisionBox()) &&
		_player.GetIsPunching() && !destroyed && _player.GetIsInPunchingFrame())
	{
		destroyed = true;
		BoolManager::Get()[uniqueID] = true;
		Audio::Get().PlaySound("PUNCH_WALL");
		DrawManager::Get().DeleteObject("Exploration", &sprite); // delete sprite
	}

	// If player collision
	if (Maths::GetSquaredLength(_player.getPosition(), sprite.getPosition()) < 150 * 150 && 
		_player.GetCostumeState() == CostumeState::BOXER &&
		_player.GetCurTie()->GetLevel() > 1 &&
		!destroyed)
	{
		_player.SetEnablePhylactery(true, Player::PhylacteryType::WALL);
		wasCollide = true;
	}
	else if (wasCollide)
	{
		_player.SetEnablePhylactery(false);
		wasCollide = false;
	}
}

const sf::FloatRect WallRiddle::GetCollisionBox(void) const
{
	return sprite.getGlobalBounds();
}

const bool& WallRiddle::IsDestroyed(void) const
{
	return destroyed;
}