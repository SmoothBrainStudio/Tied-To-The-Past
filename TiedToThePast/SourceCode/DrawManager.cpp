#include "Definitions.hpp"
#include "DrawManager.hpp"
//Copyright 2024 Property of Olivier Maurin.All rights reserved.

DrawManager& DrawManager::Get(void)
{
	static DrawManager myInstance;
	return myInstance;
}

const bool DrawManager::AddSection(const std::string& _keySection, const sf::Color& _clearColor)
{
	if (sectionsMap.find(_keySection) != sectionsMap.end())
	{
		Debug.warning("The section \"", _keySection, "\" is already in map.");
		return false;
	}

	std::unique_ptr<RendererContainer> tempSection = std::make_unique<RendererContainer>();
	tempSection->RT.create((unsigned int)Screen::Get().GetWidth(), (unsigned int)Screen::Get().GetHeight());
	tempSection->RT.display();
	tempSection->RTSprite.setTexture(tempSection->RT.getTexture());
	tempSection->clearColor = _clearColor;

	renderTextureMap[_keySection] = std::move(tempSection);
	sectionsMap[_keySection] = std::vector<std::unique_ptr<DrawableContainer>>();

	Debug.system("The section \"", _keySection, "\" is registered in the map.");
	return true;
}

void DrawManager::DeleteObject(const std::string& _keySection, const sf::Drawable* _drawableObj)
{
	const auto it = std::find_if(sectionsMap[_keySection].begin(), sectionsMap[_keySection].end(),
		[&](const std::unique_ptr<DrawableContainer>& _drawableContainer)
		{
			return _drawableContainer->drawablePart == _drawableObj;
		});

	if (it != sectionsMap[_keySection].end())
	{
		(*it)->drawablePart = nullptr;
		(*it)->transformablePart = nullptr;
		(*it)->shader = nullptr;
		sectionsMap[_keySection].erase(it);
		Debug.system("Object in \"", _keySection, "\" was delete.");
	}
}

void DrawManager::DeleteSection(const std::string& _keySection)
{
	if (sectionsMap.find(_keySection) != sectionsMap.end())
	{
		if (!sectionsMap[_keySection].empty())
		{
			for (int i = (int)(sectionsMap[_keySection].size() - 1); i >= 0; --i)
			{
				DeleteObject(_keySection, sectionsMap[_keySection][i]->drawablePart);
			}
		}


		sectionsMap.erase(_keySection);
		renderTextureMap.erase(_keySection);
		Debug.system("Section \"", _keySection, "\" was delete.");
	}
	else
	{
		Debug.warning("Can't find section \"", _keySection, "\".");
	}
}

void DrawManager::Draw(sf::RenderWindow& _window, const std::string& _keySection, const bool& _sorting)
{
	if (_sorting)
	{
		SectionSort(_keySection);
	}

	renderTextureMap[_keySection]->RT.clear(renderTextureMap[_keySection]->clearColor);
	renderTextureMap[_keySection]->RT.setView(_window.getView());
	renderTextureMap[_keySection]->RTSprite.setPosition(_window.mapPixelToCoords(sf::Vector2i(0, 0)));

	//draw all objects in the renderTexture
	for (auto& it : sectionsMap[_keySection])
	{
		if (it->isVisible)
		{
			sf::Vector2f pos = it->transformablePart->getPosition();
			sf::Vector2f scale = it->transformablePart->getScale();

			it->transformablePart->setPosition(Screen::Get().Pos(pos));
			it->transformablePart->setScale(Screen::Get().Scale(scale));

			if (sf::Shader::isAvailable() && it->shader != nullptr)
			{
				renderTextureMap[_keySection]->RT.draw(*it->drawablePart, it->shader);
			}
			else
			{
				renderTextureMap[_keySection]->RT.draw(*it->drawablePart);
			}

			it->transformablePart->setPosition(pos); //resetting initial pos after draw to avoid loop adding constantly the offset of a bigger/smaller screen
			it->transformablePart->setScale(scale); //same thing
		}
	}

	//draw renderTexture and a shader in all the renderTexture if needed
	if (sf::Shader::isAvailable() && renderTextureMap[_keySection]->shader != nullptr)
	{
		_window.draw(renderTextureMap[_keySection]->RTSprite, renderTextureMap[_keySection]->shader);
	}
	else
	{
		_window.draw(renderTextureMap[_keySection]->RTSprite);
	}

	renderTextureMap[_keySection]->RT.display();
}

void DrawManager::SectionSort(const std::string& _keySection)
{
	std::sort(sectionsMap[_keySection].begin(), sectionsMap[_keySection].end(),
		[](const std::unique_ptr<DrawableContainer>& _drawable1, const std::unique_ptr<DrawableContainer>& _drawable2)
		{
			return _drawable1->transformablePart->getPosition().y < _drawable2->transformablePart->getPosition().y;
		});
}

void DrawManager::SetObjectVisible(const std::string& _keySection, const sf::Drawable* _drawableObj, const bool& _visible)
{
	const auto it = std::find_if(sectionsMap[_keySection].begin(), sectionsMap[_keySection].end(),
		[&](const std::unique_ptr<DrawableContainer>& _drawableContainer)
		{
			return _drawableContainer->drawablePart == _drawableObj;
		});

	if (it != sectionsMap[_keySection].end())
	{
		(*it)->isVisible = _visible;
	}
}

void DrawManager::SetObjectShader(const std::string& _keySection, const sf::Drawable* _drawableObj, sf::Shader* _shader)
{
	const auto it = std::find_if(sectionsMap[_keySection].begin(), sectionsMap[_keySection].end(),
		[&](const std::unique_ptr<DrawableContainer>& _drawableContainer)
		{
			return _drawableContainer->drawablePart == _drawableObj;
		});

	if (it != sectionsMap[_keySection].end())
	{
		(*it)->shader = _shader;
	}
}

void DrawManager::SetRenderTextureShader(const std::string& _keySection, sf::Shader* _shader)
{
	if (renderTextureMap.find(_keySection) != renderTextureMap.end())
	{
		renderTextureMap[_keySection]->shader = _shader;
	}
}

bool DrawManager::IsObjectInSection(const std::string& _keySection, const sf::Drawable* _drawableObj)
{
	const auto it = std::find_if(sectionsMap[_keySection].begin(), sectionsMap[_keySection].end(),
		[&](const std::unique_ptr<DrawableContainer>& _drawableContainer)
		{
			return _drawableContainer->drawablePart == _drawableObj;
		});
	if (it != sectionsMap[_keySection].end())
	{
		return true;
	}
	else
	{
		return false;
	}
}
