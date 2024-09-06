#ifndef SHOP_BUTTON__H
#define SHOP_BUTTON__H

#include "Player.hpp"
#include "MenuItem.hpp"

class ShopButton : public MenuItem
{
public:
	constexpr static float WIDTH = 141.f;
	constexpr static float SCALE = 0.75f;

	ShopButton(Player& _player, const sf::Vector2f& _position, std::shared_ptr<Item> _item);
	~ShopButton() override = default;
	void SetSelected(bool _flag);
	void Buy();
	void SetCount(sf::Uint16 _count);
	sf::Uint16 GetCount() const;
	bool IsEmpty() const;
	const Item* GetItem() const;
	void UpdateRenderTexture();
	void PollEvent(GameData& _gameData) override;
	sf::FloatRect GetRect() const override;
	void SetShopIndex(sf::Uint32 _index);
	sf::Uint32 GetShopIndex() const;

private:
	void draw(sf::RenderTarget& _target, sf::RenderStates _states) const override;

	Player& player;
	std::shared_ptr<Item> item;
	sf::Sprite sprite;
	bool isSold = false;
	sf::Uint32 shopIndex = 0;
};

#endif