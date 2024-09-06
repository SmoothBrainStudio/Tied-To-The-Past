#include "Definitions.hpp"
#include "BoolManager.hpp"

BoolManager& BoolManager::Get(void)
{
	static BoolManager myInstance;
	return myInstance;
}

bool& BoolManager::operator[](const std::string& _key)
{
	if (boolMap.find(_key) == boolMap.end())
	{
		Debug.system("The boolean \"", _key, "\" is register.");
		boolMap[_key] = false;
	}

	return boolMap[_key];
}

void BoolManager::Load(std::ifstream& _buffer)
{
	Reset();

	size_t binarySize;
	_buffer.read((char*)&binarySize, sizeof(size_t));

	for (int i = 0; i < binarySize; i++)
	{
		size_t size;
		bool second;

		_buffer.read((char*)&size, sizeof(size_t));

		char* buffer = new char[size];
		_buffer.read(buffer, size);
		std::string first(buffer, size);
		delete[] buffer;

		_buffer.read((char*)&second, sizeof(bool));

		boolMap[first] = second;
	}
}

void BoolManager::Save(std::ofstream& _buffer)
{
	size_t binarySize = boolMap.size();
	_buffer.write((char*)&binarySize, sizeof(size_t));

	for (auto& object : boolMap)
	{
		size_t size = object.first.size();
		_buffer.write((char*)&size, sizeof(size_t));
		_buffer.write(object.first.c_str(), size);
		_buffer.write((char*)&object.second, sizeof(bool));
	}
}

void BoolManager::Reset(void)
{
	boolMap.clear();
}