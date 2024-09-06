#ifndef LILBOO__HPP
#define LILBOO__HPP

#include "NPC.hpp"
#include "DialogMap.hpp"

class Lilboo : public NPC
{
public:
	static constexpr float LENGHT_TALKING = 175.f;

	Lilboo(const Json::Value& _mapNpcJson);
	~Lilboo(void) = default;

	void Update(Player& _player);
	void MoveTowardsPlayer(Player& _player);

	void SetDialog(DialogMap& _dialog, Player& _player);
	const bool IsVisible(void);
private:
	std::string boolNeeded;
	bool isFollowing = false;
	DialogMap* dialogMap = nullptr;
};

#endif // !LILBOO__HPP