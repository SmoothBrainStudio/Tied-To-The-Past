#include "Definitions.hpp"
#include "KeyInput.hpp"

KeyInput::KeyInput(const sf::Vector2f& _position, const sf::Vector2f& _size, const sf::Color& _backgroundColor)
{
    this->setPosition(_position);

    this->rect.setSize(_size);
    this->rect.setFillColor(_backgroundColor);
    this->rectColor = _backgroundColor;

    this->text.setCharacterSize(24);
    this->text.setFillColor(sf::Color::Black);
    this->text.setString("machin");
    this->text.setFont(FontsManager::Get()["DEFAULT"]);
    sf::FloatRect textLocalBounds = this->text.getLocalBounds();
    this->text.setPosition(_size.x * 1.25f, -roundf(textLocalBounds.top + textLocalBounds.height / 2.f) + _size.y * 0.5f);

    this->className = "keyinput";
}

void KeyInput::PollEvent(GameData& _gameData)
{
    if (ControlsManager::Get().GetControllerType() == KEYBOARD)
    {
        if (_gameData.gameEvent.type == sf::Event::MouseButtonPressed)
        {
            if (this->getTransform().transformRect(this->rect.getGlobalBounds()).contains(Screen::Get().GetMousePosition(_gameData.window)) && !isClicked)
            {
                this->isClicked = true;
                this->rect.setFillColor(this->rectColor + sf::Color(60, 60, 60));
            }
            else if (this->isClicked)
            {
                ChangeInputKey(_gameData.gameEvent);
            }
        }

        if (_gameData.gameEvent.type == sf::Event::KeyPressed && this->isClicked)
        {
            ChangeInputKey(_gameData.gameEvent);
        }
    }
}

void KeyInput::ChangeInputKey(sf::Event _event)
{
    this->key = GetPressedKey(_event);
    if (!this->key.empty() && this->isClicked)
    {
        std::string path = "Contents/Assets/Sprites/UI/Inputs/KeyboardMouse/Dark/" + this->key + ".png";
        this->isClicked = false;

        TexturesManager& textureManager = TexturesManager::Get();
        textureManager.Add(path, this->key);
        const sf::Texture& keyTexture = textureManager[this->key];
        this->keySprite.setTexture(keyTexture);

        sf::Vector2u textureSize = keyTexture.getSize();
        const sf::Vector2f& rectSize = this->rect.getSize();
        this->keySprite.setScale(rectSize.x / textureSize.x, rectSize.y / textureSize.y);

        this->rect.setFillColor(this->rectColor);
    }
}

void KeyInput::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= this->getTransform();
    target.draw(this->rect, states);
    target.draw(this->text, states);
    target.draw(this->keySprite, states);
}

