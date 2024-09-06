#include "Definitions.hpp"
#include "Common.hpp"

std::vector<std::string> GetAllFilesInFolder(const std::string& _pathToFolder)
{
	std::vector<std::string> allFiles;

	for (const auto& entry : std::filesystem::directory_iterator(_pathToFolder))
	{
		std::string temp = entry.path().string();
		allFiles.push_back(temp);
	}

	return allFiles;
}

void DrawPixel(sf::Image& _image, const sf::Vector2i& _pos, const sf::Color& _color)
{
	if (_pos.x >= 0 && _pos.x < (int)_image.getSize().x
		&& _pos.y >= 0 && _pos.y < (int)_image.getSize().y)
	{
		_image.setPixel(_pos.x, _pos.y, _color);
	}
}

std::string GetNameWithoutExtension(const std::string& _fileName)
{
	const size_t pos1 = _fileName.find_last_of("/");
	std::string name = _fileName.substr(pos1 + 1);

	const size_t pos2 = name.find_first_of(".");
	name = name.substr(0, pos2);

	return name;
}

void WriteString(std::ostream& _stream, const std::string& _string)
{
	_stream.write((char*)_string.size(), sizeof(size_t));
	_stream.write(_string.c_str(), _string.size());
}

void InitFileTexture(const std::string& _path)
{
	TexturesManager& textureManager = TexturesManager::Get();
	std::ifstream file(_path);

	while (!file.eof())
	{
		std::string key;
		std::string path;

		file >> key;
		file >> path;

		textureManager.Add(path, key);
	}

	file.close();
}

void DeleteFileTexture(const std::string& _path)
{
	TexturesManager& textureManager = TexturesManager::Get();
	std::ifstream file(_path);

	while (!file.eof())
	{
		std::string key;
		std::string path;

		file >> key;
		file >> path;

		textureManager.Delete(key);
	}

	file.close();
}

// fichier si vraiment vous savez pas où mettre votre code (COUTE ENORMEMENT DE TEMPS A LA COMPILATION A CHAQUE MODIFICATION)