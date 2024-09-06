#ifndef CANDLE_MAP__HPP
#define CANDLE_MAP__HPP

#include "Common.hpp"
#include "Player.hpp"
#include "Camera.hpp"
#include "ItemOnGround.hpp"

static constexpr float SECONDS_PER_CANDLES = 1.f;
static constexpr float LENGTH_TO_INTERACT_WITH_CANDLE_RIDDLE = 150;
static constexpr float LENGTH_TO_ACTIVATE_CANDLE = 150;
class CandleRiddle
{
public:
	CandleRiddle() = default;
	CandleRiddle(const std::vector<Json::Value>& _candles, const CameraBorder& _cameraBorder);

	void PollEvent(Player& _player, sf::Event& _event);

	void Update(Player& _player, Camera& _camera);
	void UpdatePlayerLight(Player& _player);

	// Simon type
	void SimonOffStateUpdate(Player& _player);
	void SimonOffStatePollEvent(const Player& _player, sf::Event& _event);
	void SimonSetUpUpdate();
	void SimonEndSetUpUpdate();

	
	void SimonOnGoingPollEvent(Player& _player, sf::Event& _event);
	void SimonOnGoingUpdate(Player& _player);
	void SimonResolveUpdate();
	void SimonRestart();

	// Pas Simon type
	void DavidOnGoingPollEvent(Player& _player, sf::Event& _event);
	void DavidOnGoingUpdate(Player& _player);
	void DavidResolveUpdate();

	sf::RenderTexture& GetRenderTexture();
	// For all type

	
	~CandleRiddle();

private:
	void ChangeLight(const int& _indexCandle);
	enum class CANDLE_RIDDLE_TYPE
	{
		SIMON,
		DAVID
	};

	float playerLightSize = 1.75f;

	enum class CANDLE_RIDDLE_STATE
	{
		OFF,
		SET_UP,
		END_SET_UP,
		ON_GOING,
		RESOLVE,
		END
	};

	bool wasCollide = false;

	// Important variables
	CANDLE_RIDDLE_TYPE type;
	CANDLE_RIDDLE_STATE State;
	std::vector<sf::Vector2f> candlesPos;
	std::vector<AnimatedSprite*> candlesAnimation;
	std::vector<bool> areCandlesActivated;
	TexturesManager& texMng = TexturesManager::Get();
	std::string uniqueID;
	bool isRestarting = false;

	// shader variables
	std::vector<sf::Sprite*> lightSprites;
	sf::RenderTexture lightBackground;
	sf::RectangleShape background;
	sf::Shader lightShader;

	// Off variables
	bool isPlayerInteract = false;


	// SetUp variables
	bool isSetUp = false;
	Timer SetUpRiddleTimer;
	float percentPerCandle;
	int indexSetUpRiddle;

	// EndSetUp variables
	Timer EndSetUpTimer;

	// OnGoing variables
	std::vector<int> activationOrder;

	// Resolve variables
	Timer resolvingTimer;
};

#endif // !CANDLE_MAP__HPP