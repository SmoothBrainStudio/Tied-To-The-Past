#include "InGame.hpp"
#include "SaveSystem.hpp"

void InitDefaultBools(void)
{
	auto& boolMng = BoolManager::Get();

	boolMng.Reset();
	boolMng["FIRST_CANDLE_SIMON"] = true;
	boolMng["FIRST_CANDLE_DAVID"] = true;
}

InGame::~InGame()
{
	EventManager& eventMng = EventManager::Get();
	DrawManager& drawMng = DrawManager::Get();
	TexturesManager& texturesMng = TexturesManager::Get();

	Item::UnloadPrefabs();
	Tie::UnloadPrefabs();
	Enemy::UnloadPrefabs();
	Map::DeloadPrefabs();

	//delete all ingameDatas(really necessary)
	inGameDatas.Clear();

	while (!subStates.empty())
	{
		//subStates.back().reset();
		subStates.pop_back();
	}

	exploState.reset();
	//fightState->~FightingState();
	fightState.reset();

	drawMng.DeleteSection("Exploration");
	drawMng.DeleteSection("ExplorationUnSortables");
	drawMng.DeleteSection("MAP_BACKGROUND");
	drawMng.DeleteSection("MAP_GRAPHICS");

	eventMng.Delete("GotoFighting");
	eventMng.Delete("GotoExplorationWin");
	eventMng.Delete("GotoExplorationLoose");
	eventMng.Delete("GotoSaveMenu");
	eventMng.Delete("GotoCharacterMenu");
	eventMng.Delete("GotoTieSelector");
	eventMng.Delete("GotoShop");

	texturesMng.Delete("ATTACK_BOX");
	texturesMng.Delete("RUN_BOX");
	texturesMng.Delete("ITEM_BOX");
	texturesMng.Delete("SELECT_BOX");
	texturesMng.Delete("ARROW_FIGHTING");
	texturesMng.Delete("HUD_FIGHTING");
	texturesMng.Delete("BOXER_BAR");
	texturesMng.Delete("DAMAGE");
	texturesMng.Delete("PERFECT_TEXT_FIGHTING");
	texturesMng.Delete("EXCELENT_TEXT_FIGHTING");
	texturesMng.Delete("GOOD_TEXT_FIGHTING");
	texturesMng.Delete("OK_TEXT_FIGHTING");
	texturesMng.Delete("MISS_TEXT_FIGHTING");
	texturesMng.Delete("BLOCKED_TEXT_FIGHTING");
	texturesMng.Delete("DODGED_TEXT_FIGHTING");
	texturesMng.Delete("HEARTH");
	texturesMng.Delete("BACKGROUND_FIGHTING");
	texturesMng.Delete("CRATE");
	texturesMng.Delete("BgPlayerEncounter");
	texturesMng.Delete("BgEnemyEncounter");
	texturesMng.Delete("ITEM_BOX_GREY");
	texturesMng.Delete("RUN_BOX_GREY");
	texturesMng.Delete("RUN_BOX_GREY");
	texturesMng.Delete("SELECT_BOX_GREY");
	texturesMng.Delete("WARNING");
	texturesMng.Delete("BAT_0");
	texturesMng.Delete("BAT_1");
	texturesMng.Delete("BAT_2");

	texturesMng.Delete("CandleOn");
	texturesMng.Delete("CandleOff");
	texturesMng.Delete("INVENTORY_ICON");
	texturesMng.Delete("CIRCULAR_MENU_ICON");
	texturesMng.Delete("CIRCULAR_WHEEL");

	texturesMng.Delete("SHOP_BG");

	DeleteFileTexture("Contents/Data/Textures/EnemyTextureAnimation.txt");
	DeleteFileTexture("Contents/Data/Textures/AlejandroTextureAnimation.txt");
	DeleteFileTexture("Contents/Data/Textures/OthersTextureAnimation.txt");
	DeleteFileTexture("Contents/Data/Textures/UIsTextureAnimation.txt");
	DeleteFileTexture("Contents/Data/Textures/PropsTextureAnimation.txt");

	delete fadeIn2sec;
	fadeIn2sec = nullptr;

	SaveSystem::Get().SetSaveName("");

	drawMng.DeleteSection("FIGHT_BG");
	drawMng.DeleteSection("FIGHT");
	drawMng.DeleteSection("FIGHT_BOXES");
	drawMng.DeleteSection("FIGHT_HUD");
}

