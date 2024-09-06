#pragma once
#include "MenuItem.hpp"

class ImageButton : public MenuItem
{
public:
	ImageButton();
	~ImageButton() override = default;
	void SetColor(const sf::Color& _color);
	void SetHoveredColor(const sf::Color& _color);
	bool IsHovered() const;
	void SetTexture(const sf::Texture& _texture);
	void PollEvent(GameData& _gameData) override;
	sf::FloatRect GetRect() const override;

	void CallOnClick(bool _isClick = true) override;
	void CallOnOver(bool _isOver = true) override;
	void CallOnOut(bool _isOut = true) override;

private:
	void draw(sf::RenderTarget& _target, sf::RenderStates _states) const override;

	sf::Sprite sprite;
	sf::Color color = sf::Color::White;
	sf::Color hoveredColor = sf::Color(200, 200, 200);
	bool isHovered = false;
	bool wasHovered = false;
};