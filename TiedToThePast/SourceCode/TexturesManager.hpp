#ifndef TEXTURES_MANAGER__HPP
#define TEXTURES_MANAGER__HPP

#include "Definitions.hpp"
//Copyright 2024 Property of Olivier Maurin.All rights reserved.

class TexturesManager
{
public:
	~TexturesManager() = default;
	TexturesManager(const TexturesManager&) = delete;
	TexturesManager(TexturesManager&&) = delete;
	TexturesManager& operator=(const TexturesManager&) = delete;
	TexturesManager& operator=(TexturesManager&&) = delete;

	static TexturesManager& Get(void);

	void Delete(const std::string& _key, std::string str = __builtin_FUNCTION());
	const bool Add(const std::string& _fileName, const std::string& _key, const bool& _isSmooth = true);

	const bool IsFileInMap(const std::string& _fileName);

	sf::Texture& operator[](const std::string& _key);
private:
	TexturesManager() = default;

	struct TextureContainer
	{
		~TextureContainer() = default;
		std::string fileName;
		sf::Texture texture;
	};

	std::unordered_map<std::string, std::unique_ptr<TextureContainer>> textureMap;
};

#endif // !TEXTURES_MANAGER__HPP



