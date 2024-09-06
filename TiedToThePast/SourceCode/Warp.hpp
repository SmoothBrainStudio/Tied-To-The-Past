#include "Player.hpp"
#include "DialogBox.hpp"

class Warp
{
public:
	enum class Type
	{
		INTERACTION,
		COLLIDE
	};

	Warp(const Json::Value& _obj, const sf::FloatRect& _collideBox);
	Warp(const Warp& _warp);
	~Warp(void);

	void Event(sf::Event& _event, Player& _player);
	void Update(Player& _player);

	void OnEnterWarp(Player& _player);
	void OnExitWarp(Player& _player);
	void OnStayWarp(Player& _player);

	const bool IsColliding(Entity& _entity);
	const bool IsEnter(void);

	Type type;
	sf::FloatRect collision;

	std::string dialogsPath;
	std::unique_ptr<Dialog> dialogLock = nullptr;
	bool dialogLilboo;

	std::string jsonPath = "WARP_DEBUG";
	std::string key = "";
	std::string boolUnlock = "";
	std::string boolNeeded = "";
	Player::PhylacteryType interactionType = Player::PhylacteryType::NORMAL;

	unsigned char id = -1;
	unsigned char idWarp = -1;
	bool lastCollide = false;
	bool isEnter = false;
};