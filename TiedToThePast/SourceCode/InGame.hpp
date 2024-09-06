#ifndef INGAME_HPP
#define INGAME_HPP

#include "State.hpp"
#include "SceneManager.hpp"
#include "InGameDatas.hpp"
#include "CollisionManager.hpp"
#include "PauseMenu.hpp"
#include "SettingsMenu.hpp"
#include "CharacterMenu.hpp"
#include "FightingState.hpp"
#include "ExplorationState.hpp"
#include "Shop.hpp"
#include "SaveMenu.hpp"
#include "CircularMenu.hpp"

class InGame : public State
{
public:
	InGame() = default;

	void Init(GameData& _gameData) override;
	void Update(GameData& _gameData) override;
	void PollEvent(GameData& _gameData) override;
	void Display(sf::RenderWindow& _window) override;

	friend class SaveSystem;

private:
	static constexpr auto DEFAULT_MAP = "Contents/Data/Tiled/Maps/Garden.json";

	enum PhaseStatesEnum
	{
		EXPLO,
		FIGHT
	};

	void InitTexture(void);
	void InitEvents(GameData& _gameData);
	void InitPrefabs(void);
	void InitSubStates(GameData& _gameData);

	void UpdateSubState(GameData& _gameData);
	void SubStateEventHandler(GameData& _gameData);
	void ChangeSubState(GameData& _gameData, const SubStates& _subState);
	void ChangePhaseState(GameData& _gameData);
	void UpdatePhaseState(GameData& _gameData, sf::RenderWindow& _window);
	void SwitchSubState(GameData& _gameData, SubStates _subState);

public:
	InGameDatas inGameDatas = InGameDatas();

private:
	~InGame() override;
	void ReloadExploMusic(void);

	std::unique_ptr<ExplorationState> exploState = nullptr;
	std::unique_ptr<FightingState> fightState = nullptr;
	PhaseStatesEnum currentPhaseStateID = PhaseStatesEnum::EXPLO;

	std::vector<std::shared_ptr<State>> subStates;
	std::shared_ptr<State> currentSubState = nullptr;
	SubStates currentSubStateID = NULL_STATE;

	Enemy* lastEnemyColliding = nullptr;
	Audio& audioMng = Audio::Get();
	Fade* fadeIn2sec = nullptr;
	bool isExploMusicFading = false;
};

#endif // !INGAME_HPP
