#include "Definitions.hpp"
#include "FightingState.hpp"
#include "Status.hpp"

FightingState::FightingState(GameData& _gameData, InGameDatas& _inGameDatas)
{
	auto& drawMng = DrawManager::Get();

	drawMng.AddSection("FIGHT_BG");
	drawMng.AddSection("FIGHT");
	drawMng.AddSection("FIGHT_BOXES");
	drawMng.AddSection("FIGHT_HUD");

	InitTextures();
	InitSounds();

	InitDisplayedKeys();
	keyLeft.setOrigin(keyLeft.getGlobalBounds().width / 2.f, keyLeft.getGlobalBounds().height / 2.f);
	keyRight.setOrigin(keyRight.getGlobalBounds().width / 2.f, keyRight.getGlobalBounds().height / 2.f);
	drawMng.AddObject("FIGHT_HUD", keyLeft);
	drawMng.AddObject("FIGHT_HUD", keyRight);

	winScreen.setTexture(TexturesManager::Get()["WIN_SCREEN_FIGHTING"]);
	winScreen.setOrigin(winScreen.getGlobalBounds().width / 2.f, winScreen.getGlobalBounds().height / 2.f);
	winScreen.setPosition(WORKING_WIDTH / 2.f, WORKING_HEIGHT / 2.f);
	drawMng.AddObject("FIGHT_HUD", winScreen);

	defeatScreen.setTexture(TexturesManager::Get()["LOOSE_SCREEN_FIGHTING"]);
	defeatScreen.setOrigin(defeatScreen.getGlobalBounds().width / 2.f, defeatScreen.getGlobalBounds().height / 2.f);
	defeatScreen.setPosition(WORKING_WIDTH / 2.f, WORKING_HEIGHT / 2.f);
	drawMng.AddObject("FIGHT_HUD", defeatScreen);

	enemyName.setFont(FontsManager::Get()["SPOOKY"]);
	selectArrow.setRotation(270.f);
	SetOutlined(enemyName, 2);
	drawMng.AddObject("FIGHT_HUD", enemyName);

	Init(_gameData, _inGameDatas);

	std::vector<std::string> tutoTextureKeys;
	tutoTextureKeys.push_back("TUTO_FIGHT_GENERAL_1");
	tutoTextureKeys.push_back("TUTO_FIGHT_GENERAL_2");
	tutoTextureKeys.push_back("TUTO_FIGHT_GENERAL_3");
	tutoTextureKeys.push_back("TUTO_FIGHT_GENERAL_4");
	tutoTextureKeys.push_back("TUTO_FIGHT_GENERAL_5");

	std::vector<std::string> descriptionPaths;
	descriptionPaths.push_back("TutoFight1.txt");
	descriptionPaths.push_back("TutoFight2.txt");
	descriptionPaths.push_back("TutoFight3.txt");
	descriptionPaths.push_back("TutoFight4.txt");
	descriptionPaths.push_back("TutoFight5.txt");

	tutoMenu.reset(new TutoMenu(_gameData, tutoTextureKeys, descriptionPaths, "FIGHT"));

	infoButton.setTexture(TexturesManager::Get()["INFO_BTN"]);
	infoButton.setOrigin(infoButton.getGlobalBounds().width / 2.f, infoButton.getGlobalBounds().height / 2.f);
	infoButton.setPosition(infoButton.getGlobalBounds().width / 2.f + 25.f, infoButton.getGlobalBounds().height / 2.f + 25.f);

	infoKey.setTexture(ControlsManager::Get().GetTexture("FIGHT_INFO"));
	infoKey.setOrigin(infoKey.getGlobalBounds().width / 2.f, infoKey.getGlobalBounds().height / 2.f);
	infoKey.setScale(0.75f, 0.75f);
	infoKey.setPosition(infoButton.getPosition().x + infoButton.getGlobalBounds().width / 2.f + 35.f, infoButton.getPosition().y);

	DrawManager::Get().AddObject("FIGHT_HUD", infoButton);
	DrawManager::Get().AddObject("FIGHT_HUD", infoKey);

	fade.reset(new Fade(2.f, 100.f, true, false));
	drawMng.AddObject("FIGHT_HUD", *fade);
}

FightingState::~FightingState()
{
	auto& drawMng = DrawManager::Get();
	auto& textureMng = TexturesManager::Get();

	while (!enemies.empty())
	{
		enemies.back().reset();
		enemies.pop_back();
	}

	enemies.clear();

	DeleteSounds();
	DeleteTextures();
}

void FightingState::Start(InGameDatas& _inGameDatas)
{
	//run mini-game reset
	earn.sweets = 0;
	phase = ENCOUNTER;

	playerRunning = false;
	clickCount = 0;
	lastKeyPressed = "";
	runFinished = false;
	timerRun = 0.f;
	timerResetScale = TIMER_SCALE_MAX;
	keyLeft.setScale(1.f, 1.f);
	keyRight.setScale(1.f, 1.f);
	selectEnemy = 0;

	fade->SetIsFadingIn(false);

	// Add enemies
	for (auto& elem : _inGameDatas.enemyManager->GetEnemiesVector())
	{
		if (elem->GetIsInFight())
		{
			elem->SetLastPosExplo(elem->getPosition());

			AddEnemy(elem);

			for (auto& ally : elem->GetAllies())
			{
				AddEnemy(ally);
				if (ally->GetName() == "KNIGHT")
				{
					ally->SetAnimationState(Enemy::AnimationState::SPECIAL);
				}
			}

			break;
		}
	}

	canRun = true;
	for (auto& enemy : enemies)
	{
		if (enemy->GetName() == "FRANKENCHIEF" || enemy->GetName() == "WAX GOLEM" || enemy->GetName() == "BIG BOO" || enemy->GetName() == "VAMPIRE")
		{
			canRun = false;
		}
	}

	encounter.Start(*_inGameDatas.player, enemies);

	// Stop sounds and play introduction FX
	audMng.StopAllSounds();
	audMng.PlaySound("FIGHT_INTRODUCTION", 50.f);

	if (_inGameDatas.player->GetCostumeState() == CostumeState::CANDLE)
	{
		audMng.ChangeBuffer("LEAK", "CANDLE_WALK");
	}
	else
	{
		audMng.ChangeBuffer("LEAK", "IS_LEAKING");
	}

	DrawManager::Get().AddObject("FIGHT", *_inGameDatas.player);
}

