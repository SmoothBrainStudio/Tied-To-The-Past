#ifndef CHECKBOX__HPP
#define CHECKBOX__HPP

#include "MenuItem.hpp"

static constexpr unsigned short int SPACE_BETWEEN_CHECK_TEXT = 16;
static constexpr float SIZE = 54.f;

class Checkbox : public MenuItem
{
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void AdapatText();

    sf::RectangleShape* checkbox;
    sf::RectangleShape* check;
    sf::Text* text;
    bool value;
    bool lastValue;

public:
    Checkbox();
    ~Checkbox() override;

    void Update(GameData& _gameData) override;
    void PollEvent(GameData& _gameData) override;

    void SetCheckboxTexture(const sf::Texture& _texture);
    void SetCheckTexture(const sf::Texture& _texture);
    void SetValue(const bool& _value);
    void SetText(const std::string& _text);
    void SetText(const sf::Text& _text);

    sf::FloatRect GetRect() const override;
    const bool& IsChecked(void);
};

#endif