std::string GetPressedKey(sf::Event _event)
{
    if (_event.type == sf::Event::KeyPressed)
    {
        if (_event.key.code == sf::Keyboard::A) return "A";
        if (_event.key.code == sf::Keyboard::B) return "B";
        if (_event.key.code == sf::Keyboard::C) return "C";
        if (_event.key.code == sf::Keyboard::D) return "D";
        if (_event.key.code == sf::Keyboard::E) return "E";
        if (_event.key.code == sf::Keyboard::F) return "F";
        if (_event.key.code == sf::Keyboard::G) return "G";
        if (_event.key.code == sf::Keyboard::H) return "H";
        if (_event.key.code == sf::Keyboard::I) return "I";
        if (_event.key.code == sf::Keyboard::J) return "J";
        if (_event.key.code == sf::Keyboard::K) return "K";
        if (_event.key.code == sf::Keyboard::L) return "L";
        if (_event.key.code == sf::Keyboard::M) return "M";
        if (_event.key.code == sf::Keyboard::N) return "N";
        if (_event.key.code == sf::Keyboard::O) return "O";
        if (_event.key.code == sf::Keyboard::P) return "P";
        if (_event.key.code == sf::Keyboard::Q) return "Q";
        if (_event.key.code == sf::Keyboard::R) return "R";
        if (_event.key.code == sf::Keyboard::S) return "S";
        if (_event.key.code == sf::Keyboard::T) return "T";
        if (_event.key.code == sf::Keyboard::U) return "U";
        if (_event.key.code == sf::Keyboard::V) return "V";
        if (_event.key.code == sf::Keyboard::W) return "W";
        if (_event.key.code == sf::Keyboard::X) return "X";
        if (_event.key.code == sf::Keyboard::Y) return "Y";
        if (_event.key.code == sf::Keyboard::Z) return "Z";
        if (_event.key.code == sf::Keyboard::Num0) return "Num0";
        if (_event.key.code == sf::Keyboard::Num1) return "Num1";
        if (_event.key.code == sf::Keyboard::Num2) return "Num2";
        if (_event.key.code == sf::Keyboard::Num3) return "Num3";
        if (_event.key.code == sf::Keyboard::Num4) return "Num4";
        if (_event.key.code == sf::Keyboard::Num5) return "Num5";
        if (_event.key.code == sf::Keyboard::Num6) return "Num6";
        if (_event.key.code == sf::Keyboard::Num7) return "Num7";
        if (_event.key.code == sf::Keyboard::Num8) return "Num8";
        if (_event.key.code == sf::Keyboard::Num9) return "Num9";
        if (_event.key.code == sf::Keyboard::Escape) return "Escape";
        if (_event.key.code == sf::Keyboard::LControl) return "LControl";
        if (_event.key.code == sf::Keyboard::LShift) return "LShift";
        if (_event.key.code == sf::Keyboard::LAlt) return "LAlt";
        if (_event.key.code == sf::Keyboard::LSystem) return "LSystem";
        if (_event.key.code == sf::Keyboard::RControl) return "RControl";
        if (_event.key.code == sf::Keyboard::RShift) return "RShift";
        if (_event.key.code == sf::Keyboard::RAlt) return "RAlt";
        if (_event.key.code == sf::Keyboard::RSystem) return "RSystem";
        if (_event.key.code == sf::Keyboard::Menu) return "Menu";
        if (_event.key.code == sf::Keyboard::LBracket) return "LBracket";
        if (_event.key.code == sf::Keyboard::RBracket) return "RBracket";
        if (_event.key.code == sf::Keyboard::SemiColon) return "SemiColon";
        if (_event.key.code == sf::Keyboard::Comma) return "Comma";
        if (_event.key.code == sf::Keyboard::Period) return "Period";
        if (_event.key.code == sf::Keyboard::Quote) return "Quote";
        if (_event.key.code == sf::Keyboard::Slash) return "Slash";
        if (_event.key.code == sf::Keyboard::BackSlash) return "BackSlash";
        if (_event.key.code == sf::Keyboard::Tilde) return "Tilde";
        if (_event.key.code == sf::Keyboard::Equal) return "Equal";
        if (_event.key.code == sf::Keyboard::Dash) return "Dash";
        if (_event.key.code == sf::Keyboard::Space) return "Space";
        if (_event.key.code == sf::Keyboard::Return) return "Return";
        if (_event.key.code == sf::Keyboard::BackSpace) return "BackSpace";
        if (_event.key.code == sf::Keyboard::Tab) return "Tab";
        if (_event.key.code == sf::Keyboard::PageUp) return "PageUp";
        if (_event.key.code == sf::Keyboard::PageDown) return "PageDown";
        if (_event.key.code == sf::Keyboard::End) return "End";
        if (_event.key.code == sf::Keyboard::Home) return "Home";
        if (_event.key.code == sf::Keyboard::Insert) return "Insert";
        if (_event.key.code == sf::Keyboard::Delete) return "Delete";
        if (_event.key.code == sf::Keyboard::Add) return "Add";
        if (_event.key.code == sf::Keyboard::Subtract) return "Subtract";
        if (_event.key.code == sf::Keyboard::Multiply) return "Multiply";
        if (_event.key.code == sf::Keyboard::Divide) return "Divide";
        if (_event.key.code == sf::Keyboard::Left) return "Left";
        if (_event.key.code == sf::Keyboard::Right) return "Right";
        if (_event.key.code == sf::Keyboard::Up) return "Up";
        if (_event.key.code == sf::Keyboard::Down) return "Down";
        if (_event.key.code == sf::Keyboard::Numpad0) return "Numpad0";
        if (_event.key.code == sf::Keyboard::Numpad1) return "Numpad1";
        if (_event.key.code == sf::Keyboard::Numpad2) return "Numpad2";
        if (_event.key.code == sf::Keyboard::Numpad3) return "Numpad3";
        if (_event.key.code == sf::Keyboard::Numpad4) return "Numpad4";
        if (_event.key.code == sf::Keyboard::Numpad5) return "Numpad5";
        if (_event.key.code == sf::Keyboard::Numpad6) return "Numpad6";
        if (_event.key.code == sf::Keyboard::Numpad7) return "Numpad7";
        if (_event.key.code == sf::Keyboard::Numpad8) return "Numpad8";
        if (_event.key.code == sf::Keyboard::Numpad9) return "Numpad9";
        if (_event.key.code == sf::Keyboard::F1) return "F1";
        if (_event.key.code == sf::Keyboard::F2) return "F2";
        if (_event.key.code == sf::Keyboard::F3) return "F3";
        if (_event.key.code == sf::Keyboard::F4) return "F4";
        if (_event.key.code == sf::Keyboard::F5) return "F5";
        if (_event.key.code == sf::Keyboard::F6) return "F6";
        if (_event.key.code == sf::Keyboard::F7) return "F7";
        if (_event.key.code == sf::Keyboard::F8) return "F8";
        if (_event.key.code == sf::Keyboard::F9) return "F9";
        if (_event.key.code == sf::Keyboard::F10) return "F10";
        if (_event.key.code == sf::Keyboard::F11) return "F11";
        if (_event.key.code == sf::Keyboard::F12) return "F12";
        if (_event.key.code == sf::Keyboard::F13) return "F13";
        if (_event.key.code == sf::Keyboard::F14) return "F14";
        if (_event.key.code == sf::Keyboard::F15) return "F15";
    }
    if (_event.type == sf::Event::MouseButtonPressed)
    {
        if (_event.mouseButton.button == sf::Mouse::Left) return "Mouse_Left";
        if (_event.mouseButton.button == sf::Mouse::Right) return "Mouse_Right";
        if (_event.mouseButton.button == sf::Mouse::Middle) return "Mouse_Middle";
    }
    return "";
}