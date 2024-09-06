#ifndef CANDLE_TIE__HPP
#define CANDLE_TIE__HPP

#include "Tie.hpp"

class Candle : public Tie
{
public:
	Candle();

	void InitMiniGame() override;
	void UpdateMiniGame() override;
	void PollEventMiniGame(GameData& _gameData) override;
	void DisplayMiniGame(GameData& _gameData) override;

	~Candle() = default;

private:
	sf::RectangleShape barSpam;
	int compteurSpam = 0;
	
};

#endif //!CANDLE_TIE_HPP
