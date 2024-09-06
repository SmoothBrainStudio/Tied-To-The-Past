#ifndef KNIGHT_MINI_GAME__HPP
#define KNIGHT_MINI_GAME__HPP

#include "FightingMiniGames.hpp"

class KnightMiniGame : public MiniGame
{
public:
	KnightMiniGame(void);
	~KnightMiniGame(void) override;

	void InitMiniGame(std::shared_ptr<Player>& _player, std::shared_ptr<Enemy>& _enemy, FightingState* _fightingState) override;
	void UpdateMiniGame(FightingState* _fightingState) override;
	void PollEventMiniGame(sf::Event& _event, FightingState* _fightingState) override;

private:
	// Managers
	GameClock& glMng = GameClock::Get();
	Audio& audioMng = Audio::Get();

	// Const
	const sf::Vector2f offsetBetween = sf::Vector2f(180.f, 0.5f);
	static constexpr float TIME_BEGIN = 2.f;
	static constexpr float TIME_MIN_BEFORE_ATK = 1.f;
	static constexpr float TIME_MAX_BEFORE_ATK = 3.f;
	static constexpr float TIME_ATTACK = 0.5f;
	static constexpr float TIME_END = 2.f;
	static constexpr float SHAKE_OFFET = 2.f;
	static constexpr float SHAKE_TIME = 0.05f;
	static constexpr int PAUSE_FRAME = 5;

	// Private functions
	void draw(sf::RenderTarget& _target, sf::RenderStates _states) const override;

	// Update functions
	void UpdateBegin(FightingState* _fightingState);
	void UpdateBeforeAttack(FightingState* _fightingState);
	void UpdateAttack(FightingState* _fightingState);
	void UpdateEnd(FightingState* _fightingState);

	// Next state
	void NextToBeforeAttack(void);
	void NextToAttack(void);
	void NextToEnd(void);
	void End(void);

	// Members
	typedef void (KnightMiniGame::*UpdateState)(FightingState* _fightingState);
	UpdateState curUpdate;

	sf::Vector2f InitEnemyScale;

	float shakeTime;
	float timeBeforeAtk;
	bool playerTakeDamage;
	bool playerGiveDamage = false;
	sf::Vector2f enemyCounteredPos;
};

#endif // ! KNIGHT_MINI_GAME__HPP