void FightingState::End(InGameDatas& _inGameDatas)
{
	fade->SetIsFadingIn(false);

	for (unsigned int i = 0; i < earn.resultItem.size(); i++)
	{
		drwMng.DeleteObject("FIGHT_HUD", &earn.resultItem[i]->item);
		drwMng.DeleteObject("FIGHT_HUD", &earn.resultItem[i]->nbItem);
		earn.resultItem[i].reset();
	}
	earn.resultItem.clear();

	for (unsigned int i = 0; i < plusOrMinus.size(); i++)
	{
		drwMng.DeleteObject("FIGHT_HUD", plusOrMinus[i].get());
	}
	plusOrMinus.clear();
	_inGameDatas.player->GetCurAnim()->SetSpeed(1.f);

	drwMng.DeleteObject("FIGHT", _inGameDatas.player.get());

	bool hasPlayerWon = HasPlayerWon();
	for (auto& enemy : enemies)
	{
		if (enemy->GetHP() <= 0)
		{
			enemy->SaveDeath();
		}

		drwMng.DeleteObject("FIGHT", enemy.get());
		enemy->ClearStatutes();

		if (hasPlayerWon)
		{
			enemy.reset();
		}
	}
	enemies.clear();

	earn.loots.clear();
	_inGameDatas.player->ClearStatutes();

	if (hasPlayerWon)
	{
		evtMng["GotoExplorationWin"];
	}
	else
	{

		evtMng["GotoExplorationLoose"];
	}
}

void FightingState::EndTurn(InGameDatas& _inGameDatas)
{
	for (auto& enemy : enemies)
	{
		if (enemy->GetHP() > 0)
		{
			enemy->RunStatutes();
		}
	}

	_inGameDatas.player->RunStatutes();

	if (HasPlayerWon())
	{
		EndCombat(_inGameDatas);
	}
	else
	{
		phase = ACTION_SELECT;
	}

	fightBoxes.SetSelectedBox(0);
}

void FightingState::EndCombat(InGameDatas& _inGameDatas)
{
	Earnings(*_inGameDatas.player);

	phase = RESULT_SCREEN;
}

#pragma region INIT
void FightingState::Init(GameData& _gameData, InGameDatas& _inGameDatas)
{
	InitUI();
	hud.InitBackground();
	hud.InitLifeHUD();
	encounter.Init();
	fightBoxes.Init();
	audMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Enemies/FightIntroduction.wav", "FIGHT_INTRODUCTION");
}

void FightingState::InitUI(void)
{
	selectArrow.setTexture(TexturesManager::Get()["ARROW_FIGHTING"], true);
	selectArrow.setOrigin(32.f, 32.f);

	DrawManager::Get().AddObject("FIGHT_HUD", selectArrow);
}

void FightingState::InitDisplayedKeys(void)
{
	if (ControlsManager::Get().GetControllerType() == KEYBOARD)
	{
		leftKeyID = "LEFT";
		rightKeyID = "RIGHT";
	}
	else
	{
		leftKeyID = "ESCAPE_1";
		rightKeyID = "ESCAPE_2";
	}

	keyLeft.setTexture(ControlsManager::Get().GetTexture(leftKeyID));
	keyRight.setTexture(ControlsManager::Get().GetTexture(rightKeyID));
}

void FightingState::InitTextures(void)
{
	auto& textureMng = TexturesManager::Get();

	textureMng.Add("Contents/Assets/Sprites/UI/HUD/Fight/Victory_Board.png", "WIN_SCREEN_FIGHTING");
	textureMng.Add("Contents/Assets/Sprites/UI/HUD/Fight/Defeat_Board.png", "LOOSE_SCREEN_FIGHTING");

	textureMng.Add("Contents/Assets/Sprites/UI/HUD/Fight/Victory_Plus.png", "VICTORY_PLUS");
	textureMng.Add("Contents/Assets/Sprites/UI/HUD/Fight/Defeat_Minus.png", "DEFEAT_MINUS");

	textureMng.Add("Contents/Assets/Sprites/Fighting/Effects/Bleeding.png", "BLEEDING");
	textureMng.Add("Contents/Assets/Sprites/Fighting/Effects/Burning.png", "BURNING");
	textureMng.Add("Contents/Assets/Sprites/Fighting/Effects/AtkBuff.png", "ATK_BUFF");
	textureMng.Add("Contents/Assets/Sprites/Fighting/Effects/BreakAtk.png", "ATK_BREAK");
	textureMng.Add("Contents/Assets/Sprites/Fighting/Effects/BuffDef.png", "DEF_BUFF");
	textureMng.Add("Contents/Assets/Sprites/Fighting/Effects/BreakDef.png", "DEF_BREAK");
	textureMng.Add("Contents/Assets/Sprites/Fighting/Effects/Poison.png", "POISON");
	textureMng.Add("Contents/Assets/Sprites/Fighting/Effects/Immunity.png", "IMMUNITY");
	textureMng.Add("Contents/Assets/Sprites/Fighting/Effects/AntiBuff.png", "ANTI_BUFF");
	textureMng.Add("Contents/Assets/Sprites/Fighting/Effects/Stun.png", "STUN");

	texMng.Add("Contents/Assets/Sprites/UI/Tuto/TutoGeneral1.png", "TUTO_FIGHT_GENERAL_1");
	texMng.Add("Contents/Assets/Sprites/UI/Tuto/TutoGeneral2.png", "TUTO_FIGHT_GENERAL_2");
	texMng.Add("Contents/Assets/Sprites/UI/Tuto/TutoGeneral3.png", "TUTO_FIGHT_GENERAL_3");
	texMng.Add("Contents/Assets/Sprites/UI/Tuto/TutoGeneral4.png", "TUTO_FIGHT_GENERAL_4");
	texMng.Add("Contents/Assets/Sprites/UI/Tuto/TutoGeneral5.png", "TUTO_FIGHT_GENERAL_5");

	texMng.Add("Contents/Assets/Sprites/UI/Tuto/TutoBoxer.png", "TUTO_FIGHT_BOXER");
	texMng.Add("Contents/Assets/Sprites/UI/Tuto/TutoCandle.png", "TUTO_FIGHT_CANDLE");
	texMng.Add("Contents/Assets/Sprites/UI/Tuto/TutoMusician.png", "TUTO_FIGHT_MUSIC");

}
void FightingState::DeleteTextures(void)
{
	auto& textureMng = TexturesManager::Get();

	textureMng.Delete("WIN_SCREEN_FIGHTING");
	textureMng.Delete("LOOSE_SCREEN_FIGHTING");

	textureMng.Delete("VICTORY_PLUS");
	textureMng.Delete("DEFEAT_MINUS");

	textureMng.Delete("BLEEDING");
	textureMng.Delete("BURNING");
	textureMng.Delete("ATK_BUFF");
	textureMng.Delete("ATK_BREAK");
	textureMng.Delete("DEF_BUFF");
	textureMng.Delete("DEF_BREAK");
	textureMng.Delete("POISON");
	textureMng.Delete("IMMUNITY");
	textureMng.Delete("ANTI_BUFF");
	textureMng.Delete("STUN");

	textureMng.Delete("TUTO_FIGHT_GENERAL_1");
	textureMng.Delete("TUTO_FIGHT_GENERAL_2");
	textureMng.Delete("TUTO_FIGHT_GENERAL_3");
	textureMng.Delete("TUTO_FIGHT_GENERAL_4");
	textureMng.Delete("TUTO_FIGHT_GENERAL_5");

	texMng.Delete("TUTO_FIGHT_BOXER");
	texMng.Delete("TUTO_FIGHT_CANDLE");
	texMng.Delete("TUTO_FIGHT_MUSIC");
}

