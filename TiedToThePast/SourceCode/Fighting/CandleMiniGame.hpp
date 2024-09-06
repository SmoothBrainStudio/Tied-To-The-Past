#ifndef CANDLE_MINI_GAME__HPP
#define CANDLE_MINI_GAME__HPP

#include "../Common.hpp"
#include "FightingMiniGames.hpp"

static constexpr float START_TIME_CANDLE = 0.f;
static constexpr float END_TIME_CANDLE = 2.5f;
static constexpr float END_TIME_FIREBALL = 1.f;
static constexpr int NB_FRAME_FIREBALL = 14;

static constexpr float CANDLE_PERFECT_EXCELLENT_MULTIPLICATOR = 2.12f; //5 / 3
static constexpr float CANDLE_GREAT_MULTIPLICATOR = 1.62f;
static constexpr float CANDLE_OK_MULTIPLICATOR = 1.21f; // 2/3
static constexpr float CANDLE_MISS_MULTIPLICATOR = 0;

class CandleMiniGame : public MiniGame
{
public:
	CandleMiniGame();
	~CandleMiniGame() override;

	void InitMiniGame(std::shared_ptr<Player>& _player, std::shared_ptr<Enemy>& _enemy, FightingState* _fightingState) override;
	void UpdateMiniGame(FightingState* _fightingState) override;
	void PollEventMiniGame(sf::Event& _event, FightingState* _fightingState) override;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void Start(FightingState* _fightingState);
	void StartOver(FightingState* _fightingState);
	void On(FightingState* _fightingState);
	void OnOver(FightingState* _fightingState);
	void End(FightingState* _fightingState);
	void EndOver(FightingState* _fightingState);

	typedef void (CandleMiniGame::* StateFunc)(FightingState* _fightingState);
	StateFunc state = static_cast<StateFunc>(&CandleMiniGame::Start);

	int GetDamage();

	enum class StateEvent
	{
		PLAYER_INTERACTION,
		FIREBALL_ANIMATION,
		NOT_INTERACTION
	};

	sf::Sprite keyInteract;

	int levelFireball = 1;
	int indexFireball;
	int currentDamage;

	float interactKeyTime;
	static constexpr float ANIM_FIREBALL_DURATION = 1.4f;
	sf::Vector2f startPosition;
	sf::Vector2f endPosition;

	StateEvent stateEvent = StateEvent::NOT_INTERACTION;

	bool isThrowing = false;
	bool isThrowingPerfect = false;
	bool fireballLaunched = false;
	bool keyIsAppearing = true;

	AnimatedSprite fireballAnim;

	Audio& audio = Audio::Get();
};

#endif // !CANDLE_MINI_GAME__HPP



