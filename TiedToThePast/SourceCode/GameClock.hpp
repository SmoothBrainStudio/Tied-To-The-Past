#ifndef GAME_CLOCK__HPP
#define GAME_CLOCK__HPP

#include "Definitions.hpp"

class GameClock
{
public:
	~GameClock(void) = default;

	GameClock(const GameClock&) = delete;
	GameClock(GameClock&&) = delete;
	GameClock& operator=(const GameClock&) = delete;
	GameClock& operator=(GameClock&&) = delete;

	static GameClock& Get(void);

	void Update(void);
	void UpdatePlayTime(void);

	void SetPlayTime(float _time);
	void SetMultiplicator(const float& _mult);

	const float& GetTotalTime(void) const;
	const float& GetDeltaTime(void) const;
	const int& GetFPS(void) const;
	const float& GetPlayTime(void) const;
	const float& GetMultiplicator(void);
	
private:
	GameClock(void) = default;

	sf::Clock clock = sf::Clock();

	int framePerSecond = 0;		// :sunglasses: masterpcrace
	float previousTime = 0.f;	// The time taken by the last cycle to finished
	float currentTime = 0.f;	// Time the program has been turning
	float deltaTime = 0.f;		// dt : Make the game logic the same speed no matter what the framerate
	float playTime = 0.f;
	float multiplicator = 1.f;
};

#endif //!GAME_CLOCK__HPP