#ifndef PHASE_STATE__HPP
#define PHASE_STATE__HPP

#include "Common.hpp"
#include "InGameDatas.hpp"

class PhaseState
{
public:
	PhaseState() = default;
	~PhaseState() = default;

	virtual void Init(GameData& _gameData, InGameDatas& _inGameDatas) = 0;
	virtual void PollEvent(GameData& _gameData, InGameDatas& _inGameDatas) = 0;
	virtual void Update(GameData& _gameData, InGameDatas& _inGameDatas) = 0;
	virtual void Display(InGameDatas& _inGameDatas , sf::RenderWindow& _window) = 0;

	bool isDelete = false;
};

#endif // !PHASE_STATE__HPP