void InGame::Init(GameData& _gameData)
{
	InitTexture();

	InitEvents(_gameData);

	currentPhaseStateID = PhaseStatesEnum::EXPLO; // default exploration state

	fightState = std::make_unique<FightingState>(_gameData, inGameDatas);
	exploState = std::make_unique<ExplorationState>(_gameData, inGameDatas);

	InitPrefabs();

	inGameDatas.player = std::make_unique<Player>();
	inGameDatas.enemyManager = std::make_unique<EnemyManager>();
	inGameDatas.npcManager = std::make_unique<NPC_Manager>();
	inGameDatas.hud = std::make_unique<HUD>();
	inGameDatas.camera = std::make_unique<Camera>(*inGameDatas.player);
	inGameDatas.propsManager = std::make_unique<PropsManager>();

	if (SaveSystem::Get().GetSaveName().empty())
	{
		// Create new game
		InitDefaultBools();
		GameClock::Get().SetPlayTime(0.f);
		inGameDatas.map = std::make_unique<Map>(DEFAULT_MAP, inGameDatas);
		InitSubStates(_gameData);
		fade = new Fade(2.f, 100.f, false, true);
	}
	else
	{
		// Load Save
		InitSubStates(_gameData);
		SaveSystem::Get().LoadGame(this);
		isExploMusicFading = true;
	}

	fadeIn2sec = new Fade(2.f, 100.f, false, true);
	Audio::Get().PlayMusic("EXPLORATION_MUSIC", true, 0.f);
}

void InGame::Update(GameData& _gameData)
{
	if (currentSubState == nullptr)
	{
		if (fade != nullptr)
		{
			fade->Update(GameClock::Get().GetDeltaTime(), "EXPLORATION_MUSIC");
		}

		UpdatePhaseState(_gameData, _gameData.window);
	}
	else
	{
		UpdateSubState(_gameData);
	}

	if (ControlsManager::Get().IsHeldInput("CIRCULAR_MENU"))
	{
		if (currentPhaseStateID == EXPLO && currentSubState == nullptr)
		{
			ChangeSubState(_gameData, SubStates::TIE_SELECTOR);
		}
	}
	else
	{
		if (currentSubState != nullptr && currentSubStateID == SubStates::TIE_SELECTOR && currentPhaseStateID == EXPLO)
		{
			currentSubState->isDelete = true;
		}
	}

	if (isExploMusicFading)
	{
		fadeIn2sec->UpdateMusic(GameClock::Get().GetDeltaTime(), "EXPLORATION_MUSIC");
		if (fadeIn2sec->GetTimer() >= fadeIn2sec->GetTimerMax())
		{
			isExploMusicFading = false;
		}
	}
}

