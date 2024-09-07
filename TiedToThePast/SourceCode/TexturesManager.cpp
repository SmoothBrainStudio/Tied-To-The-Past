#include "Definitions.hpp"
#include "TexturesManager.hpp"
//Copyright 2024 Property of Olivier Maurin.All rights reserved.

TexturesManager& TexturesManager::Get(void)
{
	static TexturesManager myInstance;
	return myInstance;
}

const bool TexturesManager::Add(const std::string& _fileName, const std::string& _key, const bool& _isSmooth)
{
	if (IsFileInMap(_fileName))
	{
		Debug.warning("The texture \"", _fileName, "\" is already in map.");
		return false;
	}

	std::unique_ptr<TextureContainer> tempTexture = std::make_unique<TextureContainer>();
	if (tempTexture->texture.loadFromFile(_fileName))
	{
		tempTexture->texture.setSmooth(_isSmooth);
		tempTexture->fileName = _fileName;
		textureMap[_key] = std::move(tempTexture);

		Debug.system("The texture \"", _fileName, "\" is register with the key \"", _key, "\".");
		return true;
	}
	else
	{
		tempTexture.reset();

		Debug.warning("The texture \"", _fileName, "\" don't exist in assets.");
		return false;
	}
}

void TexturesManager::Delete(const std::string& _key, std::string str)
{
	const auto it = textureMap.find(_key);

	if (it != textureMap.end())
	{
		Debug.system("The texture \"", _key, "\" was delete.");
		textureMap[_key].reset();
		textureMap.erase(_key);
	}
	else
	{
		Debug.warning("The key, \"", _key, "\" don't exist in map. Function \"DeleteTexture\" from : ", str);
	}
}

const bool TexturesManager::IsFileInMap(const std::string& _fileName)
{
	const auto it = std::find_if(textureMap.begin(), textureMap.end(),
		[&](const std::pair<const std::string, std::unique_ptr<TextureContainer>>& _texturePair)
		{
			return _texturePair.second->fileName == _fileName;
		});

	return it != textureMap.end();
}

sf::Texture& TexturesManager::operator[](const std::string& _key)
{
	const auto it = textureMap.find(_key);

	if (it != textureMap.end())
	{
		return textureMap[_key]->texture;
	}
	else
	{
		Debug.error("The key, \"", _key, "\" don't exist. Function \"GetTexture\"");
		throw;
	}
}