#ifndef PLAYER__HPP
#define PLAYER__HPP

#include "Entity.hpp"
#include "ControlsManager.hpp"
#include "GameData.hpp"
#include "Consumable.hpp"
#include "Tie.hpp"
#include "CollisionManager.hpp"
#include "Particles/ParticleSystem.hpp"

#define DAMAGE_TEXT_PLAYER_OFFSET sf::Vector2f(50.f, 50.f)

class Crate;

enum class PlayerState
{
	IDLE,
	MOVE,
	SPEAK,
	ATTACK,
	INTERACT,
	DODGE,
	DEATH,
	COUNTER,
	CHARGING_ATTACK,
	DAMAGE,

	NB_STATES
};

struct InGameDatas;

class Player : public Entity
{
public:
	enum class PhylacteryType
	{
		NORMAL,
		TIE,
		KEY,
		WALL,
		CANDLE,
		LOCKED,
		SAVE
	};
	
	static constexpr int INITIAL_DAMAGE = 15;
	static constexpr float INITIAL_SPEED = 500.f;

#ifdef _DEBUG
	static constexpr int DEBUG_DAMAGE = 150;
	static constexpr float DEBUG_SPEED = 1000.f;
#endif //_DEBUG

	enum class PlayerDir
	{
		DOWN,
		UP
	};

	Player(void);
	~Player(void) override;
	void PollEvent(sf::Event& _event);
	void Update(InGameDatas& _inGameDatas);

	// Return sprite of the dialog box
	sf::Sprite& GetPhylactery(void);

	const PlayerState& GetState(void) const;
	const CostumeState GetCostumeState(void) const;
	const bool& GetIsPunching(void) const;
	const std::vector<std::shared_ptr<Item>>& GetItems() const;
	std::vector<std::string> GetKeyIds() const;
	std::vector<std::shared_ptr<Consumable>> GetConsumables() const;
	std::vector<std::shared_ptr<Item>> GetImportantItems() const;
	std::vector<std::shared_ptr<Tie>> GetTies() const;
	sf::Int16 GetPossessedSweets() const;
	const sf::RectangleShape& GetPunchCollider(void) const;
	const bool GetIsInPunchingFrame(void) const;
	const sf::Vector2f& GetLastPosExplo(void) const;
	const bool& GetIsInFight(void) const;
	AnimatedSprite* GetCurAnim(void);
	const PlayerDir& GetDir(void) const;
	const bool GetXDir(void);
	const std::array<std::array<AnimatedSprite*, (int)PlayerState::NB_STATES>, NB_COSTUMES>& GetAnims(void) const;
	
	void SetPhylacteryType(PhylacteryType _type);
	void SetPossessedSweets(sf::Int16 _amount);
	void SetIsSpeaking(bool _bool);
	void AddItem(const Item& _item, const bool& _isFree = true, bool _show = true);
	void RemoveItem(const std::string& _id);
	void DecreaseItemStack(const std::string& _id);
	void AddTie(const std::shared_ptr<Tie>& _tie, sf::Int16 _price = 0);
	void AddSweets(sf::Int16 _nbSweets);
	void SetCostumeState(const CostumeState& _costumeState);
	void SetState(const PlayerState& _state);

	void SetEnablePhylactery(const bool& _bool, PhylacteryType _type = PhylacteryType::NORMAL);
	void SetLastPosExplo(const sf::Vector2f& _pos);
	void SetIsInFight(const bool& _bool);
	void SetCandleAttackAnimation(const unsigned short& _id);
	void SetBoxerAttackAnimation(const unsigned short& _id);
	void SetCandleIdleAnimation(const unsigned short& _id);
	void SetXDir(const int& _xDir);
	void MovePlayerInFight(const sf::Vector2f& _endPosition, const int& _dir, const float& _speed);

	void SetApplicableStatus(void);
	std::shared_ptr<Tie>& GetCurTie();

	void PlaySoundCounter(void);

	void TakeDamage(short _damageDealt, const AttackType& _attackType);

	// Operator overloading for saving
	friend std::ostream& operator << (std::ostream& _buffer, Player& _player);
	friend std::istream& operator >> (std::istream& _buffer, Player& _player);

	void UpdateAnimations(void) override;

	const bool& CanTp(void);
	void SetTp(const bool& _canTp);

	bool gotSweets = false;
	bool hasNewTie = true;
private:
	constexpr static float PICKED_ITEM_SHOW_TIME = 3.f;

	void draw(sf::RenderTarget& _target, sf::RenderStates _states) const override;

	// Load datas for player in sprite and draw manager
	void InitPlayerSprite(void);

	// Load datas for dialog box in sprite and draw manager
	void InitPhilacterySprite(void);

	void InitPickedItem();

	// Create all animations
	void InitAnimations(void);

	void Move(void);
	// Update dialog box pos
	void UpdatePhylacteryPos(void);

	void UpdateShadow(void);

	void UpdatePickedItem(void);

	// Check boxCollision with screen edge for camera
	void CheckPlayerCollidingWithMapBorders(const CameraBorder& _border);

	void ResolveWallCollision(const std::vector<sf::FloatRect>& walls);
	
	PlayerState state = PlayerState::IDLE;
	PlayerState lastState = PlayerState::IDLE;
	short int costumeState = CostumeState::BASIC;

	//chef d'oeuvre
	std::array<std::array<AnimatedSprite*, (int)PlayerState::NB_STATES>, NB_COSTUMES> animations;
	std::array<AnimatedSprite*, 8> candleAttackAnimations;
	std::array<AnimatedSprite*, 2> boxerAttackAnimations;
	std::array<AnimatedSprite*, 2> candleIdleAnimations;
	//Animation* currentAnimation;

	sf::Vector2f vectorMove = { 0.f, 0.f };
	sf::Vector2f lastVectorMove = { 0.f, 0.f };

	unsigned short int possessedSweets;

	sf::Sprite phylactery;
	sf::Sprite droppedItem;

	sf::Sprite pickedItem;
	float pickedItemTimeRemaining = 0.f;

	std::vector<std::shared_ptr<Item>> items;

	bool isPunching = false;
	float timerResetPunch = 0.f;

	sf::RectangleShape punchCollider;
	sf::CircleShape shadow;

	std::shared_ptr<Tie> curTie;
	bool isInFight = false;
	sf::Vector2f lastPosExplo;

	AnimatedSprite smoke;

	bool canTp = true;
};

#endif // !PLAYER__HPP