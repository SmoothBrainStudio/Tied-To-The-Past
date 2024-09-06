#ifndef FIGHTING_HUD__HPP
#define FIGHTING_HUD__HPP

#include "Player.hpp"
#include "Enemy.hpp"

class FightingHUD
{
public:
	FightingHUD(void);
	~FightingHUD(void);

	void InitBackground(void);

	void InitLifeHUD(void);

	void Update(Player& _player, const float& _timerEncounter);
	void UpdateLifeHUD(Player& _player);

private:
	struct Life
	{
		float time;

		sf::Sprite bar;
		sf::Sprite heart;

		sf::Text point;
	};
	
	sf::Sprite background;

	Life lifeHUD;
};

#endif // !FIGHTING_HUD__HPP