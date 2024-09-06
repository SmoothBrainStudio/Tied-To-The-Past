#ifndef KEYINPUT_HPP
#define KEYINPUT_HPP

#include "MenuItem.hpp"

class KeyInput: public MenuItem
{
public:
    KeyInput(const sf::Vector2f& _position, const sf::Vector2f& _size, const sf::Color& _backgroundColor = sf::Color::White);
    ~KeyInput() override = default;

    void PollEvent(GameData& _gameData) override;

    void ChangeInputKey(sf::Event _event);

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Color rectColor;
    sf::RectangleShape rect;
    sf::Text text;
    std::string key;
    sf::Sprite keySprite;
    bool isClicked = false;
};

std::string GetPressedKey(sf::Event _event);

#endif // !KEYINPUT_HPP