void FightingState::InitSounds(void)
{
	auto& audioMng = Audio::Get();

	// Hits
	audioMng.AddBuffer("Contents/Assets/Audio/Sounds/Enemies/KnightHit.wav", "KNIGHT_HIT");
	audioMng.AddBuffer("Contents/Assets/Audio/Sounds/Enemies/ZombieHit.wav", "ZOMBIE_HIT");
	audioMng.AddSoundWithBuffer("KNIGHT_HIT", "ENEMY_HIT");
	audioMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Enemies/AilmentHit.wav", "AILMENT_HIT", 70.f);

	// Dodge and Counter attacks
	audioMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Enemies/Dodge.wav", "DODGE", 75.f);
	audioMng.AddBuffer("Contents/Assets/Audio/Sounds/Player/BoxerCounter.wav", "BOXER_COUNTER");
	audioMng.AddBuffer("Contents/Assets/Audio/Sounds/Player/CandleCounter.wav", "CANDLE_COUNTER");
	audioMng.AddBuffer("Contents/Assets/Audio/Sounds/Player/MusicianCounter.wav", "MUSICIAN_COUNTER");
	audioMng.AddSoundWithBuffer("BOXER_COUNTER", "COUNTER_ATTACK");

	// Deads
	audioMng.AddBuffer("Contents/Assets/Audio/Sounds/Player/BoxerDeath.wav", "BOXER_DEATH");
	audioMng.AddBuffer("Contents/Assets/Audio/Sounds/Player/CandleDeath.wav", "CANDLE_DEATH");
	audioMng.AddSoundWithBuffer("BOXER_DEATH", "PLAYER_DEATH");
	audioMng.AddBuffer("Contents/Assets/Audio/Sounds/Enemies/KnightDeath.wav", "KNIGHT_DEATH");
	audioMng.AddBuffer("Contents/Assets/Audio/Sounds/Enemies/ZombieDeath.wav", "ZOMBIE_DEATH");
	audioMng.AddBuffer("Contents/Assets/Audio/Sounds/Enemies/FireballDeath.wav", "FIREBALL_DEATH");
	audioMng.AddBuffer("Contents/Assets/Audio/Sounds/Enemies/FrankenchiefDeath.wav", "FRANKENCHIEF_DEATH");
	audioMng.AddBuffer("Contents/Assets/Audio/Sounds/Enemies/CandleBossDeath.wav", "CANDLEBOSS_DEATH");
	audioMng.AddBuffer("Contents/Assets/Audio/Sounds/Enemies/BooDeath.wav", "BOO_DEATH");
	audioMng.AddBuffer("Contents/Assets/Audio/Sounds/Enemies/VampireDeath.wav", "VAMPIRE_DEATH");
	audioMng.AddSoundWithBuffer("KNIGHT_DEATH", "ENEMY_DEATH");

	// Candles
	audioMng.AddSoundWithBuffer("FIREBALL_LAUNCH", "FIREBALL_LAUNCH");
	audioMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Enemies/FireballNoise.wav", "FIRE_NOISE", 40.f);
	audioMng.AddBuffer("Contents/Assets/Audio/Sounds/Enemies/FireballMiss.wav", "FIRE_MISS");
	audioMng.AddBuffer("Contents/Assets/Audio/Sounds/Enemies/FireballOk.wav", "FIRE_OK");
	audioMng.AddBuffer("Contents/Assets/Audio/Sounds/Enemies/FireballGreat.wav", "FIRE_GREAT");
	audioMng.AddBuffer("Contents/Assets/Audio/Sounds/Enemies/FireballExcelent.wav", "FIRE_EXCELENT");
	audioMng.AddBuffer("Contents/Assets/Audio/Sounds/Enemies/FireballPerfect.wav", "FIRE_PERFECT");
	audioMng.AddSoundWithBuffer("FIRE_OK", "FIRE_IMPACT");

	// Musician
	audioMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Player/MusicianAttack.wav", "MUSICIAN_ATTACK", 80.f);
}
void FightingState::DeleteSounds(void)
{
	auto& audioMng = Audio::Get();

	audioMng.DeleteSoundAndBuffer("FIGHT_INTRODUCTION");
	audioMng.DeleteBuffer("KNIGHT_HIT");
	audioMng.DeleteBuffer("ZOMBIE_HIT");
	audioMng.DeleteSound("ENEMY_HIT");
	audioMng.DeleteSoundAndBuffer("DODGE");
	audioMng.DeleteSoundAndBuffer("AILMENT_HIT");
	audioMng.DeleteBuffer("BOXER_COUNTER");
	audioMng.DeleteBuffer("CANDLE_COUNTER");
	audioMng.DeleteBuffer("MUSICIAN_COUNTER");
	audioMng.DeleteSound("COUNTER_ATTACK");
	audioMng.DeleteBuffer("BOXER_DEATH");
	audioMng.DeleteBuffer("CANDLE_DEATH");
	audioMng.DeleteSound("PLAYER_DEATH");
	audioMng.DeleteBuffer("KNIGHT_DEATH");
	audioMng.DeleteBuffer("ZOMBIE_DEATH");
	audioMng.DeleteBuffer("FIREBALL_DEATH");
	audioMng.DeleteBuffer("FRANKENCHIEF_DEATH");
	audioMng.DeleteBuffer("CANDLEBOSS_DEATH");
	audioMng.DeleteBuffer("BOO_DEATH");
	audioMng.DeleteSound("ENEMY_DEATH");

	// Candles
	audioMng.DeleteSound("FIREBALL_LAUNCH");
	audioMng.DeleteSoundAndBuffer("FIRE_NOISE");
	audioMng.DeleteBuffer("FIRE_MISS");
	audioMng.DeleteBuffer("FIRE_OK");
	audioMng.DeleteBuffer("FIRE_GREAT");
	audioMng.DeleteBuffer("FIRE_EXCELENT");
	audioMng.DeleteBuffer("FIRE_PERFECT");
	audioMng.DeleteSound("FIRE_IMPACT");

	// Musician
	audioMng.DeleteSoundAndBuffer("MUSICIAN_ATTACK");
}
#pragma endregion

