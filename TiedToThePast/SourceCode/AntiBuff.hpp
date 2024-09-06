#ifndef ANTI_BUFF__HPP
#define ANTI_BUFF__HPP

#include "Status.hpp"

class AntiBuff : public Status
{
public:
	AntiBuff();
	~AntiBuff() override = default;

	void Init(Entity*, std::shared_ptr<Entity>& _target, const int& _nbTurn) override;
	AntiBuff* Clone() const override;
	void Run() override;
	void Stop() override;
};


#endif // !ANTI_BUFF__HPP



