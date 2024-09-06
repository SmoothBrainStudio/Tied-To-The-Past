#ifndef IMMUNITY__HPP
#define IMMUNITY__HPP

#include "Status.hpp"

class Immunity : public Status
{
public:
	Immunity();
	~Immunity() override = default;

	void Init(Entity*, std::shared_ptr<Entity>& _target, const int& _nbTurn) override;
	Immunity* Clone() const override;
	void Run() override;
	void Stop() override;
};

#endif // !IMMUNITY__HPP



