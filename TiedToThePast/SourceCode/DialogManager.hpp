#ifndef DIALOG_MANAGER__HPP
#define DIALOG_MANAGER__HPP

#include "NPC_Manager.hpp"
#include "DialogMap.hpp"
#include "Lilboo.hpp"
#include "LilBooBox.hpp"

class DialogManager
{
public:
	DialogManager(void) = default;
	~DialogManager(void)
	{
		lilboo.reset();
		dialogsMap.clear();
	}

	void AddDialog(const Json::Value& _jsonData);

	void Event(sf::Event& _event, Player& _player);

	void Update(Player& _player);

	DialogMap& GetDialogMap(const int& _mapID);

	void SetLilboo(const Json::Value& _mapNpcJson);

	void Clear(void);

private:
	std::unique_ptr<Lilboo> lilboo = nullptr;
	std::vector<std::unique_ptr<DialogMap>> dialogsMap;
	LilBooBox lilbooBox;
};

#endif // !DIALOG_MANAGER__HPP