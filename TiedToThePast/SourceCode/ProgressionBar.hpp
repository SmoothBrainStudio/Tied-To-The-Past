#ifndef LOADING_BAR__HPP
#define LOADING_BAR__HPP

#include "MenuItem.hpp"

class ProgressionBar : public MenuItem
{
public:
    // The value of the bar is between 0 and 1
    ProgressionBar(const sf::Vector2f& _position, const sf::Vector2f& _size, float _value = 0.f, const sf::Color& _backgroundColor = sf::Color::White, const sf::Color& _loadingColor = sf::Color::Red);
    ~ProgressionBar() override = default;

    void PollEvent(GameData& _gameData) override;

    float GetValue() const;
    const sf::Color& GetBackgroundColor() const;
    const sf::Color& GetLoadingColor() const;

    void SetValue(float _value);
    void SetBackgroundColor(const sf::Color& _color);
    void SetLoadingColor(const sf::Color& _color);

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::RectangleShape backgroundRect;
    sf::RectangleShape loadingRect;

    float value;
};

#endif