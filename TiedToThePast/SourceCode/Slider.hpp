#ifndef SLIDER_HPP
#define SLIDER_HPP

#include "MenuItem.hpp"

class Slider: public MenuItem
{
public:
    Slider(const sf::Vector2f& _position, const sf::Vector2f& _size, int _value = 0, const sf::Color& _backgroundColor = sf::Color::White, const sf::Color& _fillColor = sf::Color::Green, const sf::Color& _buttonColor = sf::Color::Blue);
    ~Slider() override = default;

    void PollEvent(GameData& _gameData) override;

    int GetValue();

    void SetValue(float val);
    void SetMinValue(float val);
    void SetMaxValue(float val);
    void SetIncrement(float val);

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::RectangleShape sliderBar;
    sf::RectangleShape sliderButton;
    sf::RectangleShape sliderFill;
    sf::Color buttonColor;

    bool isDragging = false;

    int increment = 1;
    int value;
    int minValue = 0;
    int maxValue = 100;
};

#endif // !SLIDER_HPP
