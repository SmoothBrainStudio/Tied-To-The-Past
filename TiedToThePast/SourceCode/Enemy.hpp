#ifndef ENEMY__HPP
#define ENEMY__HPP

#include "JsonManager.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include "AnimatedSprite.hpp"
#include "Item.hpp"

enum StatesExploration
{
	PATROL,
	ATTACK
};

struct InGameDatas;

class Enemy : public Entity
{
public:
	static constexpr float DEATH_ANIM_TIME = 2.f;

	static std::unordered_map<std::string, std::unique_ptr<Enemy>> prefabs;

	static void LoadPrefabs(const std::vector<std::string>& _filePaths);
	static void UnloadPrefabs(void);

	enum DirectionEnemyHorizontal
	{
		LEFT,
		RIGHT
	};
	enum DirectionEnemyVertical
	{
		UP,
		DOWN
	};

	enum AnimationState
	{
		IDLE,
		MOVE,
		ATTACKING,
		DAMAGE,
		SPECIAL,

		NB_ANIMATIONS
	};

	
	struct FightingData
	{
		std::vector<std::shared_ptr<Enemy>> allies;
		MiniGame* miniGame = nullptr;
	};

	struct ItemLoot
	{
		
		std::shared_ptr<Item> item;
		float probability;
		int nbMax;
	};
	// Constructor for map loading
	Enemy() = default;
	Enemy(const Json::Value& _mapEnemyJson);
	Enemy(const Enemy& _enemy);
	~Enemy() override;

	void LoadSpecificData(void);
	void LoadMapData(const Json::Value& _mapData);
	void LoadCopy(const Enemy& _enemy);

	void Update(InGameDatas& _inGameDatas);

	// Return datas needed in fightings
	FightingData& GetFightingData();
	const std::string& GetName(void) const;
	const float& GetPatrolRadius(void) const;
	const float& GetTimerWhileAttackingMax(void) const;
	const float& GetTimerBeforeAttackingMax(void) const;
	const bool& GetIsInFight(void) const;
	const sf::Vector2f& GetLastPosExplo(void) const;
	const std::vector<std::shared_ptr<Enemy>>& GetAllies(void) const;
	MiniGame* GetMiniGame(void);
	void ClearAllies(void);
	const AttackType& GetAttackType(void) const;
	AnimatedSprite* GetCurAnim(void);
	const AnimationState& GetAnimationState(void) const;


	// change and reset patrol state
	void SetStateExploration(const StatesExploration _newState);
	void SetName(const std::string& _name);
	void SetPatrolRadius(const float& _patrolRadius);
	void SetTimerWhileAttackingMax(const float& _timerAttack);
	void SetTimerBeforeAttackMax(const float& _timerBeforeAttack);

	void AddAlly(std::shared_ptr<Enemy>& _ally);
	void SetIsInFight(const bool& _bool);
	void SetLastPosExplo(const sf::Vector2f& _pos);
	void SetAnimationState(const AnimationState& _state);
	void SetReadyToFight(void);
	void TakeDamage(Player& _player);

	void ResetTimeBeforeAttack();
	bool CanAttack();
	void UpdateShadow();
	void UpdateAnimations(void);

	const int& GetSweetsDrop();
	const std::vector<std::shared_ptr<ItemLoot>>& GetLootableItems();

	// Save uniqueID in boolMng (if there is one) to avoid spawning the same enemy twice
	void SaveDeath(void);

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void LoadEnemyData(const std::string& _path);

	StatesExploration stateExploration = PATROL;
	StatesExploration lastStateExploration = PATROL;
	DirectionEnemyHorizontal directionHorizontal = DirectionEnemyHorizontal::LEFT;
	DirectionEnemyVertical directionVertical = DirectionEnemyVertical::UP;
	sf::Vector2f posBasePatrol;
	sf::Vector2f vectorForward;

#pragma region JSON DATA

	// Data
	std::string name;
	float initialSpeed = 0.f;
	float speedMultiplier = 0.f;
	AttackType attackType;

	// Patrol
	float scope = 0.f;
	float patrolRadius = 0.f;
	float angle = 0.f;
	float rotateAngle = 0.f;

