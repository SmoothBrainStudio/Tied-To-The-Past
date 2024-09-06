#include "Definitions.hpp"
#include "JsonManager.hpp"

Json::Value& JsonManager::operator[](const std::string& _key)
{
	const auto it = jsonsMap.find(_key);

	if (it == jsonsMap.end())
	{
		Debug.error("ERROR : WRONG KEY GIVEN IN FUNCTION ", __FUNCTION__, " WITH KEY ", _key);
	}

	return jsonsMap[_key]->jsonValue;
}

JsonManager& JsonManager::Get(void)
{
	static JsonManager myInstance;
	return myInstance;
}

const bool JsonManager::Add(const std::string& _fileName, const std::string& _key)
{
	if (IsFileInMap(_fileName))
	{
		Debug.warning("The json file \"", _fileName, "\" is already in map.");
		return false;
	}
	else
	{
		std::unique_ptr<JsonContainer> tempJson = std::make_unique<JsonContainer>();
		tempJson->jsonValue = LoadFromFile(_fileName);
		tempJson->fileName = _fileName;
		jsonsMap[_key] = std::move(tempJson);
	}

	Debug.system("The json \"", _fileName, "\" is register in the map.");
	return true;
}

void JsonManager::Delete(const std::string& _key)
{
	const auto it = jsonsMap.find(_key);

	if (it != jsonsMap.end())
	{
		jsonsMap.erase(_key);
	}
	else
	{
		std::cerr << "ERROR : WRONG KEY GIVEN IN FUNCTION " << __FUNCTION__ << std::endl;
	}
}

const bool JsonManager::IsFileInMap(const std::string& _fileName)
{
	const auto it = std::find_if(jsonsMap.begin(), jsonsMap.end(),
		[&](const std::pair<const std::string, std::unique_ptr<JsonContainer>>& _jsonPair)
		{
			return _jsonPair.second->fileName == _fileName;
		});

	return it != jsonsMap.end();
}

Json::Value JsonManager::LoadFromFile(const std::string& _fileName)
{
	std::fstream file;
	Json::Reader jsonReader;
	Json::Value jsonValue;

	file.open(_fileName);
	if (!file.is_open())
	{
		Debug.error("ERROR : UNABLE TO OPEN FILE " + _fileName);
	}

	jsonReader.parse(file, jsonValue, false);
	file.close();

	return jsonValue;
}

void JsonManager::SaveToFile(const std::string& _key)
{
	auto it = jsonsMap.find(_key);
	if (it == jsonsMap.end())
	{
		Debug.error("ERROR : WRONG KEY GIVEN IN FUNCTION ", __FUNCTION__);
	}

	std::ofstream file;
	Json::StreamWriterBuilder writer;

	file.open(it->second->fileName, std::ios::trunc);
	file << Json::writeString(writer, it->second->jsonValue);
	file.close();
}