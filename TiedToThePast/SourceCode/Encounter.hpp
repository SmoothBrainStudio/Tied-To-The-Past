#ifndef ENCOUNTER__HPP
#define ENCOUNTER__HPP

#include "Player.hpp"
#include "Enemy.hpp"

static const sf::Vector2f END_POS_BG(float(WORKING_WIDTH) / 2.f, float(WORKING_HEIGHT) / 2.f);

class Encounter
{
public:
	Encounter(void);
	~Encounter(void);

	void Init(void);

	void Start(Player& _player, std::vector<std::shared_ptr<Enemy>> _enemies);

	void Event(sf::Event& _event, Player& _player, std::vector<std::shared_ptr<Enemy>> _enemies);

	void Update(Player& _player, std::vector<std::shared_ptr<Enemy>> _enemies);

	const bool IsFinished(void);
	const Timer& GetTimer(void) const;

private:
	static constexpr float TIME_BG_CLASH = 1.2f;
	static constexpr float DELTA_TIME_BG_CLASH = 0.5f;

	void End(Player& _player, std::vector<std::shared_ptr<Enemy>> _enemies);

	Timer timer;

	sf::Sprite playerBg;
	sf::Sprite enemiesBg;

	sf::Vector2f playerStartPos;
	sf::Vector2f enemyStartPos;
};

#endif // !ENCOUNTER__HPP