#pragma region EVENT
void FightingState::PollEvent(GameData& _gameData, InGameDatas& _inGameDatas)
{
	if (_gameData.gameEvent.type == sf::Event::JoystickConnected
		|| _gameData.gameEvent.type == sf::Event::JoystickDisconnected)
	{
		InitDisplayedKeys();
		infoKey.setTexture(ControlsManager::Get().GetTexture("FIGHT_INFO"));
	}

	switch (phase)
	{
	case FightingState::ENCOUNTER:
		encounter.Event(_gameData.gameEvent, *_inGameDatas.player, enemies);
		break;
	case FightingState::ACTION_SELECT:
		EventActionSelect(_gameData.gameEvent, _inGameDatas);
		break;
	case FightingState::BUFF_SELECT:
		EventBuffSelect(_gameData.gameEvent, _inGameDatas);
		break;
	case FightingState::TIE_SELECT:
		EventTieSelect(_gameData.gameEvent, _inGameDatas);
		break;
	case FightingState::ITEM_SELECT:
		EventItemSelect(_gameData.gameEvent, _inGameDatas);
		break;
	case FightingState::TARGET_ENEMY:
		EventTargetEnemy(_gameData.gameEvent, _inGameDatas);
		break;
	case FightingState::PLAYER_ATK:
		_inGameDatas.player->GetCurTie()->GetMiniGame()->PollEvent(_gameData.gameEvent, this);
		break;
	case FightingState::ENEMY_ATK:
		enemies[curEnemy]->GetMiniGame()->PollEvent(_gameData.gameEvent, this);
		break;
	case FightingState::PLAYER_RUN:
		EventPlayerRun(_gameData.gameEvent, _inGameDatas);
		break;
	case FightingState::TUTO:
		tutoMenu->PollEvent(_gameData);
		if ((ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "PAUSE") || ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "CANCEL") || ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "CANCEL_2")))
		{
			phase = lastPhase;
			_inGameDatas.tutoActive = false;
			BoolManager::Get()["FIGHT_TUTO"] = true;
		}
		break;
	case FightingState::RESULT_SCREEN:
		EventResultScreen(_gameData.gameEvent, _inGameDatas);
		break;
	default:
		break;
	}

	if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "FIGHT_INFO"))
	{
		_inGameDatas.tutoActive = _inGameDatas.tutoActive ? false : true;

		if (_inGameDatas.tutoActive)
		{
			lastPhase = phase;
			phase = TUTO;
		}
		else
		{
			phase = lastPhase;
			BoolManager::Get()["FIGHT_TUTO"] = true;
		}
	}
}

