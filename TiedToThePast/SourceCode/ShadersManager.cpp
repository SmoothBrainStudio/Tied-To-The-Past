#include "Definitions.hpp"
#include "ShadersManager.hpp"

ShadersManager& ShadersManager::Get(void)
{
	static ShadersManager myInstance;
	return myInstance;
}

const bool ShadersManager::Add(const std::string& _fileName, const std::string& _key)
{
	const auto it = std::find_if(shadersMap.begin(), shadersMap.end(),
		[&](const std::pair<const std::string, std::unique_ptr<ShaderContainer>>& _shaderPair)
		{
			return _shaderPair.second->fileName == _fileName;
		});

	if (it != shadersMap.end())
	{
		Debug.warning("The shader \"", _fileName, "\" is already in map.");
		return false;
	}
	else
	{
		std::unique_ptr<ShaderContainer> tempShader = std::make_unique<ShaderContainer>();
		if (tempShader->shader.loadFromFile(_fileName, sf::Shader::Fragment))
		{
			tempShader->fileName = _fileName;
			shadersMap[_key] = std::move(tempShader);

			Debug.system("The shader \"", _fileName, "\" is register with the key \"", _key, "\".");
			return true;
		}
		else
		{
			tempShader.release();

			Debug.warning("The shader \"", _fileName, "\" don't exist in assets.");
			return false;
		}
	}
}

const bool ShadersManager::Add(const std::string& _fileName, const std::string& _vert, const std::string& _key)
{
	const auto it = std::find_if(shadersMap.begin(), shadersMap.end(),
		[&](const std::pair<const std::string, std::unique_ptr<ShaderContainer>>& _shaderPair)
		{
			return _shaderPair.second->fileName == _fileName;
		});

	if (it != shadersMap.end())
	{
		Debug.warning("The shader \"", _fileName, "\" is already in map.");
		return false;
	}
	else
	{
		std::unique_ptr<ShaderContainer> tempShader = std::make_unique<ShaderContainer>();
		if (tempShader->shader.loadFromFile(_vert, _fileName))
		{
			tempShader->fileName = _fileName;
			shadersMap[_key] = std::move(tempShader);

			Debug.system("The shader \"", _fileName, "\" is register with the key \"", _key, "\".");
			return true;
		}
		else
		{
			tempShader.release();

			Debug.warning("The shader \"", _fileName, "\" don't exist in assets.");
			return false;
		}
	}
}

void ShadersManager::Delete(const std::string& _key, std::string _str)
{
	const auto it = shadersMap.find(_key);

	if (it != shadersMap.end())
	{
		Debug.system("The shader \"", _key, "\" was delete.");
		shadersMap.erase(_key);
	}
	else
	{
		Debug.warning("The key, \"", _key, "\" don't exist in map. Function \"DeleteShader\" from : ", _str);
	}
}

sf::Shader& ShadersManager::operator[](const std::string& _key)
{
	const auto it = shadersMap.find(_key);

	if (it != shadersMap.end())
	{
		return shadersMap[_key]->shader;
	}
	else
	{
		Debug.error("The key, \n", _key, "\n don't exist. Function \"GetShader\".");
	}

	return shadersMap[_key]->shader; //error return
}