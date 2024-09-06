#ifndef BREAK_ATTACK__HPP
#define BREAK_ATTACK__HPP

#include "Status.hpp"

class BreakAttack : public Status
{
public:
	BreakAttack();
	~BreakAttack() override = default;

	void Init(Entity*, std::shared_ptr<Entity>& _target, const int& _nbTurn) override;
	BreakAttack* Clone() const override;
	void Run() override;
	void Stop() override;

private:
	static constexpr float ATK_BREAK_COEF = 0.34f;
};

#endif // !BREAK_ATTACK__HPP


