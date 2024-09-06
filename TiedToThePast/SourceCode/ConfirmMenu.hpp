#pragma once
#include "MenuItem.hpp"

class ConfirmMenu : public MenuItem
{
public:
	ConfirmMenu();
	~ConfirmMenu() override;
	void SetText(const sf::String& _text);
	void Reset();
	void Close();
	bool IsFinished() const;
	bool IsConfirmed() const;
	void Confirm();
	void Cancel();
	void PollEvent(GameData& _gameData) override;
	sf::FloatRect GetRect() const override;

private:
	constexpr static float SIZE_X = 0.5f * WORKING_HEIGHT;
	constexpr static float SIZE_Y = 0.5f * WORKING_HEIGHT;
	constexpr static float POS_X = (WORKING_WIDTH - SIZE_X) * 0.5f;
	constexpr static float POS_Y = (WORKING_HEIGHT - SIZE_Y) * 0.5f;
	
	void UpdateKeyTextures();
	void draw(sf::RenderTarget& _target, sf::RenderStates _states) const override;

	sf::Sprite background;
	sf::Text text;
	sf::Sprite confirmSprite;
	sf::Sprite cancelSprite;
	bool isFinished = true;
	bool isConfirmed = false;
};
