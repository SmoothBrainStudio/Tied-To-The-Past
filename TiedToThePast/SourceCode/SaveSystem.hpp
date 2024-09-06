#ifndef SAVE_SYSTEM__HPP
#define SAVE_SYSTEM__HPP

#include "InGame.hpp"

class SaveSystem
{
public:
	static constexpr const char* SAVES_PATH = "Contents/Data/Saves/";

	~SaveSystem() = default;
	SaveSystem(const SaveSystem&) = delete;
	SaveSystem(SaveSystem&&) = delete;
	SaveSystem& operator=(const SaveSystem&) = delete;
	SaveSystem& operator=(SaveSystem&&) = delete;

	static SaveSystem& Get(void);

	static std::vector<std::string> GetAllSaveNames();
	static bool CheckIfSaveExists(const std::string& _saveName);
	static bool SavesEmpty();

	float GetSaveTime(const std::string& _name);
	void SetSaveName(const std::string& _name);
	const std::string& GetSaveName() const;
	void SaveGame(InGame* _inGame);
	void LoadGame(InGame* _inGame);
	void RemoveSave();
	void SelectLastSave();

private:
	SaveSystem() = default;

	std::string currentFileName;
};

#endif // !SAVE_SYSTEM__HPP
