#ifndef STATUS__HPP
#define STATUS__HPP
#include "Entity.hpp"
static constexpr int NB_TURN_DEFAULT = 2;
class Status : public sf::Drawable, public sf::Transformable
{
public:
	Status();
	virtual ~Status();

	struct StatusDisplay
	{
		sf::Sprite icon;
		sf::Text nbTurnsLeft;
		sf::Text nbStacks;
	};

	enum StatusType
	{
		BUFF,
		DEBUFF,
		DOT
	};

	virtual void Init(Entity* _attacker, std::shared_ptr<Entity>& _target, const int& _nbTurn = NB_TURN_DEFAULT) = 0;
	virtual Status* Clone() const = 0;
	virtual void Run() = 0;
	virtual void Stop() = 0;

	void Erase();
	void Update(void);
	void AddNewStack(void);
	void AddTurns(const int& _turn);
	void ResetCooldown(void);

	int nbTurn = -1;
	int currentTurn = -1;
	int nbTurnsLeft = -1;
	int nbStacks = 0;
	int nbStacksMax = 3;
	int id = 0;
	bool stackable = false;

	bool isOver = false;
	std::string effectName;
	std::shared_ptr<Entity> target;
	Entity* attacker = nullptr;
	StatusDisplay statusDisplay;
	bool isAppearing = false;
	bool isGoingBigger = false;
	bool isGoingSmaller = false;
	float scaleTimer = 0.f;

	StatusType type = BUFF;

	static constexpr float MAX_SCALE = 1.5f;
	static constexpr float MIN_SCALE = 1.f;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
#endif // !STATUS__HPP



