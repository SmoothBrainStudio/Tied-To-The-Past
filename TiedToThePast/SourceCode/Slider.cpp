#include "Definitions.hpp"
#include "Slider.hpp"

Slider::Slider(const sf::Vector2f& _position, const sf::Vector2f& _size, int _value, const sf::Color& _backgroundColor, const sf::Color& _fillColor, const sf::Color& _buttonColor)
{
    this->className = "select";

    this->setPosition(_position);
    this->SetValue((float)_value);

    this->sliderBar.setFillColor(_backgroundColor);
    this->sliderFill.setFillColor(_fillColor);
    this->sliderButton.setFillColor(_buttonColor);
    this->buttonColor = _buttonColor;

    this->sliderBar.setSize(_size);
    this->sliderButton.setSize({ _size.y, _size.y });
}

void Slider::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(this->sliderBar, states);
    target.draw(this->sliderFill, states);
    target.draw(this->sliderButton, states);
}

void Slider::PollEvent(GameData& _gameData)
{
    if (ControlsManager::Get().GetControllerType() == KEYBOARD)
    {
        if (_gameData.gameEvent.type == sf::Event::KeyPressed)
        {
            if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "LEFT"))
            {
                if (this->value - this->increment >= this->minValue)
                {
                    SetValue((float)(this->value - this->increment));
                }
            }
            if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "RIGHT"))
            {
                if (this->value + this->increment <= this->maxValue)
                {
                    SetValue((float)(this->value + this->increment));
                }
            }
        }

        if (_gameData.gameEvent.type == sf::Event::MouseButtonPressed)
        {
            if (_gameData.gameEvent.mouseButton.button == sf::Mouse::Left)
            {
                if (this->sliderButton.getGlobalBounds().contains(Screen::Get().GetMousePosition(_gameData.window)))
                {
                    this->isDragging = true;
                    this->sliderButton.setFillColor(this->buttonColor + sf::Color(60, 60, 60));
                }
            }
        }
        if (_gameData.gameEvent.type == sf::Event::MouseMoved)
        {
            if (this->isDragging)
            {
                _gameData.window.setMouseCursor(_gameData.cursor);
                float x = (float)_gameData.gameEvent.mouseMove.x;

                sf::Vector2f position = this->sliderBar.getPosition();
                sf::Vector2f size = this->sliderBar.getSize();

                SetValue(static_cast<float>((x - position.x - size.y / 2.f) / (size.x - size.y) * (this->maxValue - this->minValue)));
            }
        }
        if (_gameData.gameEvent.type == sf::Event::MouseButtonReleased)
        {
            if (_gameData.gameEvent.mouseButton.button == sf::Mouse::Left)
            {
                _gameData.window.setMouseCursor(_gameData.cursorGrey);
                this->isDragging = false;
                this->sliderButton.setFillColor(this->buttonColor);
            }
        }
    }
}

int Slider::GetValue()
{
    return this->value;
}

void Slider::SetValue(float val)
{
    if (val > this->maxValue) val = (float)maxValue;
    if (val < this->minValue) val = (float)minValue;

    this->value = (int)val;

    float widthSliderBar = this->sliderBar.getSize().x;
    float widthCursor = this->sliderButton.getSize().x;
    float ind = (val / (this->maxValue - this->minValue));

    float x = this->sliderBar.getPosition().x + ind * (widthSliderBar - widthCursor);
    this->sliderButton.setPosition(x, this->sliderButton.getPosition().y);
    this->sliderFill.setSize(sf::Vector2f(ind * (widthSliderBar - widthCursor), this->sliderButton.getSize().y));
}

void Slider::SetMinValue(float val)
{
    this->minValue = (int)val;
}

void Slider::SetMaxValue(float val)
{
    this->maxValue = (int)val;
}

void Slider::SetIncrement(float val)
{
    this->increment = (int)val;
}