void FightingState::EventActionSelect(sf::Event& _event, InGameDatas& _inGameDatas)
{
	fightBoxes.Event(_event, FightingBoxes::BOX_TYPE_SIZE);

	if (ControlsManager::Get().IsPressedInput(_event, "INTERACT") || ControlsManager::Get().IsPressedInput(_event, "INTERACT_2"))
	{
		switch (fightBoxes.GetSelectedBox())
		{
		case FightingBoxes::ATTACK:
			if (_inGameDatas.player->GetCostumeState() == CostumeState::MUSIC)
			{
				phase = BUFF_SELECT;
			}
			else
			{
				phase = TARGET_ENEMY;
			}
			break;
		case FightingBoxes::TIES:
			if (_inGameDatas.player->GetTies().size() > 1)
			{
				phase = TIE_SELECT;
			}
			break;
		case FightingBoxes::RUN:
			if (canRun)
			{
				phase = PLAYER_RUN;
				keyLeft.setPosition(_inGameDatas.player->getPosition().x - keyLeft.getGlobalBounds().width / 2.f, _inGameDatas.player->getPosition().y - _inGameDatas.player->GetGlobalAABB().height - 65);
				keyRight.setPosition(_inGameDatas.player->getPosition().x + keyRight.getGlobalBounds().width / 2.f, _inGameDatas.player->getPosition().y - _inGameDatas.player->GetGlobalAABB().height - 65);
				_inGameDatas.player->SetState(PlayerState::MOVE);
				_inGameDatas.player->SetXDir(-1);
			}
			break;
		case FightingBoxes::ITEMS:
			if (_inGameDatas.player->GetConsumables().size() > 0)
			{
				phase = ITEM_SELECT;
			}
			break;
		default:
			break;
		}

		fightBoxes.SetSelectedBox(0);
		audMng.PlaySound("BUTTON_PRESSED");
	}
}
void FightingState::EventBuffSelect(sf::Event& _event, InGameDatas& _inGameDatas)
{
	fightBoxes.Event(_event, _inGameDatas.player->GetApplicableEffects().size());

	if (ctlMng.IsPressedInput(_event, "INTERACT") || ctlMng.IsPressedInput(_event, "INTERACT_2"))
	{
		if (_inGameDatas.player->GetApplicableEffects()[fightBoxes.GetSelectedBox()].status->type == Status::StatusType::BUFF)
		{
			std::shared_ptr<Enemy> nullEnemy = nullptr;
			phase = PLAYER_ATK;
			_inGameDatas.player->GetCurTie()->GetMiniGame()->Init(_inGameDatas.player, nullEnemy, this);
			//fightBoxes.SetSelectedBox(0);
		}
		else
		{
			phase = TARGET_ENEMY;
		}
	}
	else if (ctlMng.IsPressedInput(_event, "CANCEL") || ctlMng.IsPressedInput(_event, "CANCEL_2"))
	{
		phase = ACTION_SELECT;
		audMng.PlaySound("BUTTON_CANCEL");
		fightBoxes.SetSelectedBox(0);
	}
}
void FightingState::EventTieSelect(sf::Event& _event, InGameDatas& _inGameDatas)
{
	fightBoxes.Event(_event, _inGameDatas.player->GetTies().size());

	if (ctlMng.IsPressedInput(_event, "INTERACT") || ctlMng.IsPressedInput(_event, "INTERACT_2"))
	{
		_inGameDatas.player->SetCostumeState(CostumeState(fightBoxes.GetSelectedBox() + 1)); // +1 because of basic tie
		_inGameDatas.player->SetApplicableStatus();
		phase = ACTION_SELECT;
		fightBoxes.SetSelectedBox(0);
	}
	else if (ctlMng.IsPressedInput(_event, "CANCEL") || ctlMng.IsPressedInput(_event, "CANCEL_2"))
	{
		phase = ACTION_SELECT;
		fightBoxes.SetSelectedBox(0);
		audMng.PlaySound("BUTTON_CANCEL");
	}
}
void FightingState::EventItemSelect(sf::Event& _event, InGameDatas& _inGameDatas)
{
	fightBoxes.Event(_event, _inGameDatas.player->GetConsumables().size());

	if (ctlMng.IsPressedInput(_event, "INTERACT") || ctlMng.IsPressedInput(_event, "INTERACT_2"))
	{
		if (_inGameDatas.player->GetConsumables()[fightBoxes.GetSelectedBox()]->GetEffect() == Consumable::Effect::BOMB)
		{
			std::vector<std::shared_ptr<Entity>> enemiesCast = {};
			for (auto& enemy : enemies)
			{
				if (enemy->GetHP() > 0)
				{
					// pourquoi..
					enemiesCast.push_back(std::static_pointer_cast<Entity>(enemy));
				}
			}
			_inGameDatas.player->GetConsumables()[fightBoxes.GetSelectedBox()]->ApplyEffectAoE(enemiesCast);
		}
		else
		{
			// pourquoi..
			std::shared_ptr<Entity> player = std::static_pointer_cast<Entity>(_inGameDatas.player);
			_inGameDatas.player->GetConsumables()[fightBoxes.GetSelectedBox()]->ApplyEffect(player);
		}

		fightBoxes.SetSelectedBox(0);
	}
	else if (ctlMng.IsPressedInput(_event, "CANCEL") || ctlMng.IsPressedInput(_event, "CANCEL_2"))
	{
		phase = ACTION_SELECT;
		fightBoxes.SetSelectedBox(0);
		audMng.PlaySound("BUTTON_CANCEL");
	}
}

void FightingState::EventTargetEnemy(sf::Event& _event, InGameDatas& _inGameDatas)
{
	if (ctlMng.IsPressedInput(_event, "LEFT") || ctlMng.IsPressedInput(_event, "UP"))
	{
		MoveCursorTargetEnemy(false);
	}
	else if (ctlMng.IsPressedInput(_event, "RIGHT") || ctlMng.IsPressedInput(_event, "DOWN"))
	{
		MoveCursorTargetEnemy(true);
	}

	if (ctlMng.IsPressedInput(_event, "INTERACT") || ctlMng.IsPressedInput(_event, "INTERACT_2"))
	{
		phase = PLAYER_ATK;
		_inGameDatas.player->GetCurTie()->GetMiniGame()->Init(_inGameDatas.player, enemies[selectEnemy], this);
	}
	else if (ctlMng.IsPressedInput(_event, "CANCEL") || ctlMng.IsPressedInput(_event, "CANCEL_2"))
	{
		phase = ACTION_SELECT;
		audMng.PlaySound("BUTTON_CANCEL");
	}
}

void FightingState::EventPlayerRun(sf::Event& _event, InGameDatas& _inGameDatas)
{
	if (_event.type == sf::Event::JoystickConnected || _event.type == sf::Event::JoystickDisconnected)
	{
		InitDisplayedKeys();
	}

	if (ctlMng.IsPressedInput(_event, leftKeyID))
	{
		if ((clickCount == 0) || (clickCount != 0 && (lastKeyPressed != leftKeyID)))
		{
			++clickCount;
			lastKeyPressed = leftKeyID;
		}
		else
		{
			runFinished = true;
			playerRunning = true;
			_inGameDatas.player->GetCurAnim()->SetSpeed(2.f);
			timerRun = 0.f;
		}
	}
	else if (ctlMng.IsPressedInput(_event, rightKeyID))
	{
		if ((clickCount == 0) || (clickCount != 0 && (lastKeyPressed != rightKeyID)))
		{
			++clickCount;
			lastKeyPressed = rightKeyID;
		}
		else
		{
			runFinished = true;
			playerRunning = true;
			_inGameDatas.player->GetCurAnim()->SetSpeed(2.f);
			timerRun = 0.f;
		}
	}
}

void FightingState::EventResultScreen(sf::Event& _event, InGameDatas& _inGameDatas)
{
	if (_inGameDatas.player->GetHP() > 0)
	{
		if (ctlMng.IsPressedInput(_event, "INTERACT") || ctlMng.IsPressedInput(_event, "INTERACT_2"))
		{
			End(_inGameDatas);
		}
	}
	else
	{
		if (!fade->GetIsFadingIn())
		{
			fade->SetIsFadingIn(true);
		}
	}
}
#pragma endregion

