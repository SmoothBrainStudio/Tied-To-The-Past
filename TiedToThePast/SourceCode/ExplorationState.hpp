#ifndef EXPLORATION_STATE__HPP
#define EXPLORATION_STATE__HPP

#include "PhaseState.hpp"

class ExplorationState : public PhaseState
{
public:
	ExplorationState(GameData& _gameData, InGameDatas& _inGameDatas) 
	{ 
		Init(_gameData, _inGameDatas); 
	}
	~ExplorationState(void);

	void Init(GameData& _gameData, InGameDatas& _inGameDatas) override;
	void PollEvent(GameData& _gameData, InGameDatas& _inGameDatas) override;
	void Update(GameData& _gameData, InGameDatas& _inGameDatas) override;
	void Display(InGameDatas& _inGameDatas, sf::RenderWindow& _window) override;

	void AddItemOnGround(InGameDatas& _inGameDatas, const sf::Vector2f& _position, const std::string& _itemId);

private:
	Audio& audMng = Audio::Get();
	Screen& scrMng = Screen::Get();
	DrawManager& drwMng = DrawManager::Get();
	EventManager& evtMng = EventManager::Get();
	TexturesManager& texMng = TexturesManager::Get();

	void InitSounds(void);

	void UpdateCrates(InGameDatas& _inGameDatas);
	void UpdateDroppedItems(InGameDatas& _inGameDatas);
	void DisplayShader(InGameDatas& _inGameDatas, sf::RenderWindow& _window);
};

#endif // !EXPLORATION_STATE__HPP