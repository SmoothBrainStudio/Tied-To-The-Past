#include "Definitions.hpp"
#include "Warp.hpp"

Warp::Warp(const Json::Value& _obj, const sf::FloatRect& _collideBox)
{
	collision = _collideBox;

	id = _obj["id"].asInt();
	idWarp = _obj["idWarp"].asInt();
	key = _obj["key"].asString();
	boolUnlock = _obj["boolUnlock"].asString();
	boolNeeded = _obj["boolNeeded"].asString();
	jsonPath = "Contents/Data/Tiled/Maps/" + _obj["map"].asString();
	dialogsPath = _obj["dialogs"].asString();
	dialogsPath = dialogsPath.substr(dialogsPath.find("/Dialogs/") + sizeof("/Dialogs/") - 1);
	interactionType = (Player::PhylacteryType)_obj["interactionType"].asInt();

	dialogLock = std::make_unique<Dialog>(dialogsPath);

	const auto& warpName = _obj["name"];
	if (warpName == "INTERACTION")
	{
		type = Type::INTERACTION;
	}
	else if (warpName == "COLLIDE")
	{
		type = Type::COLLIDE;
	}
	else
	{
		type = Type::COLLIDE;
		Debug.warning("No match found for name WARP_BOX : ", warpName);
	}

	DrawManager::Get().AddObject("ExplorationUnSortables", *dialogLock);
}

Warp::Warp(const Warp& _warp)
{
	type = _warp.type;
	collision = _warp.collision;

	dialogsPath = _warp.dialogsPath;

	dialogLock = std::make_unique<Dialog>(*_warp.dialogLock);

	dialogLilboo = _warp.dialogLilboo;

	jsonPath = _warp.jsonPath;
	key = _warp.key;
	boolUnlock = _warp.boolUnlock;
	boolNeeded = _warp.boolNeeded;
	interactionType = _warp.interactionType;

	id = _warp.id;
	idWarp = _warp.idWarp;

	lastCollide = _warp.lastCollide;
	isEnter = _warp.isEnter;

	DrawManager::Get().AddObject("ExplorationUnSortables", *dialogLock);
}

Warp::~Warp(void)
{
	DrawManager::Get().DeleteObject("ExplorationUnSortables", dialogLock.get());
}

void Warp::Event(sf::Event& _event, Player& _player)
{
	dialogLock->PollEvent(_event);

	if (IsColliding(_player) && (dialogLock->IsPlaying() || dialogLock->IsFinish() || _player.GetState() != PlayerState::SPEAK))
	{
		if (ControlsManager::Get().IsPressedInput(_event, "INTERACT") || ControlsManager::Get().IsPressedInput(_event, "INTERACT_2"))
		{
			auto keys = _player.GetKeyIds();

			if ((key.empty() || std::find(keys.begin(), keys.end(), key) != keys.end()) && (boolNeeded.empty() || BoolManager::Get()[boolNeeded]))
			{
				if (!boolUnlock.empty())
				{
					BoolManager::Get()[boolUnlock] = true; // boolUnlock the door on the other side if locked
				}
				isEnter = true;
				_player.SetTp(false);
			}
			else if (!dialogLock->IsPlaying() && !dialogLock->IsFinish() && _player.GetState() != PlayerState::SPEAK && _player.CanTp())
			{
				dialogLock->StartDialog();
				dialogLock->setPosition(_player.getPosition() - sf::Vector2f(0.f, _player.GetGlobalAABB().height));
				dialogLock->ChangeDirection((_player.GetXDir() == 0 ? Dialog::ArrowDirection::LEFT : Dialog::ArrowDirection::RIGHT));

				_player.SetIsSpeaking(true);
				_player.SetEnablePhylactery(false);
			}
		}
	}
}

void Warp::Update(Player& _player)
{
	dialogLock->Update();

	const bool isCollide = IsColliding(_player);
	if (dialogLock->IsPlaying() || dialogLock->IsFinish() || _player.GetState() != PlayerState::SPEAK)
	{
		if (isCollide != lastCollide && isCollide)
		{
			OnEnterWarp(_player);
		}
		else if (isCollide != lastCollide && !isCollide)
		{
			OnExitWarp(_player);
		}

		if (isCollide)
		{
			OnStayWarp(_player);
		}

		lastCollide = isCollide;
	}
}

void Warp::OnEnterWarp(Player& _player)
{
	auto keys = _player.GetKeyIds();
	bool hasKey = (key.empty() || std::find(keys.begin(), keys.end(), key) != keys.end());
	bool hasBoolNeeded = (boolNeeded.empty() || BoolManager::Get()[boolNeeded]);

	_player.SetEnablePhylactery(true, (hasKey && hasBoolNeeded ? Player::PhylacteryType::NORMAL : interactionType));
}

void Warp::OnExitWarp(Player& _player)
{
	_player.SetEnablePhylactery(false);
	_player.SetTp(true);
}

void Warp::OnStayWarp(Player& _player)
{
	const auto keys = _player.GetKeyIds();
	const bool hasKey = (key.empty() || std::find(keys.begin(), keys.end(), key) != keys.end());
	const bool hasBoolNeeded = (boolNeeded.empty() || BoolManager::Get()[boolNeeded]);

	// Type COLLISION
	if (type == Type::COLLIDE)
	{
		if (hasKey && hasBoolNeeded && _player.CanTp())
		{
			if (!boolUnlock.empty())
			{
				BoolManager::Get()[boolUnlock] = true; // boolUnlock the door on the other side if locked
			}
			isEnter = true;
			_player.SetTp(false);
		}
	}

	// Dialog
	if (dialogLock->IsFinish())
	{
		_player.SetIsSpeaking(false);
		_player.SetEnablePhylactery(true, interactionType);
		dialogLock->Reset();
	}
}

const bool Warp::IsColliding(Entity& _entity)
{
	return Collision::AABBs(collision, _entity.GetFeetCollisionBox());
}

const bool Warp::IsEnter(void)
{
	return isEnter;
}
