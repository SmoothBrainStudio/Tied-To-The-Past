#ifndef JSON_MANAGER__HPP
#define JSON_MANAGER__HPP

#include "Definitions.hpp"

class JsonManager
{
public:
	~JsonManager() = default;
	JsonManager(const JsonManager&) = delete;
	JsonManager(JsonManager&&) = delete;
	JsonManager& operator=(const JsonManager&) = delete;
	JsonManager& operator=(JsonManager&&) = delete;

	static JsonManager& Get(void);
	Json::Value& operator[](const std::string& _key);

	const bool Add(const std::string& _fileName, const std::string& _key);
	void Delete(const std::string& _key);
	const bool IsFileInMap(const std::string& _fileName);

	static Json::Value LoadFromFile(const std::string& _fileName);
	void SaveToFile(const std::string& _key);

private:
	JsonManager() = default;

	struct JsonContainer
	{
		~JsonContainer(void)
		{
			jsonValue.clear();
		}
		std::string fileName;
		Json::Value jsonValue;
	};

	std::unordered_map<std::string, std::unique_ptr<JsonContainer>> jsonsMap;
};

#endif // !JSON_MANAGER__HPP
