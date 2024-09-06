#ifndef DEF_BUFF__HPP
#define DEF_BUFF__HPP

#include "Status.hpp"

class DefBuff : public Status
{
public:
	DefBuff();
	~DefBuff() override = default;

	void Init(Entity*, std::shared_ptr<Entity>& _target, const int& _nbTurn) override;
	DefBuff* Clone() const override;
	void Run() override;
	void Stop() override;

	static constexpr float DEF_BUFF_COEF = 0.5f;
private:

};
#endif // !DEF_BUFF__HPP



