#ifndef VAMPIRE_MINI_GAME_HPP
#define VAMPIRE_MINI_GAME_HPP
#include "FightingMiniGames.hpp"

static constexpr std::array<float, 4> multipliers = { 0.8f, 0.9f, 1.f, 1.2f };

class VampireMiniGame : public MiniGame
{
public:
	VampireMiniGame(void);
	~VampireMiniGame(void) override;

	void InitMiniGame(std::shared_ptr<Player>& _player, std::shared_ptr<Enemy>& _enemy, FightingState* _fightingState) override;
	void UpdateMiniGame(FightingState* _fightingState) override;
	void PollEventMiniGame(sf::Event& _event, FightingState* _fightingState) override;

private:

	static constexpr float THROWING_NOTE_DURATION = 2.5f;
	static constexpr float BAT_DESCENDING_DURATION = 0.9f;
	static constexpr float BAT_ATTACK_DURATION = 1.6f;
	static constexpr float OUT_SCREEN_X = 120.f;
	const sf::Vector2f BAT_OFFSET_1 = { 30.f, 30.f };
	const sf::Vector2f BAT_OFFSET_2 = { 50.f, -30.f };

	// Managers
	Audio& audioMng = Audio::Get();

	enum Notes
	{
		SINGLE_NOTE,
		DOUBLE_NOTE,
		TRIPLE_NOTE,
		SOL,
		NB_NOTES
	};

	enum VampireState
	{
		PREPARING_NORMAL_ATK,
		PREPARING_SPECIAL_ATK,
		NORMAL_ATK,
		SPECIAL_ATK,
		END_NORMAL_ATK,
		END_SPECIAL_ATK
	};

	void InitSpecialAtk(void);
	void InitNormalAtk(void);


	// Private functions
	void draw(sf::RenderTarget& _target, sf::RenderStates _states) const override;

	// Update functions
	void UpdatePreparingNormalAttack(FightingState* _fightingState, const float& _dt);
	void UpdatePreparingSpecialAttack(FightingState* _fightingState, const float& _dt);
	void UpdateNormalAttack(FightingState* _fightingState, const float& _dt);
	void UpdateSpecialAttack(FightingState* _fightingState, const float& _dt);
	void UpdateEndNormalAttack(FightingState* _fightingState, const float& _dt);
	void UpdateEndSpecialAttack(FightingState* _fightingState, const float& _dt);
	void InitSound(void);
	void DeleteSound(void);

	// Next state
	void End();

	// Members
	std::array<AnimatedSprite*, 3> bats;
	std::array<AnimatedSprite*, 4> notesArr;
	AnimatedSprite* note = nullptr;

	sf::Vector2f batStartingPos;
	sf::Vector2f batDescendingStopPos;
	bool playerTakeDamage = false;
	int nbTurnsSpecial = 3;
	VampireState state;
	sf::Vector2f noteInitPos;
	float throwingNoteTimer = THROWING_NOTE_DURATION;
	float batDescendingTimer = 0.f;
	int chosenNote = 0;
};
#endif // !VAMPIRE_MINI_GAME_HPP