void InGame::PollEvent(GameData& _gameData)
{
	SubStateEventHandler(_gameData);

	if (currentSubState.get() != nullptr)
	{

		inGameDatas.hud->PollEvent(_gameData);
		currentSubState->PollEvent(_gameData);
	}
	else
	{
		switch (currentPhaseStateID)
		{
		case EXPLO:
			exploState->PollEvent(_gameData, inGameDatas);
			inGameDatas.hud->PollEvent(_gameData);
			break;
		case FIGHT:
			fightState->PollEvent(_gameData, inGameDatas);
			break;
		default:
			Debug.error("WRONG PHASE STATE GIVEN IN INGAME");
			throw;
		}

		// DEBUG PURPOSE //
#if _DEBUG
		if (_gameData.gameEvent.type == sf::Event::KeyPressed)
		{
			//go to game win debug key
			if (_gameData.gameEvent.key.code == sf::Keyboard::Numpad0)
			{
				SceneManager::Get().LoadScene(new WinMenu);

				Audio::Get().StopAllMusics();
				_gameData.window.setView(_gameData.window.getDefaultView());
			}

			//go to game over debug key
			if (_gameData.gameEvent.key.code == sf::Keyboard::Numpad3)
			{
				SceneManager::Get().LoadScene(new GameOverMenu);

				Audio::Get().StopAllMusics();
				_gameData.window.setView(_gameData.window.getDefaultView());
			}

			// Accel game speed
			if (_gameData.gameEvent.key.code == sf::Keyboard::Add)
			{
				GameClock::Get().SetMultiplicator(GameClock::Get().GetMultiplicator() + 1);
			}

			if (_gameData.gameEvent.key.code == sf::Keyboard::Subtract)
			{
				if (GameClock::Get().GetMultiplicator() > 1)
				{
					GameClock::Get().SetMultiplicator(GameClock::Get().GetMultiplicator() - 1);
				}
			}

			if (_gameData.gameEvent.key.code == sf::Keyboard::Multiply)
			{
				GameClock::Get().SetMultiplicator(1.f);
			}
		}

		if (currentPhaseStateID == InGame::PhaseStatesEnum::EXPLO)
		{
			if (_gameData.gameEvent.type == sf::Event::KeyPressed)
			{
				if (_gameData.gameEvent.key.code == sf::Keyboard::Numpad2)
				{
					if (inGameDatas.player->GetSpeed() == Player::INITIAL_SPEED)
					{
						inGameDatas.player->GetCurAnim()->SetColor(sf::Color::Red);
						inGameDatas.player->SetSpeed(Player::DEBUG_SPEED);
					}
					else
					{
						inGameDatas.player->GetCurAnim()->SetColor(sf::Color::White);
						inGameDatas.player->SetSpeed(Player::INITIAL_SPEED);
					}
				}

				if (_gameData.gameEvent.key.code == sf::Keyboard::I)
				{
					EventManager::Get()["GotoSaveMenu"];
				}

				if (_gameData.gameEvent.key.code == sf::Keyboard::Numpad5)
				{
					inGameDatas.player->AddSweets(SHRT_MAX);
				}

				//give all ties
				if (_gameData.gameEvent.key.code == sf::Keyboard::M)
				{
					int i = 0;
					std::vector<std::shared_ptr<Tie>> ties = inGameDatas.player->GetTies();

					for (auto& tie : Tie::prefabs)
					{
						if (i >= ties.size() && i < Tie::prefabs.size() - 1) //don't give princess tie
						{
							inGameDatas.player->AddTie(tie.second);
						}
						i++;
					}
				}

				// TROP DE HP EN FAIT
				if (_gameData.gameEvent.key.code == sf::Keyboard::Numpad9)
				{
					inGameDatas.player->SetHPMax(SHRT_MAX);
					inGameDatas.player->SetHP(SHRT_MAX);
				}
			}
			//super speed debug key

		}
		if (currentPhaseStateID == InGame::PhaseStatesEnum::FIGHT)
		{
			if (_gameData.gameEvent.type == sf::Event::KeyPressed)
			{
				if (_gameData.gameEvent.key.code == sf::Keyboard::Numpad1)
				{
					if (inGameDatas.player->GetDamage() == Player::INITIAL_DAMAGE)
					{
						inGameDatas.player->GetCurAnim()->SetColor(sf::Color::Green);
						inGameDatas.player->SetDamage(Player::DEBUG_DAMAGE);
					}
					else
					{
						inGameDatas.player->GetCurAnim()->SetColor(sf::Color::White);
						inGameDatas.player->SetDamage(Player::INITIAL_DAMAGE);
					}
				}

				if (_gameData.gameEvent.key.code == sf::Keyboard::Num0)
				{
					inGameDatas.player->ApplyParticularEffect(inGameDatas.player, Entity::Effects::ANTI_BUFF);
				}
				if (_gameData.gameEvent.key.code == sf::Keyboard::Num1)
				{
					inGameDatas.player->ApplyParticularEffect(inGameDatas.player, Entity::Effects::ATK_BREAK);
				}
				if (_gameData.gameEvent.key.code == sf::Keyboard::I)
				{
					inGameDatas.player->ApplyParticularEffect(inGameDatas.player, Entity::Effects::ATK_BUFF);
				}
				if (_gameData.gameEvent.key.code == sf::Keyboard::Num3)
				{
					inGameDatas.player->ApplyParticularEffect(inGameDatas.player, Entity::Effects::BLEEDING);
				}
				if (_gameData.gameEvent.key.code == sf::Keyboard::Num4)
				{
					inGameDatas.player->ApplyParticularEffect(inGameDatas.player, Entity::Effects::BURNING);
				}
				if (_gameData.gameEvent.key.code == sf::Keyboard::Num5)
				{
					inGameDatas.player->ApplyParticularEffect(inGameDatas.player, Entity::Effects::DEF_BREAK);
				}
				if (_gameData.gameEvent.key.code == sf::Keyboard::Num6)
				{
					inGameDatas.player->ApplyParticularEffect(inGameDatas.player, Entity::Effects::DEF_BUFF);
				}
				if (_gameData.gameEvent.key.code == sf::Keyboard::Num7)
				{
					inGameDatas.player->ApplyParticularEffect(inGameDatas.player, Entity::Effects::IMMUNITY);
				}
				if (_gameData.gameEvent.key.code == sf::Keyboard::Num8)
				{
					inGameDatas.player->ApplyParticularEffect(inGameDatas.player, Entity::Effects::POISON);
				}
				if (_gameData.gameEvent.key.code == sf::Keyboard::Num9)
				{
					inGameDatas.player->ClearBadStatutes();
				}
				if (_gameData.gameEvent.key.code == sf::Keyboard::Subtract)
				{
					inGameDatas.player->ClearStatutes();
				}
			}
		}
#endif // !_DEBUG
	}
}