#pragma region UPDATE
void FightingState::Update(GameData& _gameData, InGameDatas& _inGameDatas)
{
	// Update tuto
	if (_inGameDatas.player->hasNewTie)
	{
		_inGameDatas.player->hasNewTie = false;
		tutoMenu->GetTutoPages()->ClearTiesPages();
		for (size_t i = 0; i < _inGameDatas.player->GetTies().size(); ++i)
		{
			std::vector<std::string> newKeys;
			std::string id = _inGameDatas.player->GetTies()[i]->GetId();

			newKeys.push_back("TUTO_FIGHT_" + id);
			std::vector<std::string> newPathes;
			newPathes.push_back("TUTO_FIGHT_" + id + ".txt");
			tutoMenu->GetTutoPages()->AddNewTuto(newKeys, newPathes);
		}
	}

	hud.Update(*_inGameDatas.player, encounter.GetTimer().GetTimeLeft());
	// Update player
	_inGameDatas.player->Update(_inGameDatas);
	for (auto& status : _inGameDatas.player->GetStatutes())
	{
		drwMng.SetObjectVisible("FIGHT_HUD", status, (_inGameDatas.player->GetHP() > 0 && status->isAppearing) || (phase == ACTION_SELECT || phase == TIE_SELECT || phase == ITEM_SELECT || phase == TARGET_ENEMY || phase == BUFF_SELECT));
	}
	// Update enemies
	for (auto& enemy : enemies)
	{
		enemy->Update(_inGameDatas);
		DrawManager::Get().SetObjectVisible("FIGHT", enemy.get(), enemy->IsAlive());
		for (auto& status : enemy->GetStatutes())
		{
			drwMng.SetObjectVisible("FIGHT_HUD", status, (status->isAppearing && enemy->IsAlive()) || ((phase == ACTION_SELECT || phase == TIE_SELECT || phase == ITEM_SELECT || phase == TARGET_ENEMY || phase == BUFF_SELECT)));
		}
	}

	switch (phase)
	{
	case FightingState::ENCOUNTER:
		encounter.Update(*_inGameDatas.player, enemies);

		if (encounter.IsFinished())
		{
			if (!BoolManager::Get()["FIGHT_TUTO"])
			{
				_inGameDatas.tutoActive = true;
				lastPhase = phase;
				phase = TUTO;
			}
			else
			{
				phase = ACTION_SELECT;
			}
		}
		break;
	case FightingState::ACTION_SELECT:
		fightBoxes.Update(*_inGameDatas.player, FightingBoxes::BOX_TYPE_SIZE);
		fightBoxes.UpdateActionTexture(*_inGameDatas.player, canRun);

		if (HasPlayerWon())
		{
			EndCombat(_inGameDatas);
		}
		break;
	case FightingState::BUFF_SELECT:
		fightBoxes.Update(*_inGameDatas.player, _inGameDatas.player->GetApplicableEffects().size());
		fightBoxes.UpdatePartitionTexture(*_inGameDatas.player);

		if (HasPlayerWon())
		{
			EndCombat(_inGameDatas);
		}
		break;
	case FightingState::TIE_SELECT:
		fightBoxes.Update(*_inGameDatas.player, _inGameDatas.player->GetTies().size());
		fightBoxes.UpdateTieTexture(*_inGameDatas.player);

		if (HasPlayerWon())
		{
			EndCombat(_inGameDatas);
		}
		break;
	case FightingState::ITEM_SELECT:
		// safety if player use all his consumables
		if (_inGameDatas.player->GetConsumables().size() == 0)
		{
			phase = ACTION_SELECT;
			break;
		}

		fightBoxes.Update(*_inGameDatas.player, _inGameDatas.player->GetConsumables().size());
		fightBoxes.UpdateItemTexture(*_inGameDatas.player);

		if (HasPlayerWon())
		{
			EndCombat(_inGameDatas);
		}
		break;
	case FightingState::TARGET_ENEMY:
		UpdateTargetEnemy(_inGameDatas);
		break;
	case FightingState::PLAYER_ATK:
		_inGameDatas.player->GetCurTie()->GetMiniGame()->Update(this);
		if (_inGameDatas.player->GetCurTie()->GetMiniGame()->IsOver())
		{
			curEnemy = 0;
			enemies[curEnemy]->GetMiniGame()->Init(_inGameDatas.player, enemies[curEnemy], this);
			phase = ENEMY_ATK;
			CheckPlayerDead(_inGameDatas);
		}
		break;
	case FightingState::ENEMY_ATK:
		UpdateEnemiesAtk(_inGameDatas);
		break;
	case FightingState::PLAYER_RUN:
		UpdatePlayerRun(_inGameDatas);
		break;
	case FightingState::TUTO:
		tutoMenu->Update(_gameData);
		break;
	case FightingState::RESULT_SCREEN:
		UpdateResultScreen(_inGameDatas);
		break;
	default:
		break;
	}
}

void FightingState::UpdateTargetEnemy(InGameDatas& _inGameDatas)
{
	// security if the player target a dead enemy
	if (enemies[selectEnemy]->GetHP() <= 0)
	{
		for (int i = 0; i < enemies.size(); i++)
		{
			if (--selectEnemy < 0)
			{
				selectEnemy = (int)enemies.size() - 1;
			}

			if (enemies[selectEnemy]->GetHP() > 0)
			{
				break;
			}
		}
	}

	if (ControlsManager::Get().IsPressedDirection("LEFT") || ctlMng.IsPressedDirection("UP"))
	{
		MoveCursorTargetEnemy(false);
	}
	else if (ControlsManager::Get().IsPressedDirection("RIGHT") || ctlMng.IsPressedDirection("DOWN"))
	{
		MoveCursorTargetEnemy(true);
	}

	const float deltaPos = sin(GameClock::Get().GetTotalTime() * 6.f) * 10.f;
	const sf::FloatRect& rect = enemies[selectEnemy]->GetGlobalAABB();

	enemyName.setPosition(enemies[selectEnemy]->getPosition().x, enemies[selectEnemy]->getPosition().y + 6.f);
	enemyName.setString(enemies[selectEnemy]->GetName());
	enemyName.setOrigin(enemyName.getGlobalBounds().width / 2.f, enemyName.getGlobalBounds().height / 2.f);
	enemyName.setFillColor(enemies[selectEnemy]->GetHpColor());

	selectArrow.setPosition(rect.left - 50.f + deltaPos, enemies[selectEnemy]->getPosition().y - rect.height / 2.f);
}

void FightingState::UpdateEnemiesAtk(InGameDatas& _inGameDatas)
{
	enemies[curEnemy]->GetMiniGame()->Update(this);
	if (enemies[curEnemy]->GetMiniGame()->IsOver())
	{
		if (++curEnemy >= enemies.size())
		{
			curEnemy = 0;
			EndTurn(_inGameDatas);
		}
		else
		{
			enemies[curEnemy]->GetMiniGame()->Init(_inGameDatas.player, enemies[curEnemy], this);
		}

		CheckPlayerDead(_inGameDatas);
	}
}

