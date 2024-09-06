#include "Definitions.hpp"
#include "SaveSystem.hpp"

SaveSystem& SaveSystem::Get(void)
{
    static SaveSystem myInstance;
    return myInstance;
}

std::vector<std::string> SaveSystem::GetAllSaveNames()
{
    std::vector<std::string> files;
    for (const auto& entry : std::filesystem::directory_iterator(SAVES_PATH))
    {
        if (entry.path().extension() == ".txt")
        {
            files.push_back(entry.path().stem().string());
        }
    }
    return files;
}

bool SaveSystem::CheckIfSaveExists(const std::string& _saveName)
{
    return std::filesystem::exists(SAVES_PATH + _saveName + ".txt");
}

bool SaveSystem::SavesEmpty()
{
    return std::filesystem::is_empty(SAVES_PATH);
}

float SaveSystem::GetSaveTime(const std::string& _name)
{
    std::ifstream file(SAVES_PATH + _name + ".txt");
    float playTime;
    file >> playTime;
    return playTime;
}

void SaveSystem::SetSaveName(const std::string& _name)
{
    this->currentFileName = _name;
}

const std::string& SaveSystem::GetSaveName() const
{
    return this->currentFileName;
}

void SaveSystem::SaveGame(InGame* _inGame)
{
    if (this->currentFileName.empty())
    {
        Debug.warning("Can't save game no file selected (return without loading save)");
        return;
    }

    std::ofstream file(SAVES_PATH + this->currentFileName + ".txt", std::ios::trunc);
    file << GameClock::Get().GetPlayTime() << std::endl;
    file << _inGame->inGameDatas.map->GetName() << std::endl;
    file << *_inGame->inGameDatas.player << std::endl;
    file << *((Shop*)_inGame->subStates[SubStates::SHOP].get()) << std::endl;
    file.close();

    std::ofstream boolFile(SAVES_PATH + this->currentFileName + ".bin", std::ios::trunc | std::ios::binary);
    BoolManager::Get().Save(boolFile);
    boolFile.close();

    Audio::Get().PlaySoundNoSecurity("SAVE_SUCCESS");
}

void SaveSystem::LoadGame(InGame* _inGame)
{
   if (this->currentFileName.empty())
   {
       Debug.warning("Can't load game no file selected (return without loading save)");
       _inGame->inGameDatas.map = std::make_unique<Map>("Contents/Data/Tiled/Maps/Garden.json", _inGame->inGameDatas);
       return;
   }
   
   std::ifstream boolFile(SAVES_PATH + this->currentFileName + ".bin", std::ios::binary);
   BoolManager::Get().Load(boolFile);
   boolFile.close();
   
   std::ifstream file(SAVES_PATH + this->currentFileName + ".txt");
   
   float playTime;
   file >> playTime;
   GameClock::Get().SetPlayTime(playTime);
   
   std::string mapName;
   file >> mapName;
   
   _inGame->inGameDatas.ResetExplorationObjects();
   _inGame->inGameDatas.map.reset();
   _inGame->inGameDatas.map = std::make_unique<Map>(mapName, _inGame->inGameDatas);
   
   file >> *_inGame->inGameDatas.player;
   //file >> *((Shop*)_inGame->subStates[SubStates::SHOP].get());
   
   file.close();
   
   Audio::Get().PlaySoundNoSecurity("CHANGE_TIE");
   Audio::Get().StopAllMusics();
   Audio::Get().PlayMusic("EXPLORATION_MUSIC", true);
}

void SaveSystem::RemoveSave()
{
    std::filesystem::remove(SAVES_PATH + this->currentFileName + ".txt");
    std::filesystem::remove(SAVES_PATH + this->currentFileName + ".bin");
}

void SaveSystem::SelectLastSave()
{
    if (std::filesystem::is_empty(SAVES_PATH))
    {
        return;
    }

    std::filesystem::directory_iterator it(SAVES_PATH);
    this->currentFileName = it->path().stem().string();
    std::filesystem::file_time_type latest = std::filesystem::last_write_time(it->path());
    ++it;
    while (!it._At_end())
    {
        std::filesystem::file_time_type time = std::filesystem::last_write_time(it->path());
        if (time > latest)
        {
            time = latest;
            this->currentFileName = it->path().stem().string();
        }
        ++it;
    }
}