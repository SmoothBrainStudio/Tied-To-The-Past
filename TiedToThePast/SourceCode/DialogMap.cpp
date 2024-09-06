#include "Definitions.hpp"
#include "DialogMap.hpp"

DialogMap::DialogMap(const Json::Value& _jsonData)
{
	boxCollision = sf::FloatRect(_jsonData["x"].asFloat(), _jsonData["y"].asFloat(), _jsonData["width"].asFloat(), _jsonData["height"].asFloat());

	file = _jsonData["dialogs"].asString();
	file = file.substr(file.find("/Dialogs/") + sizeof("/Dialogs/") - 1);

	pushBack = _jsonData["pushBack"].asBool();
	interaction = _jsonData["interaction"].asBool();
	unique = _jsonData["unique"].asBool();

	type = _jsonData["class"].asString();
	addTie = _jsonData["addTie"].asString();

	boolUnlocked = _jsonData["unlockBool"].asString();
	boolNeeded = _jsonData["boolNeeded"].asString();
	uniqueID = _jsonData["uniqueID"].asString();
	id = std::stoi(_jsonData["mapID"].asString());

	dialogList = std::make_unique<Dialog>(file);
	DrawManager::Get().AddObject("ExplorationUnSortables", *dialogList);
}

DialogMap::~DialogMap(void)
{
	DrawManager::Get().DeleteObject("ExplorationUnSortables", dialogList.get());
}

void DialogMap::Event(sf::Event& _event, Player& _player)
{
	dialogList->PollEvent(_event);

	const bool isboolNeeded = boolNeeded.empty() || BoolManager::Get()[boolNeeded];
	if (isboolNeeded && !BoolManager::Get()[uniqueID] && !dialogList->IsPlaying() && interaction && Collision::AABBs(boxCollision, _player.GetFeetCollisionBox()))
	{
		if (!dialogList->IsFinish() && (ControlsManager::Get().IsPressedInput(_event, "INTERACT") || ControlsManager::Get().IsHeldInput("INTERACT_2")))
		{
			dialogReady = true;
		}

		if (!dialogList->IsPlaying()) _player.SetEnablePhylactery(true);
	}
}

void DialogMap::Update(Player& _player)
{
	coolDown -= GameClock::Get().GetDeltaTime();

	UpdateDialog(_player);

	const bool isboolNeeded = boolNeeded.empty() || BoolManager::Get()[boolNeeded];
	if (isboolNeeded && !BoolManager::Get()[uniqueID])
	{
		if (!dialogList->IsPlaying())
		{
			if (!interaction && coolDown <= 0.f && Collision::AABBs(boxCollision, _player.GetFeetCollisionBox()))
			{
				dialogReady = true;
			}

			if (interaction && Collision::AABBs(boxCollision, _player.GetFeetCollisionBox()))
			{
				wasCollide = true;
				if (!unique) _player.SetEnablePhylactery(true);
			}
			else if (wasCollide)
			{
				wasCollide = false;
				_player.SetEnablePhylactery(false);
			}
		}
	}

	if (dialogList->IsFinish())
	{
		_player.SetIsSpeaking(false);
		if (!unique) _player.SetEnablePhylactery(true);
		dialogList->Reset();
	}
}

void DialogMap::UpdateDialog(Player& _player)
{
	dialogList->Update();

	// update dir (depending on player)
	if (_player.GetXDir())
	{
		dialogList->ChangeDirection(Dialog::ArrowDirection::LEFT);
	}
	else
	{
		dialogList->ChangeDirection(Dialog::ArrowDirection::RIGHT);
	}
}

void DialogMap::StartDialog(Player& _player)
{
	dialogReady = false;
	dialogList->StartDialog();
	_player.SetIsSpeaking(true);
	_player.SetEnablePhylactery(false);

	if (pushBack)
	{
		CollisionManager::StopMovementOnObstacleEdge(_player, boxCollision);
		coolDown = 0.f;
	}
	else
	{
		coolDown = COLLIDE_COOLDOWN;
	}

	if (unique)
	{
		BoolManager::Get()[uniqueID] = true;
	}
	if (!boolUnlocked.empty())
	{
		BoolManager::Get()[boolUnlocked] = true;
	}

	if (!addTie.empty())
	{
		for (auto& tie : Tie::prefabs)
		{
			if (tie.first == addTie)
			{
				_player.AddTie(tie.second);
				_player.SetCostumeState((CostumeState)(_player.GetCostumeState() + 1));
				break;
			}
		}
	}
}

void DialogMap::SetPos(const sf::Vector2f& _pos)
{
	dialogList->setPosition(_pos);
}

const bool& DialogMap::Ready(void) const
{
	return dialogReady;
}

const std::string& DialogMap::GetFile(void) const
{
	return file;
}

const bool& DialogMap::IsPlaying(void)
{
	return dialogList->IsPlaying();
}

const std::string& DialogMap::GetType(void)
{
	return type;
}