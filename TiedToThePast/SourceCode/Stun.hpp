#ifndef STUN__HPP
#include "Status.hpp"
class Stun : public Status
{
public:
	Stun();
	~Stun() override = default;

	void Init(Entity* _attacker, std::shared_ptr<Entity>& _target, const int& _nbTurn = 1) override;
	Stun* Clone() const override;
	void Run() override;
	void Stop() override;
};
#endif // !STUN__HPP



