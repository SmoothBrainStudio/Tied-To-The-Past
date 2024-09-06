#ifndef ENTITY__HPP
#define ENTITY__HPP

#include "Definitions.hpp"
#include "Audio.hpp"
#include "Fighting/TextDamage.hpp"
#include "Particles/ParticleSystem.hpp"

struct InGameDatas;
class Status;
class Entity : public sf::Transformable, public sf::Drawable
{

protected:
	struct ApplicableEffects
	{
		//~ApplicableEffects()
		//{ 
		//	if (status != nullptr) 
		//	{ 
		//		delete status; 
		//		status = nullptr; 
		//	} 
		//}

		Status* status = nullptr;
		float probability;
	};

public:
	static constexpr float DAMAGE_TEXT_RANDOM_OFFSET = 25.f;

	enum Effects
	{
		BURNING,
		BLEEDING,
		POISON,
		//SI VOUS VOULEZ RAJOUTER UN EFFET DE STATUS, RAJOUTEZ LE AVANT CETTE LIGNE

		ATK_BUFF,
		ATK_BREAK,
		DEF_BUFF,
		DEF_BREAK,
		IMMUNITY,
		ANTI_BUFF,
		STUN,
		NB_EFFECTS
	};

	enum class ParticleTypes
	{
		DAMAGE,
		BLEEDING,
		BURNING,
		POISON,
		NB_PARTICLES
	};
	
	Entity();
	virtual ~Entity();

	void UpdateCollide(void);
	virtual void UpdateAnimations(void) {};

	const std::vector<ApplicableEffects>& GetApplicableEffects(void) const;
	const short int& GetHP(void) const;
	const float GetPercentHPLeft(void) const;
	const unsigned short int& GetHPMax(void) const;
	const sf::Color GetHpColor(void) const;
	sf::Sprite& GetSprite(void);
	sf::FloatRect GetGlobalAABB(void) const;
	sf::FloatRect GetLocalAABB(void) const;
	const sf::FloatRect GetFeetCollisionBox(void) const;
	const float GetBoxRight(void) const;
	const float GetBoxLeft(void) const;
	const bool& IsAlive(void) const;
	const float& GetSpeed(void) const;
	std::deque<std::shared_ptr<TextDamage>>& GetTextDamages();
	const int& GetDamage(void);
	const int& GetInitialDamage(void) const;
	const sf::Vector2u GetShadowSize(void) const;
	const std::array<float, NB_ATTACK_TYPES>& GetResistances(void) const;
	const std::array<bool, NB_EFFECTS>& GetImmunities(void) const;
	void AddResistanceCoef(const float& _coef);
	std::array<std::shared_ptr<ParticleSystem>, (int)Entity::ParticleTypes::NB_PARTICLES>& GetParticles(void);
	const sf::Vector2f& GetShadowOffset(void) const;
	const float& GetShadowRadiusFactor(void) const;

	void AddHP(const short int& _value);
	void SetHP(const short int& _value);
	void AddHPMax(const short int& _value);
	void SetHPMax(const short int& _value);
	void SetAlive(const bool& _alive);
	void SetDamage(const int& _damage);
	void SetSpeed(const float& _speed);
	void AddDamageText(std::shared_ptr<TextDamage>&& _toAdd);
	void SetHitBoxDraw(const bool& _isDraw);
	void AddDamage(const int& _damage);

	void AddStatus(Status* _status);
	const std::vector<Status*>& GetStatutes();
	bool DoesItHaveStatus(const Effects& _effect);
	bool DoesItImmuned(const int& _id);
	void RunStatutes();
	void ClearStatutes();
	void ClearBadStatutes();
	void DeleteStatus(const std::string& _name);
	void ApplyEffect(std::shared_ptr<Entity> _target);
	void ApplyParticularEffect(std::shared_ptr<Entity> _target, const Effects& _effect, const int& _nbTurns = 1);
	void SetImmunity(const bool& _bool);
	void SetAntiBuff(const bool& _bool);
	const sf::Vector2f GetLastPos(void) const;
	const bool& IsStun(void) const;
	void SetStun(const bool& _stun);
	void SetShadowOffset(sf::Vector2f& _offset);
	void SetShadowOffset(const float& _offsetX, const float& _offsetY);
	void SetShadowRadiusFactor(const float& _factor);

	sf::RectangleShape collideBox;
	std::deque<std::shared_ptr<TextDamage>> textDamages;
	int nbIconX = 0;
	int nbIconFloorY = 0;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

protected:
	unsigned short int hpMax = 0;
	short int hp = 0;
	float speed = 0.f;
	bool isAlive = true;
	bool isStun = false;
	int initialDamage = 0;
	int damage = 0;
	//sf::Sprite sprite;
	sf::CircleShape shadow;
	sf::Vector2f shadowOffset = { 0.f, 0.f };
	float shadowRadiusFactor = 0.5f;

	Audio& audio = Audio::Get();

	std::vector<AnimatedSprite> animations;
	AnimatedSprite* curAnim = nullptr;
	

	std::vector<Status*> statutes;
	std::vector<ApplicableEffects> applicableStatutes;
	
	std::array<bool, NB_EFFECTS> immunities;
	std::array<bool, NB_EFFECTS> initialImmunities;
	std::array<float, NB_ATTACK_TYPES> resistances;

	sf::Vector2f lastPos = {};

// Hitbox test
	sf::RectangleShape feetHitbox;
	sf::CircleShape position;

	std::array<std::shared_ptr<ParticleSystem>, (int)ParticleTypes::NB_PARTICLES> particles;
};

#endif // !ENTITY__HPP