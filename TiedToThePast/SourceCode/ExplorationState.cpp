#include "Definitions.hpp"
#include "ExplorationState.hpp"
#include "LoadingScreen.hpp"

ExplorationState::~ExplorationState()
{
	audMng.DeleteBuffer("ZOMBIE_PATROL");
	audMng.DeleteBuffer("ZOMBIE_PATROL_FAST");
	audMng.DeleteBuffer("KNIGHT_WALK");
	audMng.DeleteBuffer("VAMPIRE_WALK");
	audMng.DeleteBuffer("CANDLE_BOSS_IDLE");
	audMng.DeleteBuffer("FRANKENCHIEF_CUT");
	audMng.DeleteBuffer("RIDDLE_RESOLVED");
	audMng.DeleteBuffer("RIDDLE_FAILED");
	audMng.DeleteSoundAndBuffer("CANDY_PICK");
	audMng.DeleteSoundAndBuffer("ITEM_PICK");
	audMng.DeleteSoundAndBuffer("ITEM_SPECIAL_PICK");
	audMng.DeleteSoundAndBuffer("CRATE_BREAK_SMALL");
	texMng.Delete("transparentSprite");
}

void ExplorationState::Init(GameData& _gameData, InGameDatas& _inGameDatas)
{
	drwMng.AddSection("Exploration");
	drwMng.AddSection("ExplorationUnSortables");

	TexturesManager::Get().Add("Contents/Assets/Sprites/Other/Transparent.png", "transparentSprite");

	InitSounds();
}

void ExplorationState::PollEvent(GameData& _gameData, InGameDatas& _inGameDatas)
{
	_inGameDatas.player->PollEvent(_gameData.gameEvent);
	_inGameDatas.enemyManager->PollEvent(_gameData.gameEvent);
	_inGameDatas.npcManager->Interact(_gameData.gameEvent, _inGameDatas.player);
	_inGameDatas.map->Event(_gameData.gameEvent, _inGameDatas);

	if (_inGameDatas.candleRiddle != nullptr)
	{
		_inGameDatas.candleRiddle->PollEvent(*_inGameDatas.player, _gameData.gameEvent);
	}
	if (_inGameDatas.savePoint != nullptr)
	{
		_inGameDatas.savePoint->Event(_gameData.gameEvent , *_inGameDatas.player);
	}
}

void ExplorationState::Update(GameData& _gameData, InGameDatas& _inGameDatas)
{
	_inGameDatas.propsManager->Update(*_inGameDatas.player);
	_inGameDatas.player->Update(_inGameDatas);
	_inGameDatas.hud->Update(*_inGameDatas.player);
	_inGameDatas.npcManager->Update(_inGameDatas.player);
	_inGameDatas.enemyManager->Update(_inGameDatas);
	_inGameDatas.camera->Update(*_inGameDatas.player);
	if (_inGameDatas.candleRiddle != nullptr)
	{
		_inGameDatas.candleRiddle->Update(*_inGameDatas.player, *_inGameDatas.camera);
	}
	if (_inGameDatas.savePoint != nullptr)
	{
		_inGameDatas.savePoint->Update(*_inGameDatas.player, *_inGameDatas.npcManager);
	}
	UpdateCrates(_inGameDatas);
	UpdateDroppedItems(_inGameDatas);

	_inGameDatas.map->Update(_inGameDatas);

	// Event qui lance le combat
	evtMng["GotoFighting"];
}

void ExplorationState::Display(InGameDatas& _inGameDatas , sf::RenderWindow& _window)
{
	_window.setView(_inGameDatas.camera->GetView());

	// Shader transparent
	DisplayShader(_inGameDatas, _window);

	drwMng.Draw(_window, "MAP_BACKGROUND");
	drwMng.Draw(_window, "Exploration", true);
	drwMng.Draw(_window, "ExplorationUnSortables");

	_window.setView(_window.getDefaultView());
	_inGameDatas.hud->Display(_window);
}