void FightingState::UpdatePlayerRun(InGameDatas& _inGameDatas)
{
	const float dt = GameClock::Get().GetDeltaTime();

	if (!runFinished)
	{
		timerRun += dt;

		if (keyLeft.getScale().x == PRESSED_SCALE)
		{
			timerResetScale -= dt;
			if (timerResetScale <= 0.f)
			{
				keyLeft.setScale(1.f, 1.f);
				keyRight.setScale(PRESSED_SCALE, PRESSED_SCALE);
				timerResetScale = TIMER_SCALE_MAX;
			}
		}
		else
		{
			timerResetScale -= dt;
			if (timerResetScale <= 0.f)
			{
				keyLeft.setScale(PRESSED_SCALE, PRESSED_SCALE);
				keyRight.setScale(1.f, 1.f);
				timerResetScale = TIMER_SCALE_MAX;
			}
		}


		if (_inGameDatas.player->GetCostumeState() == CostumeState::CANDLE)
		{
			if (_inGameDatas.player->GetCurAnim()->GetCurrentFrame() == 2)
			{
				audMng.PlaySound("LEAK", 75.f);
			}
		}
		else
		{
			audMng.PlaySound("LEAK", 40.f);
		}

		if (timerRun >= TIMER_RUN_MAX)
		{
			runFinished = true;
			playerRunning = true;
			_inGameDatas.player->GetCurAnim()->SetSpeed(2.f);
			timerRun = 0.f;
		}
	}
	else
	{
		audMng.StopSound("LEAK");
		if (_inGameDatas.player->GetCostumeState() == CostumeState::CANDLE)
		{
			audMng.ChangeBuffer("LEAK", "LEAK_END_CANDLE");
			audMng.SetSoundVolume("LEAK", 75.f);
		}
		else
		{
			audMng.ChangeBuffer("LEAK", "LEAK_END");
			audMng.SetSoundVolume("LEAK", 40.f);
		}

		audMng.PlaySound("LEAK");
		earn.sweets = 0; // Pour enlever les bonbons drop par les ennemis.
		if (clickCount <= NB_CLICK_MISS_MAX)
		{
			earn.sweets -= 25;
		}
		else if (clickCount <= NB_CLICK_OK_MAX)
		{
			earn.sweets -= 17;
		}
		else if (clickCount <= NB_CLICK_GREAT_MAX)
		{
			earn.sweets -= 9;
		}
		else
		{
			earn.sweets -= 5;
		}

		EndCombat(_inGameDatas);
	}
}

void FightingState::UpdateResultScreen(InGameDatas& _inGameDatas)
{
	if (playerRunning)
	{
		_inGameDatas.player->MovePlayerInFight(sf::Vector2f(-200.f, _inGameDatas.player->getPosition().y), -1, 900.f);
		if (_inGameDatas.player->getPosition().x <= -200.f)
		{
			playerRunning = false;
			_inGameDatas.player->GetCurAnim()->SetSpeed(1.f);
		}
	}
	
	fade->Update(GameClock::Get().GetDeltaTime(), "FIGHT_MUSIC");
	if (fade->GetTimer() >= fade->GetTimerMax())
	{
		End(_inGameDatas);
	}
}

void FightingState::CheckPlayerDead(InGameDatas& _inGameDatas)
{
	if (_inGameDatas.player->GetHP() <= 0)
	{
		_inGameDatas.player->SetState(PlayerState::DEATH);
		EndCombat(_inGameDatas);
	}
}
#pragma endregion

void FightingState::Display(InGameDatas& _inGameDatas, sf::RenderWindow& _window)
{
	//_window.setMouseCursorVisible(tutoActive);

	fightBoxes.SetVisible(phase == ACTION_SELECT || phase == TIE_SELECT || phase == ITEM_SELECT || phase == BUFF_SELECT);

	drwMng.SetObjectVisible("FIGHT_HUD", &selectArrow, (phase == TARGET_ENEMY));

	drwMng.SetObjectVisible("FIGHT_HUD", &keyLeft, phase == PLAYER_RUN);
	drwMng.SetObjectVisible("FIGHT_HUD", &keyRight, phase == PLAYER_RUN);
	drwMng.SetObjectVisible("FIGHT_HUD", &winScreen, (phase == RESULT_SCREEN && HasPlayerWon()));
	drwMng.SetObjectVisible("FIGHT_HUD", &defeatScreen, (phase == RESULT_SCREEN && !HasPlayerWon()));
	drwMng.SetObjectVisible("FIGHT_HUD", &enemyName, phase == TARGET_ENEMY);

	// Background display
	drwMng.Draw(_window, "FIGHT_BG");

	// Enemies & Player display
	drwMng.Draw(_window, "FIGHT", true);

	//HUD display
	if (phase != ENCOUNTER)
	{
		drwMng.Draw(_window, "FIGHT_BOXES", true);
		drwMng.Draw(_window, "FIGHT_HUD");
	}

	if (_inGameDatas.tutoActive)
	{
		tutoMenu->Display(_window);
	}
}

void FightingState::AddEnemy(const std::shared_ptr<Enemy>& _enemy)
{
	if (enemies.size() >= ENEMIES_MAX_SIZE)
	{
		bool addEnemy = false;
		for (int i = 0; i < ENEMIES_MAX_SIZE; i++)
		{
			if (!enemies[i]->IsAlive())
			{
				addEnemy = true;

				enemies[i].reset();
				enemies[i] = _enemy;
				enemies[i]->SetIsInFight(true);

				enemies[i]->setPosition(ENEMIES_FIGHT_POS[i]);
				drwMng.AddObject("FIGHT", *enemies[i]);
				break;
			}
		}

		if (!addEnemy)
		{
			Debug.warning("Enemies vector full : can't add new enemy in fight");
			return;
		}
	}
	else
	{
		enemies.push_back(_enemy);
		const size_t t = enemies.size() - 1;
		enemies[t]->SetIsInFight(true);

		enemies[t]->setPosition(ENEMIES_FIGHT_POS[t]);
		drwMng.AddObject("FIGHT", *enemies[t]);
	}
}

const int FightingState::GetEnemiesSize(void)
{
	return static_cast<const int>(enemies.size());
}

const size_t& FightingState::GetSelectedPartition(void) const
{
	return fightBoxes.GetSelectedBox();
}

const bool FightingState::HasPlayerWon(void)
{
	int numEnemyDead = 0;
	for (auto& enemy : enemies)
	{
		if (enemy->GetHP() <= 0)
		{
			numEnemyDead++;
		}
	}

	return numEnemyDead == enemies.size();
}