void InGame::Display(sf::RenderWindow& _window)
{
	_window.setMouseCursorVisible((currentSubState != nullptr && ControlsManager::Get().GetControllerType() == KEYBOARD) || (inGameDatas.tutoActive));

	switch (currentPhaseStateID)
	{
	case EXPLO:
		exploState->Display(inGameDatas, _window);
		break;
	case FIGHT:
		fightState->Display(inGameDatas, _window);
		break;
	default:
		Debug.error("WRONG PHASE STATE GIVEN IN INGAME");
		throw;
	}

	_window.setView(_window.getDefaultView());

	if (fade != nullptr)
	{
		_window.draw(*fade);
	}

	if (currentSubState != nullptr)
	{
		currentSubState->Display(_window);
	}
}

void InGame::InitTexture()
{
	TexturesManager& texturesMng = TexturesManager::Get();

	texturesMng.Add("Contents/Assets/Sprites/Fighting/Boxs/attackBox.png", "ATTACK_BOX");
	texturesMng.Add("Contents/Assets/Sprites/Fighting/Boxs/runBox.png", "RUN_BOX");
	texturesMng.Add("Contents/Assets/Sprites/Fighting/Boxs/itemBox.png", "ITEM_BOX");
	texturesMng.Add("Contents/Assets/Sprites/Fighting/Boxs/selectBox.png", "SELECT_BOX");
	texturesMng.Add("Contents/Assets/Sprites/Fighting/Other/Arrow.png", "ARROW_FIGHTING");
	texturesMng.Add("Contents/Assets/Sprites/Fighting/Other/Bottom_Box.png", "HUD_FIGHTING");
	texturesMng.Add("Contents/Assets/Sprites/Fighting/Other/boxer_bar.png", "BOXER_BAR");
	texturesMng.Add("Contents/Assets/Sprites/Fighting/Other/Damage.png", "DAMAGE");
	texturesMng.Add("Contents/Assets/Sprites/Fighting/Other/PERFECT.png", "PERFECT_TEXT_FIGHTING");
	texturesMng.Add("Contents/Assets/Sprites/Fighting/Other/EXCELLENT.png", "EXCELENT_TEXT_FIGHTING");
	texturesMng.Add("Contents/Assets/Sprites/Fighting/Other/GREAT.png", "GOOD_TEXT_FIGHTING");
	texturesMng.Add("Contents/Assets/Sprites/Fighting/Other/OKAY.png", "OK_TEXT_FIGHTING");
	texturesMng.Add("Contents/Assets/Sprites/Fighting/Other/MISS.png", "MISS_TEXT_FIGHTING");
	texturesMng.Add("Contents/Assets/Sprites/Fighting/Other/BLOCKED.png", "BLOCKED_TEXT_FIGHTING");
	texturesMng.Add("Contents/Assets/Sprites/Fighting/Other/Dodged.png", "DODGED_TEXT_FIGHTING");
	texturesMng.Add("Contents/Assets/Sprites/Fighting/Other/Hearth.png", "HEARTH");
	texturesMng.Add("Contents/Assets/Sprites/Backgrounds/background.png", "BACKGROUND_FIGHTING");
	texturesMng.Add("Contents/Assets/Sprites/Entities/Interactables/Crate.png", "CRATE");
	texturesMng.Add("Contents/Assets/Sprites/UI/HUD/InventoryIcon.png", "INVENTORY_ICON");
	texturesMng.Add("Contents/Assets/Sprites/UI/HUD/CircularMenuIcon.png", "CIRCULAR_MENU_ICON");
	texturesMng.Add("Contents/Assets/Sprites/Fighting/Other/BackgroundPlayerEncounter.png", "BgPlayerEncounter");
	texturesMng.Add("Contents/Assets/Sprites/Fighting/Other/BackgroundEnemyEncounter.png", "BgEnemyEncounter");
	texturesMng.Add("Contents/Assets/Sprites/Riddles/CandleOn.png", "CandleOn");
	texturesMng.Add("Contents/Assets/Sprites/Riddles/CandleOff.png", "CandleOff");
	texturesMng.Add("Contents/Assets/Sprites/UI/HUD/CircularWheelBG.png", "CIRCULAR_WHEEL");
	texturesMng.Add("Contents/Assets/Sprites/UI/Backgrounds/Shop.png", "SHOP_BG");
	texturesMng.Add("Contents/Assets/Sprites/Fighting/Boxs/itemBoxGrey.png", "ITEM_BOX_GREY");
	texturesMng.Add("Contents/Assets/Sprites/Fighting/Boxs/runBoxGrey.png", "RUN_BOX_GREY");
	texturesMng.Add("Contents/Assets/Sprites/Fighting/Boxs/selectBoxGrey.png", "SELECT_BOX_GREY");
	texturesMng.Add("Contents/Assets/Sprites/Fighting/Other/Warning.png", "WARNING");

	texturesMng.Add("Contents/Assets/Sprites/Entities/Enemies/Vampire/Bats/1.png", "BAT_0");
	texturesMng.Add("Contents/Assets/Sprites/Entities/Enemies/Vampire/Bats/2.png", "BAT_1");
	texturesMng.Add("Contents/Assets/Sprites/Entities/Enemies/Vampire/Bats/3.png", "BAT_2");

	InitFileTexture("Contents/Data/Textures/EnemyTextureAnimation.txt");
	InitFileTexture("Contents/Data/Textures/AlejandroTextureAnimation.txt");
	InitFileTexture("Contents/Data/Textures/OthersTextureAnimation.txt");
	InitFileTexture("Contents/Data/Textures/UIsTextureAnimation.txt");
	InitFileTexture("Contents/Data/Textures/PropsTextureAnimation.txt");
}

