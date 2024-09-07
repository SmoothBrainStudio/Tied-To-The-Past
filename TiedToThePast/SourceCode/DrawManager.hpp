#ifndef DRAW_MANAGER__HPP
#define DRAW_MANAGER__HPP

#include "Definitions.hpp"
#include "Screen.hpp"
//Copyright 2024 Property of Olivier Maurin.All rights reserved.

class DrawManager
{
public:
	~DrawManager() = default;
	DrawManager(const DrawManager&) = delete;
	DrawManager(DrawManager&&) = delete;
	DrawManager& operator=(const DrawManager&) = delete;
	DrawManager& operator=(DrawManager&&) = delete;

	struct DrawableContainer
	{
		~DrawableContainer() {};
		sf::Transformable* transformablePart = nullptr;
		sf::Drawable* drawablePart = nullptr;
		bool isVisible = false;
		sf::Shader* shader = nullptr;
	};

	struct RendererContainer
	{
		~RendererContainer() {};
		sf::RenderTexture RT;
		sf::Sprite RTSprite;
		sf::Shader* shader = nullptr;
		sf::Color clearColor = sf::Color::Transparent;
	};

	static DrawManager& Get(void);

	const bool AddSection(const std::string& _keySection, const sf::Color& _clearColor = sf::Color::Transparent);

	template<typename T> const bool AddObject(const std::string& _keySection, T& _obj, const bool& _isVisible = true, sf::Shader* _shader = nullptr)
	{
		if (sectionsMap.find(_keySection) == sectionsMap.end())
		{
			Debug.warning("KEY'", _keySection, "' NOT FOUND IN MAP");
			return false;
		}

		std::unique_ptr<DrawableContainer> temp = std::make_unique<DrawableContainer>();
		temp->transformablePart = &_obj;
		temp->drawablePart = &_obj;
		temp->isVisible = _isVisible;

		if (_shader != nullptr)
		{
			temp->shader = _shader;
		}

		sectionsMap[_keySection].push_back(std::move(temp));

		return true;
	};

	void DeleteSection(const std::string& _keySection);
	void DeleteObject(const std::string& _keySection, const sf::Drawable* _drawableObj);
	void Draw(sf::RenderWindow& _window, const std::string& _keySection, const bool& _sorting = false);
	void SetObjectVisible(const std::string& _keySection, const sf::Drawable* _drawableObj, const bool& _visible = true);
	void SetObjectShader(const std::string& _keySection, const sf::Drawable* _drawableObj, sf::Shader* _shader = nullptr);
	void SetRenderTextureShader(const std::string& _keySection, sf::Shader* _shader = nullptr);
	bool IsObjectInSection(const std::string& _keySection, const sf::Drawable* _drawableObj);

	std::unordered_map<std::string, std::unique_ptr<RendererContainer>>& GetRenderTextureMap(void) { return renderTextureMap; };

private:
	DrawManager() = default;

	void SectionSort(const std::string& _keySection);

	std::unordered_map<std::string, std::unique_ptr<RendererContainer>> renderTextureMap;
	std::unordered_map<std::string, std::vector<std::unique_ptr<DrawableContainer>>> sectionsMap;
};
#endif // !DRAW_MANAGER__HPP



