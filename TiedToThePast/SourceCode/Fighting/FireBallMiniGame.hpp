#ifndef FIREBALL_MG__HPP
#define FIREBALL_MG__HPP

#include "FightingMiniGames.hpp"

class FireBallMiniGame : public MiniGame
{
public:
	FireBallMiniGame(void);
	~FireBallMiniGame(void) override = default;

	void InitMiniGame(std::shared_ptr<Player>& _player, std::shared_ptr<Enemy>& _enemy, FightingState* _fightingState) override;
	void UpdateMiniGame(FightingState* _fightingState) override;
	void PollEventMiniGame(sf::Event& _event, FightingState* _fightingState) override;

private:
	// Managers
	Audio& audioMng = Audio::Get();
	Screen& scrMng = Screen::Get();
	GameClock& glMng = GameClock::Get();
	TexturesManager& texMng = TexturesManager::Get();

	// Constantes
	const sf::Vector2f offsetBetween = sf::Vector2f(800.f, 0.5f);
	static constexpr float TIME_START = 1.25f;
	static constexpr float TIME_MIN_BEFORE_ATK = 1.f;
	static constexpr float TIME_MAX_BEFORE_ATK = 3.f;
	static constexpr float TIME_MIN_ATTACK = 0.75f;
	static constexpr float TIME_MAX_ATTACK = 1.5f;
	static constexpr float TIME_RETURN_START = 1.25f;
	static constexpr float SHAKE_OFFET = 2.f;
	

	// Private functions
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	// Update functions
	void UpdateBegin(FightingState* _fightingState);
	void UpdateBeforeAttack(FightingState* _fightingState);
	void UpdateAttack(FightingState* _fightingState);
	void UpdateReturnStart(FightingState* _fightingState);
	void EndDead(FightingState* _fightingState);

	// Next functions
	void NextToBeforeAttack(void);
	void NextToAttack(void);
	void NextToReturnStart(void);
	void End(void);

	// Members
	typedef void (FireBallMiniGame::* StateFunc)(FightingState* _fightingState);
	StateFunc state;
	float timeBeforeAtk;
	float timeAtk;
	float shakeTime;
};
#endif // !FIREBALL_MG__HPP
