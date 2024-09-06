#ifndef ATTACK_BUFF__HPP
#define ATTACK_BUFF__HPP

#include "Status.hpp"

class AttackBuff : public Status
{
public:
	AttackBuff();
	~AttackBuff() override = default;

	void Init(Entity*, std::shared_ptr<Entity>& _target, const int& _nbTurn) override;
	AttackBuff* Clone() const override;
	void Run() override;
	void Stop() override;

private:
	static constexpr float ATK_BUFF_COEF = 0.34f;
};

#endif // !ATTACK_BUFF__HPP



