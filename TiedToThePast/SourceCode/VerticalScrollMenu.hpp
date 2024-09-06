#pragma once
#include "MenuItem.hpp"

class VerticalScrollMenu : public MenuItem
{
public:
	VerticalScrollMenu();
	~VerticalScrollMenu() override;
	void SetLooping(bool _isLooping);
	void AddItem(MenuItem* _item);
	void Clear();
	std::vector<MenuItem*>& GetItems();
	size_t GetNbItems() const;
	void SetSelectedItemIndex(size_t _index);
	void SelectLeft();
	void SelectRight();
	void SelectUp();
	void SelectDown();
	void Update(GameData& _gameData) override;
	void PollEvent(GameData& _gameData) override;
	void SetSize(const sf::Vector2f& _size);
	sf::FloatRect GetRect() const override;
	MenuItem* operator[](size_t _index);
	void SetVerticalMode(bool _flag);
	bool GetVerticalMode() const;

private:
	constexpr static float BAR_WIDTH = 10.f;

	void CallSelectedItem();
	// Return true if an item is hovered
	bool SelectItemAtMousePos(GameData& _gameData);
	void draw(sf::RenderTarget& _target, sf::RenderStates _states) const override;
	
	std::vector<MenuItem*> items;
	sf::RectangleShape bar;
	mutable sf::RenderTexture renderTexture;
	mutable sf::Sprite sprite;
	float scrollOffset = 0.f;
	float maxBottom = 0.f;
	size_t selectedItemIndex = 0;
	bool isLooping = true;
	bool verticalMode = true;
};
