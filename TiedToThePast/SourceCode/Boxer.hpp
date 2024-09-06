#ifndef BASE_TIE__HPP
#define BASE_TIE__HPP

#include "Tie.hpp"

class Boxer : public Tie
{
public:
	Boxer();

	void InitMiniGame() override;
	void UpdateMiniGame() override;
	void DisplayMiniGame(GameData& _gameData) override;
	void PollEventMiniGame(GameData& _gameData) override;

	~Boxer() = default;

private:

};

#endif // !BASE_TIE__HPP
