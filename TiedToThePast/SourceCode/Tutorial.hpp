#ifndef TUTORIAL_HPP
#define TUTORIAL_HPP

#include "Definitions.hpp"
#include "Button.hpp"
#include "DialogBox.hpp"
//Copyright 2024 Property of Olivier Maurin.All rights reserved.

class Tutorial : public sf::Drawable, public sf::Transformable
{
public:
	Tutorial(const std::vector<std::string>& _tutoTextureKeys, const std::vector<std::string>& _descriptionPaths,const std::string& _keyDraw);
	~Tutorial();

	void PollEvent(GameData& _gameData);
	void Update(GameData& _gameData);

	void ClearTiesPages();
	void AddNewTuto(const std::vector<std::string>& _tutoTextureKeys, const std::vector<std::string>& _descriptionPaths);
	void SetCurPage(const unsigned int& _page);

private:
	static constexpr float ARROW_OFFSET = 175.f;
	static constexpr float TEXT_OFFSET = 65.f;

	void draw(sf::RenderTarget& _target, sf::RenderStates _states) const override;

	void InitButtons(void);

	enum Arrows
	{
		LEFT,
		RIGHT
	};


	unsigned int curPage = 0u;
	unsigned int nbPages = 0u;
	unsigned int nbBasePages = 0u;
	int currentButton = RIGHT;
	sf::Sprite screenShot;
	std::vector<std::string> tutoTextureKeys;
	std::vector<Dialog*> descriptions;
	std::array<std::unique_ptr<Button>, 2> arrows;
	sf::Text nbPagesText;

	std::string keyDraw;
};

#endif // !TUTORIAL_HPP



