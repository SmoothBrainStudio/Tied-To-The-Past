#ifndef POISON__HPP
#define POISON__HPP

#include "Status.hpp"

static constexpr float POISON_COEF = 0.03f;

class Poison : public Status
{
public:
	Poison();
	~Poison() override = default;

	void Init(Entity* _attacker, std::shared_ptr<Entity>& _target, const int& _nbTurn) override;
	Poison* Clone() const override;
	void Run() override;
	void Stop() override;
};

#endif

