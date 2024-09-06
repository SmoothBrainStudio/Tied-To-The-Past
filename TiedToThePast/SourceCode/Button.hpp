#ifndef BUTTON__HPP
#define BUTTON__HPP

#include "MenuItem.hpp"

class Button : public MenuItem
{
public:
    Button(void);
    Button(sf::Vector2f _position, sf::Texture& _texture);
    Button(sf::Vector2f _position, sf::Texture& _texture, sf::Text& _text);
    Button(sf::Vector2f _position, sf::Texture& _texture,sf::Text& _text, std::function<void()>&& _onClickFunc, std::function<void()>&& _overFunc, std::function<void()>&& _outFunc);
    ~Button(void) override;

    void SetColor(const sf::Color& _color);
    const sf::Color& GetColor(void) const;

    void SetStringText(std::string&);
    void SetTextureSprite(const sf::Texture& _texture, const bool& _resetRect = false);

    sf::FloatRect GetRect(void) const override;

    void PollEvent(GameData& _gameData) override;

    sf::Sprite& GetSprite(void);

private:
    void InitButton(sf::Vector2f _position, sf::Texture& _texture);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Sprite sprite;
    sf::Texture* texture;
    std::unique_ptr<sf::Text> text;
};

#endif