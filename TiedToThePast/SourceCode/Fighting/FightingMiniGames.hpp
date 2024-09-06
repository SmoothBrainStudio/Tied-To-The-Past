#ifndef MINIGAMES__HPP
#define MINIGAMES__HPP

#include "../Common.hpp"
#include "../Maths.hpp"
#include "../Easings.hpp"
#include "../Entity.hpp"
#include "TextDamage.hpp"

#define ATTACK_OFFSET_TO_ENEMY sf::Vector2f(144.f , -5.f)
static constexpr float ENEMY_ANIMATION_ATTACK_TIME = 0.5f;
static constexpr float TEXT_RESULT_TIME = 2.f;
static constexpr float START_TIME = 2.5f;
static constexpr float END_TIME = 2.5f;
static constexpr float VOID_SPEED = 2.f;

struct InGameDatas;
class Player;
class Enemy;
class FightingState;
enum class PlayerState;

class MiniGame : public sf::Drawable, public sf::Transformable
{
public:
	constexpr static sf::Uint8 PLAYER_DODGE_TRANSPARENCY = 100;
	constexpr static float PLAYER_DODGE_DURATION = 1.f;
	constexpr static float PLAYER_DODGE_OFFSET = 160.f;

	constexpr static int PLAYER_CLONE_SIZE = 5;
	constexpr static float PLAYER_COOLDOWN_DODGE = 0.75f;
	constexpr static float PLAYER_COOLDOWN_COUNTER = 0.75f;

	static constexpr float SHAKE_TIME = 0.05f;

	enum class ResultAttack
	{
		MISS,
		OK,
		GREAT,
		EXCELENT,
		PERFECT
	};

	enum class ResultDodge
	{
		ATTACKED,
		BLOCKED,
		DODGE
	};

	MiniGame(void);
	virtual ~MiniGame(void);

	void Init(std::shared_ptr<Player>& _player, std::shared_ptr<Enemy>& _enemy, FightingState* _fightingState);
	void Update(FightingState* _fightingState);
	void PollEvent(sf::Event& _event, FightingState* _fightingState);
	bool IsOver(void);
	void FinishMiniGame(void);

	void EventPlayerDefense(sf::Event& _event);
	void UpdatePlayerDefense(void);
	void SetPlayerInteractState(const PlayerState& _playerState);

	void CallResult(const ResultAttack& _result);
	void CallResult(const ResultDodge& _result);

	const sf::Vector2f GetShakingPos(const sf::Vector2f& _currentPos, const sf::Vector2f& _center, const float& _offset = 2.f);

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

	virtual void InitMiniGame(std::shared_ptr<Player>& _player, std::shared_ptr<Enemy>& _enemy, FightingState* _fightingState) = 0;
	virtual void UpdateMiniGame(FightingState* _fightingState) = 0;
	virtual void PollEventMiniGame(sf::Event& _event, FightingState* _fightingState) = 0;
	void SetActiveDef(const bool& _bool = true);

	bool isInit = false;
	bool activeWarning = false;
	sf::Sprite warning;

	float totalTime = 0.f;
	bool over = false;
	bool playerActiveDef = false;

	sf::Sprite playerClone[PLAYER_CLONE_SIZE];
	float playerCloneActionTime[PLAYER_CLONE_SIZE] = { 0.f };
	float playerActionTime = 0.f;
	float playerCoolDown = 0.f;

	std::shared_ptr<Player> player = nullptr;
	std::shared_ptr<Enemy> enemy = nullptr;
	sf::Vector2f playerInitPos;
	sf::Vector2f enemyInitPos;

	float textResultTime = FLT_MAX;
	float shaderTotalTime = 0;
	float voidTime = 0;
	bool counteredOneTime = false;
	sf::Sprite textResult;
};

#endif // !MINIGAMES__HPP