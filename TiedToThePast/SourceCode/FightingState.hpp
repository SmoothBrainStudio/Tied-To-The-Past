#ifndef FIGHTING_STATE__HPP
#define FIGHTING_STATE__HPP

#include "PhaseState.hpp"

#include "TutoMenu.hpp"
#include "FightingHUD.hpp"
#include "FightingBoxes.hpp"
#include "Encounter.hpp"
#include "Item.hpp"

static constexpr float OFFSET_WIN_SCREEN_ITEM_X = 385.f;
static constexpr float OFFSET_WIN_SCREEN_ITEM_Y = 365.f;
static constexpr float OFFSET_BETWEEN_ITEM = 85.f;

class FightingState : public PhaseState
{
public:
	static constexpr size_t ENEMIES_MAX_SIZE = 3;
	enum Phase
	{
		ENCOUNTER,

		ACTION_SELECT,
		BUFF_SELECT,
		TIE_SELECT,
		ITEM_SELECT,

		TARGET_ENEMY,
		PLAYER_ATK,
		ENEMY_ATK,

		PLAYER_RUN,

		TUTO,
		RESULT_SCREEN
	};

	void EventActionSelect(sf::Event& _event, InGameDatas& _inGameDatas);
	void EventBuffSelect(sf::Event& _event, InGameDatas& _inGameDatas);
	void EventTieSelect(sf::Event& _event, InGameDatas& _inGameDatas);
	void EventItemSelect(sf::Event& _event, InGameDatas& _inGameDatas);
	void EventTargetEnemy(sf::Event& _event, InGameDatas& _inGameDatas);
	void EventPlayerRun(sf::Event& _event, InGameDatas& _inGameDatas);
	void EventResultScreen(sf::Event& _event, InGameDatas& _inGameDatas);
	
	void UpdateTargetEnemy(InGameDatas& _inGameDatas);
	void UpdateEnemiesAtk(InGameDatas& _inGameDatas);
	void UpdatePlayerRun(InGameDatas& _inGameDatas);
	void UpdateResultScreen(InGameDatas& _inGameDatas);
	void CheckPlayerDead(InGameDatas& _inGameDatas);

	FightingState(GameData& _gameData, InGameDatas& _inGameDatas);
	~FightingState(void);

	void Init(GameData& _gameData, InGameDatas& _inGameDatas) override;
	void PollEvent(GameData& _gameData, InGameDatas& _inGameDatas) override;
	void Update(GameData& _gameData, InGameDatas& _inGameDatas) override;
	void Display(InGameDatas& _inGameDatas, sf::RenderWindow& _window) override;

	// start the combat
	void Start(InGameDatas& _inGameDatas);
	// 
	void End(InGameDatas& _inGameDatas);

	void EndCombat(InGameDatas& _inGameDatas);
	void EndTurn(InGameDatas& _inGameDatas);

	void AddEnemy(const std::shared_ptr<Enemy>& _enemy);
	void LootEnemy(Enemy& _enemy);
	const int GetEnemiesSize(void);
	const size_t& GetSelectedPartition(void) const;

private:
	void AddingToEarningsScreen(const sf::Texture& _item, int _nbItems); // ITEM
	const bool IsInLootedItem(const Item& _item); // ITEM
	void Earnings(Player& _player); // HUD
	void MoveCursorTargetEnemy(const bool& _forward);
	void IncrementLootedItem(const Item& _item);
	void CloseEarnings(InGameDatas& _inGameDatas);

	void InitUI(void);
	void InitDisplayedKeys(void);

	void InitSounds(void);
	void DeleteSounds(void);

	void InitTextures(void);
	void DeleteTextures(void);

	const bool HasPlayerWon(void);

	struct WinItem
	{
		sf::RectangleShape item;
		sf::Text nbItem;
	};

	class ItemLooted
	{
	public:
		~ItemLooted() = default;
		std::shared_ptr<Item> item = nullptr;
		int nbItems = 0;
	};

	class Earn
	{
	public:
		~Earn() = default;
		short sweets = 0;
		std::vector<std::shared_ptr<ItemLooted>> loots;
		std::vector<std::unique_ptr<WinItem>> resultItem;
	};

	#pragma region TEMP
	Audio& audMng = Audio::Get();
	DrawManager& drwMng = DrawManager::Get();
	EventManager& evtMng = EventManager::Get();
	FontsManager& fntMng = FontsManager::Get();
	TexturesManager& texMng = TexturesManager::Get();
	ControlsManager& ctlMng = ControlsManager::Get();

	sf::Sprite selectArrow;		// HUD

	sf::Sprite infoButton;		// TUTO
	sf::Sprite infoKey;			// TUTO
	
	sf::Sprite VictoryTitle;	// HUD WIN/LOOSE
	sf::Sprite LooseTitle;		// HUD WIN/LOOSE
	sf::Sprite winScreen;		// HUD WIN/LOOSE
	sf::Sprite defeatScreen;	// HUD WIN/LOOSE
	std::vector<std::unique_ptr<sf::Sprite>> plusOrMinus; // WIN/LOOSE

	sf::Text enemyName;			// HUD

	std::unique_ptr<Fade> fade = nullptr;					// HUD

	bool canRun = true;

	int curEnemy = 0;	// current enemy mini game
	int selectEnemy = 0;	// target enemy

	sf::Sprite keyLeft;												// RUN
	sf::Sprite keyRight;											// RUN
	bool playerRunning = false;										// RUN
	float timerResetScale = TIMER_SCALE_MAX;						// RUN
	float timerRun = 0.f;											// RUN
	bool runFinished = false;										// RUN
	std::string leftKeyID;											// RUN
	std::string rightKeyID;											// RUN
	std::string lastKeyPressed;										// RUN
	int clickCount = 0;												// RUN
	static constexpr float TIMER_RUN_MAX = 5.f;						// RUN
	static constexpr float TIMER_SCALE_MAX = 0.247f;				// RUN
	static constexpr float PRESSED_SCALE = 0.75f;					// RUN
	static constexpr unsigned short NB_CLICK_MISS_MAX = 8;			// RUN
	static constexpr unsigned short NB_CLICK_OK_MAX = 16;			// RUN
	static constexpr unsigned short NB_CLICK_GREAT_MAX = 24;		// RUN
	#pragma endregion

	Phase phase;

	std::vector<std::shared_ptr<Enemy>> enemies;

	Earn earn;

	FightingBoxes fightBoxes;
	FightingHUD hud;
	Encounter encounter;
	std::unique_ptr<TutoMenu> tutoMenu;

	Phase lastPhase;
};

#endif // !FIGHTING_STATE__HPP