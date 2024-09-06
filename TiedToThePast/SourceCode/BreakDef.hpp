#ifndef BREAK_DEF__HPP
#define BREAK_DEF__HPP
#include "Status.hpp"

class BreakDef :public Status
{
public:
	BreakDef();
	~BreakDef() override = default;

	void Init(Entity*, std::shared_ptr<Entity>& _target, const int& _nbTurn) override;
	BreakDef* Clone() const override;
	void Run() override;
	void Stop() override;

	static constexpr float DEF_BREAK_COEF = 0.33f;
private:
};
#endif // !BREAK_DEF__HPP



