#ifndef FIGHTING_BOXES
#define FIGHTING_BOXES

#include "Player.hpp"

class FightingBoxes
{
public:
	enum ActionType
	{
		ATTACK,
		TIES,
		RUN,
		ITEMS,

		BOX_TYPE_SIZE
	};

	FightingBoxes(void);
	~FightingBoxes(void);

	void Init(void);

	void Event(sf::Event& _event, const size_t& _boxesSize);

	void Update(Player& _player, const size_t& _boxesSize);
	void UpdateActionTexture(Player& _player, const bool& _canRun);
	void UpdateItemTexture(Player& _player);
	void UpdateTieTexture(Player& _player);
	void UpdatePartitionTexture(Player& _player);

	void SetSelectedBox(const int& _var);
	const size_t& GetSelectedBox(void) const;
	void SetVisible(const bool& _visible = true);

private:
	static constexpr int TEMPLATE_BOX_SIZE = 100;
	static constexpr float BOX_Y_POSITION = 300.f;

	size_t boxSelected = 0;
	sf::RectangleShape boxes[BOX_TYPE_SIZE];
};
#endif // !FIGHTING_BOXES