	float timerWhileAttackingMax = 0.f;
	float timerBeforeAttackMax = 0.f;
	float timerWhileAttacking = 0.f;
	float timerBeforeAttack = 0.f;

#pragma endregion

	FightingData fightingData;
	float deathTimer = 0.f;
	bool isInFight = false;
	AnimationState animState = AnimationState::IDLE;

	sf::Vector2f lastPosExplo;
	std::string walkSoundName;
	std::string walkSpeedSoundName;
	
	std::string unlockBool;
	std::string uniqueID; // id to save in boolMng when dead

	int sweetsLoot = 10;
	std::vector<std::shared_ptr<ItemLoot>> itemsLootable = {};

	void MoveKnight(const float& _dt, std::shared_ptr<Player>& _player, const std::vector<sf::FloatRect>& _walls);
	void MoveFireBall(const float& _dt, std::shared_ptr<Player>& _player, const std::vector<sf::FloatRect>& _walls);
	void MoveZombie(const float& _dt, std::shared_ptr<Player>& _player, const std::vector<sf::FloatRect>& _walls);
	void MoveCandleBoss(const float& _dt, std::shared_ptr<Player>& _player, const std::vector<sf::FloatRect>& _walls);
	void MoveFrankenchief(const float& _dt, std::shared_ptr<Player>& _player, const std::vector<sf::FloatRect>& _walls);

	std::function<void(const float& _dt, std::shared_ptr<Player>& _player, const std::vector<sf::FloatRect>& _walls)> moveFunc;

#pragma region Patrols and walls collision

	// Loop movement from left to right and right to left
	void RunHorizontalPatrol(float _dt, sf::Vector2f& _posEnemy);

	// Manage collision with walls for horizontal patrol
	void ResolveCollisionHorizontal(const std::vector<sf::FloatRect>& _walls, const float& _dt, const sf::Vector2f& _playerPos);

	// Loop circular movement
	void RunCircularPatrol(float _dt, sf::Vector2f& _posEnemy, const float& _rotationSpeed, float& _rotateAngle);

#pragma endregion Patrols and walls collision

	/// <summary>
	/// Calculate length with player to active attack 
	/// </summary>
	/// <param name="_player"> : player pointer</param>
	/// <param name="attackScope"> : attack distance</param>
	/// <param name="speedFactor"> : speed multiplier for attack</param>
	/// <param name="_hasVisionCone"> : is enemy has a vision cone. false by default</param>
	void CheckIfAttackIsPossible(std::shared_ptr<Player>& _player, const float& _attackScope, const float& _speedFactor, const bool& _hasVisionCone = false);

	// Return to horizontal patrol
	void ReturnToHorizontalPatrol(void);

	// Return to circular patrol
	void ReturnToCircularPatrol(sf::Vector2f& _posEnemy, float& _rotateAngle);

	// Turn direction, sprite and forward vector on the left
	void TurnLeft(void);

	// Turn direction, sprite and forward vector on the right
	void TurnRight(void);

	// Return false if point is on the left
	bool IsPointOnTheRight(sf::Vector2f& _posEnemy, const sf::Vector2f& _point);

	// Return false if point is upper than enemy
	bool IsPointIsDowner(sf::Vector2f& _posEnemy, const sf::Vector2f& _point);

	// Change state from patrol to attack, speed according to _speedFactor and reset timer for attack
	void LaunchAttack(const float& _speedFactor);

	// Move to the point given in parameters
	void GoToPoint(const float& _dt, sf::Vector2f& _posEnemy, sf::Vector2f& _point);

	// Come back to the initial position
	bool BackToInitPosition(const float& _dt, sf::Vector2f& _enemyPos);

	// Guide Enemy to the left or to the right when in collision with an obstacle
	void AvoidObstacleInHorizontalWay(float _dt, sf::Vector2f& _enemyPos, const sf::Vector2f& _destination);

	// Guide Enemy to the top or to the bottom when in collision with an obstacle
	void AvoidObstacleInVerticalWay(float _dt, sf::Vector2f& _enemyPos, const sf::Vector2f& _destination);

	// Change buffer and play death sound of the enemy
	void PlayDeathSound(const std::string& _bufferName, const float& _soundVolume = 100.f);
};

#endif // !ENEMY__HPP