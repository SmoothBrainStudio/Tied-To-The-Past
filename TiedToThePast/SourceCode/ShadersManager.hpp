#ifndef SHADER_MANAGER__HPP
#define SHADER_MANAGER__HPP

#include"Definitions.hpp"


class ShadersManager
{
public:
	~ShadersManager() = default;
	ShadersManager(const ShadersManager&) = delete;
	ShadersManager(ShadersManager&&) = delete;
	ShadersManager& operator=(const ShadersManager&) = delete;
	ShadersManager& operator=(ShadersManager&&) = delete;

	static ShadersManager& Get(void);

	const bool Add(const std::string& _fileName, const std::string& _key);
	const bool Add(const std::string& _fileName, const std::string& _vert, const std::string& _key);
	void Delete(const std::string& _key, std::string str = __builtin_FUNCTION());

	sf::Shader& operator [](const std::string& _key);

private:
	ShadersManager() = default;


	struct ShaderContainer
	{
		std::string fileName;
		sf::Shader shader;
	};

	std::unordered_map<std::string, std::unique_ptr<ShaderContainer>> shadersMap;
};

#endif // !SHADER_MANAGER__HPP



