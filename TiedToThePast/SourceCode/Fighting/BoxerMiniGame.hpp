#ifndef BOXER_MINIGAME_HPP
#define BOXER_MINIGAME_HPP

#include "FightingMiniGames.hpp"

static constexpr float MAX_ATTACK_BOXER_TIME = 0.5f;
static constexpr float MAX_BAR_TIME = 3.f;
static constexpr int MAX_BAR_SCORE = 21;
static constexpr float MAX_BAR_WIDTH = 1344.f;
static constexpr float BURNOUT_WIDTH = 1155.f;
static constexpr float EXCELLENT_WIDTH = 1038.f;
static constexpr float GREAT_WIDTH = 807.f;
static constexpr float OK_WIDTH = 462.f;

static constexpr float BOXER_PERFECT_EXCELLENT_MULTIPLICATOR = 4.f;
static constexpr float BOXER_GREAT_MULTIPLICATOR = 2.67f;// 8/3
static constexpr float BOXER_OK_MULTIPLICATOR = 1.34f; // 4/3
static constexpr float BOXER_MISS_MULTIPLICATOR = 0;

class BoxerMiniGame : public MiniGame
{
public:
	BoxerMiniGame();
	~BoxerMiniGame() override;

	void InitMiniGame(std::shared_ptr<Player>& _player, std::shared_ptr<Enemy>& _enemy, FightingState* _fightingState) override;
	void UpdateMiniGame(FightingState* _fightingState) override;
	void PollEventMiniGame(sf::Event& _event, FightingState* _fightingState) override;

private:
	Audio& audio = Audio::Get();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void Start(FightingState* _fightingState);
	void StartOver(FightingState* _fightingState);
	void On(FightingState* _fightingState);
	void OnOver(FightingState* _fightingState);
	void End(FightingState* _fightingState);
	void EndOver(FightingState* _fightingState);

	typedef void (BoxerMiniGame::* StateFunc)(FightingState* _fightingState);
	StateFunc state = static_cast<StateFunc>(&BoxerMiniGame::Start);

	static constexpr float PRESSED_SCALE_BOXER = 0.75f;
	static constexpr float TIMER_SCALE_MAX_BOXER = 0.185f;
	float timerResetScale;
	ParticleSystem* particlePressedTouch;

	int GetDamage();

	enum class StateEvent
	{
		PLAYER_INTERACTION,
		NOT_INTERACTION
	};

	int initPlayerDirection;

	StateEvent stateEvent = StateEvent::NOT_INTERACTION;
	sf::Sprite keyInteract;
	int clickCount;
	bool burnOut;

};

#endif // !BOXER_MINIGAME_HPP
