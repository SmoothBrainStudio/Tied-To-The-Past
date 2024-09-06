#pragma once
#include "MenuItem.hpp"

class SaveButton : public MenuItem
{
public:
	SaveButton();
	SaveButton(const sf::Vector2f& _size, const sf::String& _text);
	~SaveButton() override = default;

private:
	void Init();
	
public:
	void Update(GameData& _gameData) override;
	void PollEvent(GameData& _gameData) override;
	sf::FloatRect GetRect() const override;
	void SetSelected(bool _selected);
	void SetSize(const sf::Vector2f& _size);
	
private:
	constexpr static float NAME_POS_X = 25.f;

	void draw(sf::RenderTarget& _target, sf::RenderStates _states) const override;

	sf::RectangleShape rect;
	sf::Text nameText;
	sf::Sprite hourglass;
	sf::Text timeText;
	std::vector<sf::Sprite> ties;
	bool isHovered = false;
};
