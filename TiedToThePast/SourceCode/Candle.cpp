#include "Definitions.hpp"
#include "Candle.hpp"

Candle::Candle()
{
	this->SetName("PlayerBougie");
}

void Candle::InitMiniGame()
{
	this->compteurSpam = 0;
	this->barSpam.setSize({ 300,50 });
	this->barSpam.setPosition(Screen::Get().GetWidth() / 2, Screen::Get().GetHeight() / 2);
}

void Candle::UpdateMiniGame()
{
}

void Candle::PollEventMiniGame(GameData& _gameData)
{
	if (_gameData.gameEvent.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		compteurSpam++;
	}
}

void Candle::DisplayMiniGame(GameData& _gameData)
{
	_gameData.window.clear();
	_gameData.window.draw(barSpam);

}
