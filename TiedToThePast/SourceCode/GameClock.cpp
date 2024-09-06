#include "Definitions.hpp"
#include "GameClock.hpp"

GameClock& GameClock::Get(void)
{
	static GameClock instance;
	return instance;
}

void GameClock::Update(void)
{
	previousTime = currentTime;
	currentTime = clock.getElapsedTime().asSeconds();
	deltaTime = (currentTime - previousTime) * multiplicator;
	framePerSecond = int(1 / deltaTime);
}

const float& GameClock::GetTotalTime(void) const
{
	return currentTime;
}

const float& GameClock::GetDeltaTime(void) const
{
	return deltaTime;
}

const int& GameClock::GetFPS(void) const
{
	return framePerSecond;
}

void GameClock::UpdatePlayTime()
{
	playTime += deltaTime;
}

void GameClock::SetPlayTime(float _time)
{
	playTime = _time;
}

void GameClock::SetMultiplicator(const float& _mult)
{
	multiplicator = _mult;
}

const float& GameClock::GetPlayTime(void) const
{
	return playTime;
}

const float& GameClock::GetMultiplicator(void)
{
	return multiplicator;
}
