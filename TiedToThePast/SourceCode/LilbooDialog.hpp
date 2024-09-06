#ifndef LILBOO__DIALOG__HPP
#define LILBOO__DIALOG__HPP

#include "NPC.hpp"

class LilbooDialog : public NPC
{
public:
	LilbooDialog(const Json::Value& _mapNpcJson);
	~LilbooDialog();

	void Update(std::shared_ptr<Player>& _player);
	void StartNewDialog(std::shared_ptr<Player>& _player, const std::string& _file);

	void AddDialogRect(const Json::Value& _object);
private:
	// Rect for tests
	struct DialogRect
	{
		DialogRect(const Json::Value& _data)
		{
			rect.setPosition(_data["x"].asFloat(), _data["y"].asFloat());
			rect.setSize(sf::Vector2f(_data["width"].asFloat(), _data["height"].asFloat()));

			file = _data["dialogs"].asString();
			file = file.substr(file.find("/Dialogs/") + sizeof("/Dialogs/") - 1);

			uniqueID = _data["uniqueID"].asString();
			unlockBool = _data["unlockBool"].asString();
			addTie = _data["addTie"].asString();
		}
		sf::RectangleShape rect;
		std::string file;
		std::string uniqueID;
		std::string unlockBool;
		std::string addTie;
	};
	
	std::vector<std::unique_ptr<DialogRect>> dialogRects;

	std::string boolNeeded;
	bool isAppears = false;
	bool startDialog = false;
	unsigned short dialCount = 2;
	float angleWithPlayer = 0.f;
	void MoveTowardsPlayer(std::shared_ptr<Player>& _player);
};

#endif // !LILBOO__DIALOG__HPP