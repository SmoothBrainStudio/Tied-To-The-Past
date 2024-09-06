#include "Definitions.hpp"
#include "Button.hpp"

Button::Button()
{
    text = std::make_unique<sf::Text>();
}

Button::Button(sf::Vector2f _position, sf::Texture& _texture)
{
    InitButton(_position, _texture);

    text = std::make_unique<sf::Text>();
}

Button::Button(sf::Vector2f _position, sf::Texture& _texture, sf::Text& _text)
{
    InitButton(_position, _texture);

    text = std::make_unique<sf::Text>(_text);
    sf::FloatRect sizeBox = sprite.getGlobalBounds();
    sf::FloatRect textRect = text->getGlobalBounds();
    text->setPosition(sf::Vector2f(_position.x + sizeBox.width / 2.f, _position.y + sizeBox.height / 2.f));
}

Button::Button(sf::Vector2f _position, sf::Texture& _texture,sf::Text& _text, std::function<void()>&& _onClickFunc, std::function<void()>&& _overFunc, std::function<void()>&& _outFunc)
{
    InitButton(_position, _texture);

    text = std::make_unique<sf::Text>(_text);
    sf::FloatRect sizeBox = sprite.getGlobalBounds();
    sf::FloatRect textRect = text->getGlobalBounds();
    text->setPosition(sf::Vector2f(_position.x + sizeBox.width / 2.f, _position.y + sizeBox.height / 2.f));

    SetOnClickFct((std::function<void()>)_onClickFunc);
    SetOnOverFct((std::function<void()>)_overFunc);
    SetOnOutFct((std::function<void()>)_outFunc);
}


Button::~Button()
{
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    if (visible)
    {
        target.draw(sprite, states);
        target.draw(*text, states);
    }
}

void Button::InitButton(sf::Vector2f _position, sf::Texture& _texture)
{
    className = "button";

    texture = &_texture;

    sf::Vector2u animFramesXY = sf::Vector2u(2, 1);

    sprite.setTexture(*texture);
    sf::FloatRect rectSprite = sprite.getGlobalBounds();
    setOrigin(rectSprite.width / 2.f, rectSprite.height / 2.f);
    setPosition(_position.x + rectSprite.width/2.f, _position.y + rectSprite.height/2.f);
}

void Button::SetColor(const sf::Color& _color)
{
    sprite.setColor(_color);
}

const sf::Color& Button::GetColor() const
{
    return sprite.getColor();
}

sf::FloatRect Button::GetRect() const
{
    sf::FloatRect result;
    sf::Vector2f position = getPosition();
    sf::Vector2f origin = getOrigin();

    result = sprite.getGlobalBounds();
    result.left = position.x - origin.x;
    result.top = position.y - origin.y;

    return result;
}

void Button::SetStringText(std::string& _string)
{
    text->setString(_string);
}

void Button::SetTextureSprite(const sf::Texture& _texture, const bool& _resetRect)
{
    sprite.setTexture(_texture, _resetRect);
}

void Button::PollEvent(GameData& _gameData)
{
 
        if (_gameData.gameEvent.type == sf::Event::MouseButtonPressed && ControlsManager::Get().GetControllerType() == KEYBOARD)
        {
            CallOnClick(enable && GetRect().contains(Screen::Get().GetMousePosition(_gameData.window)));
        }
        else if (_gameData.gameEvent.type == sf::Event::MouseMoved && ControlsManager::Get().GetControllerType() == KEYBOARD)
        {
            if (enable && visible)
            {
                bool IsOverButton = GetRect().contains(Screen::Get().GetMousePosition(_gameData.window));
                if (IsOverButton && !wasOver)
                {
                    _gameData.window.setMouseCursor(_gameData.cursor);
                    Audio::Get().PlaySoundNoSecurity("BUTTON_SWAP");
                    wasOver = true;
                    CallOnOver();
                }
                else if (!IsOverButton && wasOver)
                {
                    _gameData.window.setMouseCursor(_gameData.cursorGrey);
                    wasOver = false;
                    CallOnOut();
                }
            }
        }
        else if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "INTERACT")
            || ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "INTERACT_2"))
        {
            CallOnClick(enable && wasOver);
        }
}

sf::Sprite& Button::GetSprite(void)
{
    return sprite;
}