void InGame::InitEvents(GameData& _gameData)
{
	EventManager& eventMng = EventManager::Get();

	auto gotoFight = [&](void) -> void
	{
		_gameData.window.setView(_gameData.window.getDefaultView());
		fightState->Start(inGameDatas);
		currentPhaseStateID = FIGHT;
	};

	auto testGotoFight = [&](void) -> bool
	{
		std::shared_ptr<Enemy> collidedEntity = nullptr;
		bool enemyCollide = inGameDatas.enemyManager->ResolveEnemyPlayerCollision(inGameDatas.player, collidedEntity);

		if (enemyCollide && currentPhaseStateID == EXPLO)
		{
			if (!collidedEntity->CanAttack())
			{
				return false;
			}

			collidedEntity->ResetTimeBeforeAttack();
			collidedEntity->SetReadyToFight();

			if (inGameDatas.player->GetCostumeState() == CostumeState::BASIC)
			{
				inGameDatas.player->SetCostumeState(CostumeState::BOXER);
			}

			inGameDatas.player->SetApplicableStatus();
			lastEnemyColliding = collidedEntity.get();

			audioMng.PauseMusic("EXPLORATION_MUSIC");

			if (collidedEntity->GetName() == "FRANKENCHIEF")
			{
				for (int i = 0; i < 2; i++)
				{
					Enemy enemyTemp = inGameDatas.enemyManager->CopyPrefabEnemyFighting("ZOMBIE");
					std::shared_ptr<Enemy> newAlly = std::make_shared<Enemy>(enemyTemp);
					collidedEntity->AddAlly(newAlly);
				}
				audioMng.PlayMusic("BOSS_MUSIC", true, 70.f);
			}
			else if (collidedEntity->GetName() == "WAX GOLEM" || collidedEntity->GetName() == "VAMPIRE")
			{
				audioMng.PlayMusic("BOSS_MUSIC", true, 70.f);
			}
			else if (collidedEntity->GetName() != "BIG BOO")
			{
				int i = 0;
				int random = rand() % 3;

				while (i != random)
				{
					std::shared_ptr<Enemy> newAlly = std::make_shared<Enemy>(inGameDatas.enemyManager->CopyPrefabEnemyFighting(inGameDatas.map->GetEnemiesName()[rand() % inGameDatas.map->GetEnemiesName().size()]));
					collidedEntity->AddAlly(newAlly);
					i++;
				}

				audioMng.PlayMusic("FIGHT_MUSIC", true);
			}
			else
			{
				audioMng.PlayMusic("FINAL_BOSS_MUSIC", true, 80.f);
			}

			collidedEntity->SetIsInFight(true);
			for (auto& elem : collidedEntity->GetAllies())
			{
				elem->SetIsInFight(true);
			}

			inGameDatas.player->SetIsInFight(true);

			return true;
		}

		return false;
	};

	auto gotoExploWin = [&](void) -> void
	{
		ReloadExploMusic();

		lastEnemyColliding->SetAlive(false);
		lastEnemyColliding->SetIsInFight(false);
		inGameDatas.player->SetIsInFight(false);
		lastEnemyColliding->ClearAllies();

		if (lastEnemyColliding->GetName() == "BIG BOO")
		{
			SceneManager::Get().LoadScene(new WinMenu);
			return;
		}

		inGameDatas.player->setPosition(inGameDatas.player->GetLastPosExplo());
		inGameDatas.enemyManager->EraseDeadEnemies();

		ChangePhaseState(_gameData);
	};

	auto gotoExploLoose = [&](void) -> void
	{
		if (inGameDatas.player->GetHP() <= 0)
		{
			SceneManager::Get().LoadScene(new GameOverMenu);
			audioMng.StopAllMusics();
			_gameData.window.setView(_gameData.window.getDefaultView());
		}
		else
		{
			ReloadExploMusic();

			ChangePhaseState(_gameData);

			if (lastEnemyColliding->IsAlive())
			{
				lastEnemyColliding->SetStateExploration(StatesExploration::PATROL);
				lastEnemyColliding->SetIsInFight(false);
				lastEnemyColliding->SetHP(lastEnemyColliding->GetHPMax());
				lastEnemyColliding->setPosition(lastEnemyColliding->GetLastPosExplo());
				lastEnemyColliding->ClearAllies();
				lastEnemyColliding->SetAnimationState(Enemy::AnimationState::IDLE);
			}
			else
			{
				//if you only kill the enemy that you had collided with, he is dispawning from map, avoiding crashes but feels weird?
				inGameDatas.enemyManager->EraseDeadEnemies();
			}

			inGameDatas.player->SetIsInFight(false);
			inGameDatas.player->setPosition(inGameDatas.player->GetLastPosExplo());
		}
	};

	eventMng.Add("GotoFighting", gotoFight, testGotoFight);
	eventMng.Add("GotoExplorationWin", gotoExploWin);
	eventMng.Add("GotoExplorationLoose", gotoExploLoose);
	eventMng.Add("GotoSaveMenu", std::bind(&InGame::SwitchSubState, this, std::ref(_gameData), SubStates::SAVE_MENU));
	eventMng.Add("GotoCharacterMenu", std::bind(&InGame::SwitchSubState, this, std::ref(_gameData), SubStates::CHARACTER_MENU));
	eventMng.Add("GotoTieSelector", std::bind(&InGame::SwitchSubState, this, std::ref(_gameData), SubStates::TIE_SELECTOR));
	eventMng.Add("GotoShop", std::bind(&InGame::SwitchSubState, this, std::ref(_gameData), SubStates::SHOP));
}