void FightingState::AddingToEarningsScreen(const sf::Texture& _item, int _nbItems)
{
	std::unique_ptr<WinItem> resultItem = std::make_unique<WinItem>();
	sf::RectangleShape itemSprite;
	itemSprite.setSize({ 75.f,75.f });
	itemSprite.setTexture(&_item);
	sf::Font& font = fntMng["DEFAULT"];
	sf::FloatRect wGB = winScreen.getGlobalBounds();
	sf::Text nbItem(std::to_string(std::abs(_nbItems)), font);
	std::unique_ptr<sf::Sprite> PlusMinus = std::make_unique<sf::Sprite>();
	plusOrMinus.push_back(std::move(PlusMinus));
	if (_nbItems > 0)
	{
		plusOrMinus[plusOrMinus.size() - 1]->setTexture(texMng["VICTORY_PLUS"]);
	}
	else
	{
		plusOrMinus[plusOrMinus.size() - 1]->setTexture(texMng["DEFEAT_MINUS"]);
	}

	itemSprite.setOrigin(itemSprite.getGlobalBounds().width / 2, itemSprite.getGlobalBounds().height / 2);
	nbItem.setOrigin(nbItem.getGlobalBounds().width / 2, nbItem.getGlobalBounds().height / 2);
	plusOrMinus[plusOrMinus.size() - 1]->setOrigin(plusOrMinus[plusOrMinus.size() - 1]->getGlobalBounds().width / 2, plusOrMinus[plusOrMinus.size() - 1]->getGlobalBounds().height / 2);

	itemSprite.setPosition(wGB.left + OFFSET_WIN_SCREEN_ITEM_X, wGB.top + OFFSET_WIN_SCREEN_ITEM_Y + OFFSET_BETWEEN_ITEM * earn.resultItem.size());
	nbItem.setPosition(wGB.left + OFFSET_WIN_SCREEN_ITEM_X - 75, wGB.top + OFFSET_WIN_SCREEN_ITEM_Y + OFFSET_BETWEEN_ITEM * earn.resultItem.size());
	plusOrMinus[plusOrMinus.size() - 1]->setPosition(wGB.left + OFFSET_WIN_SCREEN_ITEM_X - 120, wGB.top + 10 + OFFSET_WIN_SCREEN_ITEM_Y + OFFSET_BETWEEN_ITEM * earn.resultItem.size());

	resultItem->item = itemSprite;
	resultItem->nbItem = nbItem;
	drwMng.AddObject("FIGHT_HUD", resultItem->item);
	drwMng.AddObject("FIGHT_HUD", resultItem->nbItem);
	drwMng.AddObject("FIGHT_HUD", *plusOrMinus[plusOrMinus.size() - 1]);
	earn.resultItem.push_back(std::move(resultItem));
}
const bool FightingState::IsInLootedItem(const Item& _item)
{
	for (int i = 0; i < earn.loots.size(); i++)
	{
		if (earn.loots[i]->item->GetId() == _item.GetId())
		{
			return true;
		}
	}
	return false;
}
void FightingState::IncrementLootedItem(const Item& _item)
{
	for (int i = 0; i < earn.loots.size(); i++)
	{
		if (earn.loots[i]->item->GetId() == _item.GetId())
		{
			earn.loots[i]->nbItems++;
			return;
		}
	}
}
void FightingState::LootEnemy(Enemy& _enemy)
{
	earn.sweets += _enemy.GetSweetsDrop();
	for (int j = 0; j < _enemy.GetLootableItems().size(); j++)
	{
		for (int k = 0; k < _enemy.GetLootableItems()[j]->nbMax; k++)
		{
			float probabilityToGetItem = _enemy.GetLootableItems()[j]->probability * 100;
			const bool isItemLooted = rand() % 100 < probabilityToGetItem;
			if (isItemLooted)
			{
				if (IsInLootedItem(*_enemy.GetLootableItems()[j]->item))
				{
					IncrementLootedItem(*_enemy.GetLootableItems()[j]->item);
				}
				else
				{
					std::shared_ptr<ItemLooted> tempItemLooted = std::make_shared<ItemLooted>();
					tempItemLooted->item = _enemy.GetLootableItems()[j]->item;
					tempItemLooted->nbItems = 1;
					earn.loots.push_back(std::move(tempItemLooted));
				}
			}
			else
			{
				// La boucle continue de nous give l'item tant qu'on n'a pas atteint le max donné.
				// Mais si la proba fait que nous ne droppons pas l'item alors la boucle se finit.
				break;
			}
		}
	}
}
void FightingState::Earnings(Player& _player)
{
	if (HasPlayerWon())
	{
		for (auto& enemy : enemies)
		{
			LootEnemy(*enemy);
		}

		if (earn.sweets != 0)
		{
			_player.AddSweets(earn.sweets);

			AddingToEarningsScreen(texMng["CANDY"], earn.sweets);
		}

		for (int i = 0; i < earn.loots.size(); i++)
		{
			sf::Sprite tempSprite;
			tempSprite.setTexture(*earn.loots[i]->item->GetTexture());
			for (int j = 0; j < earn.loots[i]->nbItems; j++)
			{
				_player.AddItem(Item(*earn.loots[i]->item));
			}
			AddingToEarningsScreen(*tempSprite.getTexture(), earn.loots[i]->nbItems);
		}
	}
	else
	{
		if (earn.sweets != 0)
		{
			_player.AddSweets(earn.sweets);

			AddingToEarningsScreen(texMng["CANDY"], earn.sweets);
		}
	}
}

void FightingState::MoveCursorTargetEnemy(const bool& _forward)
{
	if (_forward)
	{
		for (int i = 0; i < enemies.size(); i++)
		{
			if (++selectEnemy >= enemies.size())
			{
				selectEnemy = 0;
			}

			if (enemies[selectEnemy]->GetHP() > 0)
			{
				break;
			}
		}
	}
	else
	{
		for (int i = 0; i < enemies.size(); i++)
		{
			if (--selectEnemy < 0)
			{
				selectEnemy = (int)enemies.size() - 1;
			}

			if (enemies[selectEnemy]->GetHP() > 0)
			{
				break;
			}
		}
	}

	Audio::Get().PlaySoundNoSecurity("BUTTON_SWAP");
}