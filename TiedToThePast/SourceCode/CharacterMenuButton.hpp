#pragma once
#include "MenuItem.hpp"
#include "Tie.hpp"
#include "Item.hpp"

class CharacterMenuButton : public MenuItem
{
public:
	constexpr static float WIDTH = 200.f;
	constexpr static float HEIGHT = 200.f;

	CharacterMenuButton();
	~CharacterMenuButton() override = default;
	CharacterMenuButton(std::shared_ptr<Item> _item, const sf::Vector2f& _position = { 0.f, 0.f });
	void SetSelected(bool _flag);
	std::shared_ptr<Item> GetItem() const;
	void PollEvent(GameData& _gameData) override;
	sf::FloatRect GetRect() const override;

private:
	sf::Sprite background;
	sf::Sprite sprite;
	std::shared_ptr<Item> item;

	void draw(sf::RenderTarget& _target, sf::RenderStates _states) const override;
};