void InGame::InitPrefabs(void)
{
	Item::LoadPrefabs();
	Map::LoadPrefabs();
	Tie::LoadPrefabs();
	Enemy::LoadPrefabs(GetAllFilesInFolder("Contents/Data/Enemies/"));
}

void InGame::InitSubStates(GameData& _gameData)
{
	subStates.push_back(std::make_shared<SettingsMenu>());
	subStates.push_back(std::make_shared<Pause>());
	subStates.push_back(std::make_shared<CharacterMenu>());
	subStates.push_back(std::make_shared<Shop>());

	std::vector<std::string> tieTextureKeys;
	for (auto& elem : inGameDatas.player->GetTies())
	{
		tieTextureKeys.push_back(elem->GetName());
	}
	subStates.push_back(std::make_shared<CircularMenu>(TexturesManager::Get()["CIRCULAR_PIECE_0"], 8, tieTextureKeys));

	subStates.push_back(std::make_shared<SaveMenu>());
	((SaveMenu*)subStates.back().get())->SetInGame(this);

	((Shop*)subStates[SubStates::SHOP].get())->Load("Contents/Data/Shops/shop1.json", inGameDatas.player, *inGameDatas.hud);
	((CharacterMenu*)subStates[SubStates::CHARACTER_MENU].get())->SetPlayer(inGameDatas.player);

	for (auto& it : subStates)
	{
		it->Init(_gameData);
	}
}

