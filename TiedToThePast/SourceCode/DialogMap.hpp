#ifndef DIALOG_MAP__HPP
#define DIALOG_MAP__HPP

#include "DialogBox.hpp"
#include "Player.hpp"

class DialogMap
{
public:
	static constexpr float COLLIDE_COOLDOWN = 3.f;

	DialogMap(const Json::Value& _jsonData);

	~DialogMap(void);

	void Event(sf::Event& _event, Player& _player);
	void Update(Player& _player);

	void UpdateDialog(Player& _player);

	void StartDialog(Player& _player);

	void SetPos(const sf::Vector2f& _pos);

	const bool& Ready(void) const;
	const std::string& GetFile(void) const;
	const bool& IsPlaying(void);
	const std::string& GetType(void);
	void SetReady(const bool& _ready)
	{
		dialogReady = _ready;
	}
	const int& GetId(void) const
	{
		return id;
	}

private:
	std::unique_ptr<Dialog> dialogList = nullptr;

	std::string type = "THINKING";
	std::string uniqueID;
	std::string boolNeeded;
	std::string boolUnlocked;
	std::string addTie;
	std::string file;
	int id;

	bool unique = false;

	sf::FloatRect boxCollision;
	bool interaction = false;
	bool pushBack = false;

	bool dialogReady = false; // pas d'idée

	float coolDown = 0.f;

	bool wasCollide = false;
};

#endif // !DIALOG_MAP__HPP