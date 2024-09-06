#ifndef BOSS_CANDLE_MINI_GAME
#define BOSS_CANDLE_MINI_GAME

#include "FightingMiniGames.hpp"
#include "../Player.hpp"
#include "../Enemy.hpp"
#include "../FightingState.hpp"

class BossCandleMiniGame : public MiniGame
{
public:
	constexpr static short TURN_BEFORE_SPECIAL_ATK = 3;

	constexpr static float FIRE_BALL_SPEED = 800.f;

	BossCandleMiniGame();
	~BossCandleMiniGame() override;

	void InitMiniGame(std::shared_ptr<Player>& _player, std::shared_ptr<Enemy>& _enemy, FightingState* _fightingState) override;
	void UpdateMiniGame(FightingState* _fightingState) override;
	void PollEventMiniGame(sf::Event& _event, FightingState* _fightingState) override;

	void Finish(void);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void InitSounds(void);
	void DeleteSounds(void);
	void PlayFireballFX(void);

	enum State
	{
		ATK_BASIQUE,
		ATK_BASIQUE_THROW,
		ATK_SPECIAL
	};

	void UpdateAtkBasic(FightingState* _fightingState);
	void UpdateAtkBasicThrow(FightingState* _fightingState);
	void UpdateAtkSpecial(FightingState* _fightingState);

	short turnBeforAtkSpc = TURN_BEFORE_SPECIAL_ATK;

	State enemyState = State::ATK_BASIQUE;

	sf::Vector2f vectorFireBall;
	sf::Vector2f target;
	AnimatedSprite fireballAnim;
	float timerThrow = 0.f;
};

#endif // !BOSS_CANDLE_MINI_GAME