void InGame::UpdateSubState(GameData& _gameData)
{
	currentSubState->Update(_gameData);

	if (currentSubState->isDelete)
	{
		currentSubState->isDelete = false;
		ChangeSubState(_gameData, currentSubState->nextSubState);
	}
}

void InGame::SubStateEventHandler(GameData& _gameData)
{

	if (currentSubState == nullptr && ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "PAUSE") && !inGameDatas.tutoActive)
	{
		ChangeSubState(_gameData, SubStates::PAUSE);
	}
	else if (currentSubState != nullptr && (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "PAUSE") || ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "CANCEL") || ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "CANCEL_2")))
	{
		if (currentSubStateID != SubStates::SETTINGS)
		{
			currentSubState->isDelete = true;
		}
		else
		{
			//obligé de faire un else puis un if sinon ça va cast le pointeur dans une classe qu'il n'est pas forcément du coup potentiels crash
			if (((SettingsMenu*)currentSubState.get())->GetSubMenuState() == SettingsMenu::SubMenuState::NONE)
			{
				currentSubState->isDelete = true;
			}
		}
	}


	if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "INVENTORY") && currentPhaseStateID == EXPLO)
	{
		SwitchSubState(_gameData, SubStates::CHARACTER_MENU);
	}

#ifndef _DEBUG
	if (_gameData.gameEvent.type == sf::Event::LostFocus)
	{
		ChangeSubState(_gameData, SubStates::PAUSE);
	}
#endif // !DEBUG
}

