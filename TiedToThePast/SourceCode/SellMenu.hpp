#pragma once
#include "Player.hpp"
#include "ConfirmMenu.hpp"
#include "Button.hpp"
#include "VerticalScrollMenu.hpp"

class SellMenu : public sf::Transformable, public sf::Drawable
{
public:
	SellMenu() = default;
	~SellMenu() = default;
	void Init(GameData& _gameData, std::shared_ptr<Player> _player);
	void Update(GameData& _gameData);
	bool PollEvent(GameData& _gameData);
	const sf::FloatRect GetGlobalBounds();
	void SelectItem(int _index);
	void SellSelected();
	void SetColor(const sf::Color& _color);

private:
	void draw(sf::RenderTarget& _target, sf::RenderStates _states) const override;

	Button* hoveredButton = nullptr;
	const Item* selectedItem = nullptr;
	GameData* gameData = nullptr;
	std::shared_ptr<Player> player = nullptr;
	ConfirmMenu confirmMenu;

	TexturesManager& texMng = TexturesManager::Get();
	DrawManager& drwMng = DrawManager::Get();
	Screen& scrMng = Screen::Get();

	const sf::Vector2f SIZE_BACKGROUND = sf::Vector2f(751.f, 811.f);
	const sf::Vector2f START_ORIGIN = sf::Vector2f(22.f, 346.f);
	const sf::Vector2f SIZE_ONE_BOX = sf::Vector2f(166.f, 168.f);
	const sf::Vector2f SIZE_ALL_BOX = sf::Vector2f(707.f, 349.f);
	static constexpr float SPACE_BETWEEN_BOXS = 14.f;
	static constexpr float OFFSET_OVER_MOUSE = 48.f;

	sf::RectangleShape background;
	VerticalScrollMenu scrollMenu;
	std::vector<const Item*> items;
	unsigned short scrollValue = 0;
};
