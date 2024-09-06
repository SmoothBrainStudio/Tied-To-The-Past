#ifndef BIG_BOO__HPP
#define BIG_BOO__HPP

#include "FightingMiniGames.hpp"
#include "../Player.hpp"
#include "../Enemy.hpp"
#include "../FightingState.hpp"

class BigBooMiniGame : public MiniGame
{
public:
	const sf::Vector2f offsetBetween = sf::Vector2f(350.f, 155.f);
	const sf::Vector2f offsetSpecial = sf::Vector2f(410.f, 545.f);
	constexpr static float BIG_BOO_ATACK_TIMER_MIN = 1.f;
	constexpr static float BIG_BOO_ATACK_TIMER_MAX = 1.75f;
	static constexpr float TIME_MOVE = 1.75f;
	static constexpr float TIME_END = 2.f;
	static constexpr float TIME_RETURN_START = 2.5f;
	static constexpr float TIME_COUNTER_MOVE = 2.85f;
	constexpr static int TURN_BEFORE_SPECIAL_ATK = 2;
	static constexpr float SHAKE_TIME = 0.05f;
	static constexpr float ROTATE_SPEED = 500.f;
	static constexpr float OUT_SCREEN_OFFSET = 350.f;

	enum class State
	{
		NORMAL_MOVE,
		SPECIAL_MOVE,
		NORMAL_ATK,
		SPECIAL_ATK,
		NORMAL_SHAKE,
		SPECIAL_SHAKE,
		NORMAL_REPLACE,
		SPECIAL_REPLACE,
		COUNTER_REPLACE
	};

	BigBooMiniGame();
	~BigBooMiniGame() override;

	void InitMiniGame(std::shared_ptr<Player>& _player, std::shared_ptr<Enemy>& _enemy, FightingState* _fightingState) override;
	void UpdateMiniGame(FightingState* _fightingState) override;
	void PollEventMiniGame(sf::Event& _event, FightingState* _fightingState) override;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	Audio& audioMng = Audio::Get();
	void InitSounds(void);
	void DeleteSounds(void);

	void InitNormalAttack(void);
	void InitSpecialAttack(void);

	// Move next to player
	void UpdateNormalMove(const float& _dt);
	// Inflict damage with a punch
	void UpdateNormalAttack(const float& _dt);
	// Take player as a tagret to inflict damage
	void UpdateSpecialMove(const float& _dt);
	// Prepare special attack
	void UpdateSpecialAttack(const float& _dt);
	// Shake before normal attack
	void UpdateNormalShake(const float& _dt);
	// Shake before special attack
	void UpdateSpecialShake(const float& _dt);
	// Replace Big boo after normal attack
	void UpdateNormalReplace(const float& _dt);
	// Replace Big boo after special attack
	void UpdateSpecialReplace(const float& _dt);
	// Reset values of rotation and origin to initial values
	void ResetInitOrigin(void);
	// Remove Big boo to his initial place after player's counter attack
	void UpdateCounterReplace(const float& _dt);
	// Shake movement
	void Shake(const float& _dt, const sf::Vector2f _refPos);

	void NextToEnd(void);
	void End();

	State bigBooState;
	State lastState;
	sf::Vector2f initEnemyScale;
	sf::Vector2f initEnemyOrigin;
	sf::Vector2f initSpecialPos;
	sf::Vector2f initCounterPos;

	float rotateAngle;

	float shakeTime;
	float attackTimer = 0.f;
	int turnBeforeSpecialAtk = TURN_BEFORE_SPECIAL_ATK;

	sf::Vector2f target;
	sf::Vector2f vectorMove;

	bool playerGiveDamage;
	bool playerTakeDamage;

	void Start(FightingState* _fightingState);

	typedef void (BigBooMiniGame::* StateFunc)(FightingState* _fightingState);
	StateFunc state;
};

#endif // !BIG_BOO__HPP
