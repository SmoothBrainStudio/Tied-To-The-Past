#ifndef SELECT_HPP
#define SELECT_HPP

#include "MenuItem.hpp"
#include "Text.hpp"

#define SELECT_TEXT_OFFSET 20
#define COLOR_OPTION_UNSELECT 220, 220, 220, 255
#define COLOR_OPTION_SELECT 170, 170, 170, 255

class SelectMenu: public MenuItem
{
public:
    SelectMenu(float _x, float _y, float _width = 0.f, float _height = 0.f);
    ~SelectMenu() override = default;

    void AddOption(std::string _str, std::function<void()>&& _onClickFunc, bool _isCurrentOption = false);
    void VerifyMaxSize(int _optionIndex);
    void Sort();

    void PollEvent(GameData& _gameData) override;

    void SetCurrentOption(int _cur);
    std::string GetCurrentOption();

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    // Main case
    sf::RectangleShape menuBox;

    // Others cases
    std::vector<sf::RectangleShape> optionRects;
    std::vector<sf::Text> options;
    std::vector<std::function<void()>> onClick;

    // Common
    int currentOption;
    int currentOver;
    bool isOpen = false;
    sf::Vector2f sizeBox;

    sf::FloatRect GetOptionRect(int optionIndex);
};

#endif // !SELECT_HPP