void InGame::ChangeSubState(GameData& _gameData, const SubStates& _subState)
{
	if (currentSubStateID == SubStates::CHARACTER_MENU)
	{
		((CharacterMenu*)currentSubState.get())->DisableShader();
		Audio::Get().PlaySound("CLOSE_SELLING");
	}

	switch (_subState)
	{
	case SubStates::PAUSE:
		currentSubState.reset();
		currentSubState = subStates[SubStates::PAUSE];
		currentSubStateID = SubStates::PAUSE;
		break;
	case SubStates::SETTINGS:
		currentSubState.reset();
		currentSubState = subStates[SubStates::SETTINGS];
		currentSubStateID = SubStates::SETTINGS;
		break;
	case SubStates::CHARACTER_MENU:
		if (inGameDatas.player->GetState() != PlayerState::SPEAK)
		{
			currentSubState.reset();
			currentSubState = subStates[SubStates::CHARACTER_MENU];
			currentSubStateID = SubStates::CHARACTER_MENU;
			((CharacterMenu*)currentSubState.get())->InitOnOpen(_gameData, inGameDatas.player);
		}
		break;
	case SubStates::SHOP:
		currentSubState.reset();
		currentSubState = subStates[SubStates::SHOP];
		currentSubStateID = SubStates::SHOP;
		break;
	case SubStates::TIE_SELECTOR:
		if (inGameDatas.player->GetState() != PlayerState::SPEAK)
		{
			_gameData.window.setMouseCursor(_gameData.cursor);
			currentSubState.reset();
			currentSubState = subStates[SubStates::TIE_SELECTOR];
			currentSubStateID = SubStates::TIE_SELECTOR;
			((CircularMenu*)currentSubState.get())->GetTieSelector()->SetIsVisible(true);
			((CircularMenu*)currentSubState.get())->GetTieSelector()->SetTieIndex(-1);
			((CircularMenu*)currentSubState.get())->GetTieSelector()->ChangeTieTextures(inGameDatas.player);
		}
		break;
	case SubStates::SAVE_MENU:
		currentSubState.reset();
		currentSubState = subStates[SubStates::SAVE_MENU];
		((SaveMenu*)currentSubState.get())->UpdateSaves(_gameData);
		currentSubStateID = SubStates::SAVE_MENU;
		break;
	case SubStates::NULL_STATE:
		if (currentSubStateID == SubStates::TIE_SELECTOR)
		{
			_gameData.window.setMouseCursor(_gameData.cursorGrey);
			if ((CostumeState)((CircularMenu*)currentSubState.get())->GetTieSelector()->GetTieIndex() != -1)
			{
				inGameDatas.player->SetCostumeState((CostumeState)((CircularMenu*)currentSubState.get())->GetTieSelector()->GetTieIndex());
			}
			inGameDatas.player->SetState(PlayerState::IDLE);
			inGameDatas.player->UpdateAnimations();
		}
		currentSubState.reset();
		currentSubState = nullptr;
		currentSubStateID = SubStates::NULL_STATE;
		break;
	default:
		break;
	}

	subStates[SubStates::PAUSE]->nextSubState = SubStates::NULL_STATE;
}

void InGame::ChangePhaseState(GameData& _gameData)
{
	switch (currentPhaseStateID)
	{
	case PhaseStatesEnum::EXPLO:
		currentPhaseStateID = PhaseStatesEnum::FIGHT;
		break;
	case PhaseStatesEnum::FIGHT:
		_gameData.window.setView(inGameDatas.camera->GetView());
		currentPhaseStateID = PhaseStatesEnum::EXPLO;
		break;
	default:
		break;
	}
}

void InGame::UpdatePhaseState(GameData& _gameData, sf::RenderWindow& _window)
{
	GameClock::Get().UpdatePlayTime();
	switch (currentPhaseStateID)
	{
	case EXPLO:
		exploState->Update(_gameData, inGameDatas);

		if (exploState->isDelete)
		{
			exploState->isDelete = false;
			ChangePhaseState(_gameData);
		}

		break;
	case FIGHT:
		fightState->Update(_gameData, inGameDatas);

		if (fightState->isDelete)
		{
			fightState->isDelete = false;
			ChangePhaseState(_gameData);
		}

		break;
	default:
		Debug.error("WRONG PHASE STATE GIVEN : ", currentPhaseStateID);
		throw;
		break;
	}
}

void InGame::SwitchSubState(GameData& _gameData, SubStates _subState)
{
	if (currentSubState == nullptr)
	{
		ChangeSubState(_gameData, _subState);
	}
	else if (currentSubState != nullptr && currentSubStateID == _subState)
	{
		currentSubState->isDelete = true;
	}
}

void InGame::ReloadExploMusic(void)
{
	audioMng.StopAllMusics();
	audioMng.StopAllSounds();
	isExploMusicFading = true;
	fadeIn2sec->SetIsFadingOut(true);
	audioMng.SetMusicVolume("EXPLORATION_MUSIC", 0.f);
	audioMng.UnPauseMusic("EXPLORATION_MUSIC");
}
