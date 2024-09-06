#ifndef SELECTBOX_HPP
#define SELECTBOX_HPP

#include "MenuItem.hpp"
#include "Text.hpp"

#define MARGIN_BOX_TOPDOWN 24
#define MARGIN_BOX_LEFTRIGHT 128

template <typename T>
class SelectBox : public MenuItem
{
public:
    SelectBox();
    ~SelectBox() override = default;

    void AddOption(const std::string& _content, const T& _value);
    void Update(GameData& _gameData) override;
    void PollEvent(GameData& _gameData) override;

    void Next(void);
    void Previous(void);

    void SetBackTexture(const sf::Texture& _texture);
    void SetBorderTexture(const sf::Texture& _texture);
    void SetValueByCurValue(const T& _val);

    T GetValue();
    sf::FloatRect GetRect() const override;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void ReplaceEls();

    sf::Sprite back;
    sf::Sprite border;

    sf::Text displayText;
    std::vector<std::string> options;
    std::vector<T> values;

    char curOption = 0;
    char lastOption;

    std::array<sf::Sprite, 2> arrows;

};

template <typename T>
SelectBox<T>::SelectBox()
{
    className = "select";

    arrows[0] = CreateSprite(TexturesManager::Get()["LEFT_ARROW"]);
    arrows[1] = CreateSprite(TexturesManager::Get()["RIGHT_ARROW"]);

    sf::FloatRect arrowRect = arrows[0].getGlobalBounds();
    arrows[0].setOrigin(0, arrowRect.height / 2.f);
    arrows[1].setOrigin(arrowRect.width, arrowRect.height / 2.f);

    displayText = CreateText(FontsManager::Get()["SPOOKY"], "", 42);
    SetOutlined(displayText, 2);
}

template <typename T>
void SelectBox<T>::AddOption(const std::string& _content, const T& _value)
{
    // Add option
    options.push_back(_content);
    values.push_back(_value);
}

template <typename T>
void SelectBox<T>::Update(GameData& _gameData)
{
    displayText.setString(options[curOption]);
    SetCenteredText(displayText);

    CallOnChange(lastOption != curOption);
    lastOption = curOption;
}

template <typename T>
void SelectBox<T>::PollEvent(GameData& _gameData)
{
    if (ControlsManager::Get().GetControllerType() == KEYBOARD)
    {
        if (_gameData.gameEvent.type == sf::Event::MouseButtonPressed && sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2f mousePos = Screen::Get().GetMousePosition(_gameData.window);
            sf::FloatRect leftArrowGB = arrows[0].getGlobalBounds();
            sf::FloatRect rightArrowGB = arrows[1].getGlobalBounds();

            if (leftArrowGB.contains(mousePos - getPosition()))
            {
                Previous();
            }

            if (rightArrowGB.contains(mousePos - getPosition()))
            {
                Next();
            }
        }
        if (_gameData.gameEvent.type == sf::Event::MouseMoved)
        {
            if (enable && visible)
            {
                bool IsOverButton = GetRect().contains(Screen::Get().GetMousePosition(_gameData.window));
                if (IsOverButton && !wasOver)
                {
                    _gameData.window.setMouseCursor(_gameData.cursor);
                    //Audio::Get().PlaySoundNoSecurity("BUTTON_SWAP");
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
    }
}

template <typename T>
void SelectBox<T>::Next(void)
{
    if (++curOption > options.size() - 1)
    {
        curOption = 0;
    }
}

template <typename T>
void SelectBox<T>::Previous(void)
{
    if (--curOption < 0)
    {
        curOption = (char)(options.size() - 1);
    }
}

template <typename T>
void SelectBox<T>::SetBackTexture(const sf::Texture& _texture)
{
    back.setTexture(_texture, true);

    ReplaceEls();
}

template <typename T>
void SelectBox<T>::SetBorderTexture(const sf::Texture& _texture)
{
    border.setTexture(_texture, true);
}

template<typename T>
inline void SelectBox<T>::SetValueByCurValue(const T& _val)
{
    for (int i = 0; i < values.size(); i++)
    {
        if (values[i] == _val)
        {
            curOption = i;
            break;
        }
    }
}

template <typename T>
T SelectBox<T>::GetValue()
{
    return values[curOption];
}

template <typename T>
sf::FloatRect SelectBox<T>::GetRect() const
{
    sf::Vector2f pos = getPosition();
    sf::FloatRect rect = back.getGlobalBounds();
    rect.left = pos.x;
    rect.top = pos.y;

    return rect;
}


template <typename T>
void SelectBox<T>::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(back, states);
    target.draw(border, states);

    target.draw(displayText, states);

    target.draw(arrows[0], states);
    target.draw(arrows[1], states);
}

template <typename T>
void SelectBox<T>::ReplaceEls()
{
    sf::FloatRect rect = GetRect();
    displayText.setPosition(rect.width / 2.f, rect.height / 2.f);

    arrows[0].setPosition(50, rect.height / 2.f);
    arrows[1].setPosition(rect.width - 50, rect.height / 2.f);
}

#endif // !SELECTBOX_HPP