void ExplorationState::InitSounds(void)
{
	// Enemies
	audMng.AddBuffer("Contents/Assets/Audio/Sounds/Enemies/ZombiePatrol.wav", "ZOMBIE_PATROL");
	audMng.AddBuffer("Contents/Assets/Audio/Sounds/Enemies/ZombiePatrolFast.wav", "ZOMBIE_PATROL_FAST");
	audMng.AddBuffer("Contents/Assets/Audio/Sounds/Enemies/KnightWalk.wav", "KNIGHT_WALK");
	audMng.AddBuffer("Contents/Assets/Audio/Sounds/Enemies/CandleBossIdle.wav", "CANDLE_BOSS_IDLE");
	audMng.AddBuffer("Contents/Assets/Audio/Sounds/Enemies/FrankenchiefCut.wav", "FRANKENCHIEF_CUT");
	audMng.AddBuffer("Contents/Assets/Audio/Sounds/Enemies/BooWalk.wav", "BOO_WALK");
	audMng.AddBuffer("Contents/Assets/Audio/Sounds/Enemies/VampireWalk.wav", "VAMPIRE_WALK");

	// Riddles
	audMng.AddBuffer("Contents/Assets/Audio/Sounds/RiddleResolved.wav", "RIDDLE_RESOLVED");
	audMng.AddBuffer("Contents/Assets/Audio/Sounds/RiddleFailed.wav", "RIDDLE_FAILED");

	// Items
	audMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Items/CandyPick.wav", "CANDY_PICK");
	audMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Items/ItemPick.wav", "ITEM_PICK");
	audMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Items/ItemSpecialPick.wav", "ITEM_SPECIAL_PICK", 50.f);
	audMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Items/WoodCrateBreakSmall.wav", "CRATE_BREAK_SMALL", 60.f);

}

void ExplorationState::AddItemOnGround(InGameDatas& _inGameDatas, const sf::Vector2f& _position, const std::string& _itemId)
{
	_inGameDatas.itemsOnGround.push_back(std::make_unique<ItemOnGround>(_position, _itemId));
	drwMng.AddObject("Exploration", *_inGameDatas.itemsOnGround.back().get());
}

void ExplorationState::UpdateCrates(InGameDatas& _inGameDatas)
{
	for (unsigned short int i = 0; i < _inGameDatas.crateVect.size(); ++i)
	{
		_inGameDatas.crateVect[i]->Update(_inGameDatas);
		if (_inGameDatas.crateVect[i]->GetIsDestroy())
		{
			if (!_inGameDatas.crateVect[i]->GetItemName().empty() && !_inGameDatas.crateVect[i]->GetItemIsDrop())
			{
				AddItemOnGround(_inGameDatas, _inGameDatas.crateVect[i]->getPosition(), _inGameDatas.crateVect[i]->GetItemName());
				_inGameDatas.crateVect[i]->SetItemIsDrop(true);
			}

			if (_inGameDatas.crateVect[i]->GetIsDelete())
			{
				_inGameDatas.crateVect.erase(_inGameDatas.crateVect.begin() + i);
			}
		}
	}
}

void ExplorationState::UpdateDroppedItems(InGameDatas& _inGameDatas)
{
	for (size_t i = 0; i < _inGameDatas.itemsOnGround.size(); ++i)
	{
		sf::Vector2f targetPos(_inGameDatas.player->getPosition());
		sf::FloatRect itemRect = _inGameDatas.itemsOnGround[i]->GetRect();

		_inGameDatas.itemsOnGround[i]->Update();
		targetPos.y -= itemRect.height * 0.5f;

		if (_inGameDatas.itemsOnGround[i]->IsClose(targetPos))
		{
			const std::string& itemId = _inGameDatas.itemsOnGround[i]->GetItemId();
			if (itemId == "CANDY")
			{
				audMng.PlaySound("CANDY_PICK");
				_inGameDatas.player->AddSweets(1);
			}
			else
			{
				std::shared_ptr<Item> item = Item::prefabs.find(itemId)->second;

				if (item->GetType() == Item::IMPORTANT)
				{
					audMng.PlaySound("ITEM_SPECIAL_PICK");
				}
				else
				{
					audMng.PlaySound("ITEM_PICK");
				}

				_inGameDatas.player->AddItem(*item);
			}

			drwMng.DeleteObject("Exploration", _inGameDatas.itemsOnGround[i].get());
			_inGameDatas.itemsOnGround.erase(_inGameDatas.itemsOnGround.begin() + i);
		}
	}
}

void ExplorationState::DisplayShader(InGameDatas& _inGameDatas, sf::RenderWindow& _window)
{
	if (_inGameDatas.candleRiddle == nullptr)
	{
		_inGameDatas.propsManager->DisplayTransparentShader(_window, *_inGameDatas.player);
	}
}