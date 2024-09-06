#ifndef BLEEDING__HPP
#define BLEEDING__HPP
#include "Status.hpp"
static constexpr int BLEEDING_DAMAGE = 7;
class Bleeding : public Status
{
public:
	Bleeding();
	~Bleeding() override = default;

	void Init(Entity* _attacker, std::shared_ptr<Entity>& _target, const int& _nbTurn) override;
	Bleeding* Clone() const override;
	void Run() override;
	void Stop() override;
};

#endif // !BURNING__HPP

