#ifndef FRANKENCHEF_MINI_GAME__HPP
#define FRANKENCHEF_MINI_GAME__HPP

#include "Fighting/FightingMiniGames.hpp"
#include "Player.hpp"
#include "Enemy.hpp"

class FrankenChefFight : public MiniGame
{
public:
	constexpr static float CHEF_THROW_TIMER_MIN = 0.75f;
	constexpr static float CHEF_THROW_TIMER_MAX = 1.5f;

	constexpr static int TURN_BEFORE_SPECIAL_ATK = 2;

	enum class State
	{
		THROW,
		FLYING_KNIFE,
		SPECIAL_ATK,
		SPECIAL_FALLING
	};

	enum Objects
	{
		KNIFE,
		ROLL,
		PAN,
		SAUCE_PAN,

		OBJECTS_SIZE
	};

	FrankenChefFight();
	~FrankenChefFight() override;

	void UpdateThrow(FightingState* _fightingState);
	void UpdateFlyingKnife(FightingState* _fightingState);
	void UpdateSpeAtk(FightingState* _fightingState);
	void UpdateSpeFall(FightingState* _fightingState);

	void InitMiniGame(std::shared_ptr<Player>& _player, std::shared_ptr<Enemy>& _enemy, FightingState* _fightingState) override;
	void UpdateMiniGame(FightingState* _fightingState) override;
	void PollEventMiniGame(sf::Event& _event, FightingState* _fightingState) override;

	void InitFlyingKnife(void);
	void InitSpecialAtk(void);
	void InitSpecialFlying(void);

	void Finish(void);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void InitSounds(void);
	void DeleteSounds(void);

	State enemyState = State::THROW;

	sf::Vector2f vectorKnife;
	sf::Vector2f target;

	Objects fallingObject;

	float throwTimer = 0.f;

	sf::Sprite chefObject[OBJECTS_SIZE];
	float speed = 0.f;

	int turnBeforeSpecialAtk = TURN_BEFORE_SPECIAL_ATK;
};

#endif // !FRANKENCHEF_MINI_GAME__HPP