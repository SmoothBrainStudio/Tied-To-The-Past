#ifndef BURNING__HPP
#define BURNING__HPP
#include "Status.hpp"
static constexpr float FIRE_COEF = 0.05f;
class Burning : public Status
{
public:
	Burning();
	~Burning() override = default;

	void Init(Entity* _attacker, std::shared_ptr<Entity>& _target, const int& _nbTurn) override;
	Burning* Clone() const override;
	void Run() override;
	void Stop() override;
};

#endif // !BURNING__HPP



