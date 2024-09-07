#include "Definitions.hpp"
#include "FontsManager.hpp"
//Copyright 2024 Property of Olivier Maurin.All rights reserved.

FontsManager& FontsManager::Get(void)
{
	static FontsManager myInstance;
	return myInstance;
}

const bool FontsManager::Add(const std::string& _fileName, const std::string& _key)
{
	const auto it = std::find_if(fontMap.begin(), fontMap.end(),
		[&](const std::pair<const std::string, std::unique_ptr<FontContainer>>& _fontPair)
		{
			return _fontPair.second->fileName == _fileName;
		});

	if (it != fontMap.end())
	{
		Debug.warning("The font \"", _fileName, "\" is already in map.");
		return false;
	}
	else
	{
		std::unique_ptr<FontContainer> tempFont = std::make_unique<FontContainer>();
		if (tempFont->font.loadFromFile(_fileName))
		{
			tempFont->fileName = _fileName;
			fontMap[_key] = std::move(tempFont);

			Debug.system("The font \"", _fileName, "\" is register with the key \"", _key, "\".");
			return true;
		}
		else
		{
			tempFont.release();

			Debug.warning("The font \"", _fileName, "\" don't exist in assets.");
			return false;
		}
	}
}

void FontsManager::Delete(const std::string& _key, std::string _str)
{
	const auto it = fontMap.find(_key);

	if (it != fontMap.end())
	{
		Debug.system("The font \"", _key, "\" was delete.");
		fontMap.erase(_key);
	}
	else
	{
		Debug.warning("The font \"", _key,"\" don't exist in map. Function \"DeleteFont\" from : ", _str);
	}
}

sf::Font& FontsManager::operator[](const std::string& _key)
{
	const auto it = fontMap.find(_key);

	if (it != fontMap.end())
	{
		return fontMap[_key]->font;
	}
	else
	{
		Debug.error("The font \"", _key," \" don't exist in map. Function \"GetFont\".");
		throw;
	}
}
