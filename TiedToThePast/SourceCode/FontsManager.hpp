#ifndef FONTS_MANAGER__HPP
#define FONTS_MANAGER__HPP

#include "Definitions.hpp"

class FontsManager
{
public:
	~FontsManager() = default;
	FontsManager(const FontsManager&) = delete;
	FontsManager(FontsManager&&) = delete;
	FontsManager& operator=(const FontsManager&) = delete;
	FontsManager& operator=(FontsManager&&) = delete;

	static FontsManager& Get(void);

	const bool Add(const std::string& _fileName, const std::string& _key);
	void Delete(const std::string& _key, std::string str = __builtin_FUNCTION());

	sf::Font& operator [](const std::string& _key);
private:
	FontsManager() = default;

	struct FontContainer
	{
		std::string fileName;
		sf::Font font;
	};

	std::unordered_map<std::string, std::unique_ptr<FontContainer>> fontMap;
};


#endif // !FONTS_MANAGER_H



