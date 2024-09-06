#include "Definitions.hpp"
#include "Select.hpp"


SelectMenu::SelectMenu(float _x, float _y, float _width, float _height)
{
    className = "select";

    sizeBox = sf::Vector2f(_width, _height);

    menuBox.setSize(sizeBox);
    menuBox.setPosition(_x, _y);
    menuBox.setFillColor(sf::Color::White);

    currentOption = 0;
    currentOver = -1;
}

void SelectMenu::Sort()
{
    // Text in main case
    options[currentOption].setPosition(menuBox.getPosition().x + menuBox.getSize().x / 2.f, menuBox.getPosition().y + menuBox.getSize().y / 2.f);

    // Others options
    if (isOpen)
    {
        for (int i = 0; i < options.size(); i++)
        {
            // Bg options
            optionRects[i].setPosition(menuBox.getPosition().x, menuBox.getPosition().y + sizeBox.y * (i + 1));
            optionRects[i].setFillColor(sf::Color(COLOR_OPTION_UNSELECT));
            if (i == currentOption || i == currentOver)
            {
                optionRects[i].setFillColor(sf::Color(COLOR_OPTION_SELECT));
            }

            // Text options
            sf::Vector2f rectPos = optionRects[i].getPosition();
            sf::Vector2f rectSize = optionRects[i].getSize();
            options[i].setPosition(rectPos.x + rectSize.x / 2.f, rectPos.y + rectSize.y / 2.f);
        }
    }
}

void SelectMenu::AddOption(std::string _str, std::function<void()>&& _onClickFunc, bool _isCurrentOption)
{
    sf::Text toAdd = CreateText(FontsManager::Get()["DEFAULT_FONT"], _str, 50, sf::Color::Black);
    SetCenteredText(toAdd);

    // Add text
    options.push_back(toAdd);
    VerifyMaxSize((int)options.size() - 1);

    // Add rect
    sf::RectangleShape rectangleToAdd = menuBox;

    optionRects.push_back(rectangleToAdd);

    // Add function OnClick
    onClick.push_back(_onClickFunc);

    if (_isCurrentOption)
    {
        currentOption = (int)onClick.size() - 1;
    }

    Sort();
}

void SelectMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(menuBox);
    target.draw(options[currentOption]);

    // Others options
    if (isOpen)
    {
        for (int i = 0; i < options.size(); i++)
        {
            // Bg options
            target.draw(optionRects[i]);

            // Text options
            target.draw(options[i]);
        }
    }
}

void SelectMenu::VerifyMaxSize(int _optionIndex)
{
    bool hasChange = false;
    sf::FloatRect textRect = GetOptionRect(_optionIndex);

    // Si le texte dépasse la box
    float textWidth = textRect.width + SELECT_TEXT_OFFSET;
    float textHeight = textRect.height + SELECT_TEXT_OFFSET;
    if (menuBox.getGlobalBounds().width < textWidth)
    {
        sizeBox.x = textWidth;
        hasChange = true;
    }
    if (menuBox.getGlobalBounds().height < textHeight)
    {
        sizeBox.y = textHeight;
        hasChange = true;
    }

    if (hasChange)
    {
        // Set all box with the same size
        menuBox.setSize(sizeBox);
        for (int i = 0; i < optionRects.size(); i++)
        {
            optionRects[i].setSize(sizeBox);
        }
    }
}

void SelectMenu::PollEvent(GameData& _gameData)
{
    if (ControlsManager::Get().GetControllerType() == KEYBOARD)
    {
        if (_gameData.gameEvent.type == sf::Event::MouseButtonPressed)
        {
            if (_gameData.gameEvent.mouseButton.button == sf::Mouse::Left)
            {
                if (menuBox.getGlobalBounds().contains(Screen::Get().GetMousePosition(_gameData.window)))
                {
                    isOpen = !isOpen;
                    Sort();
                }
                else if (isOpen)
                {
                    for (int i = 0; i < options.size(); i++)
                    {
                        if (optionRects[i].getGlobalBounds().contains(Screen::Get().GetMousePosition(_gameData.window)))
                        {
                            currentOption = i;
                            onClick[currentOption]();
                        }
                    }
                    isOpen = false;
                    Sort();
                }
            }
        }
        if (_gameData.gameEvent.type == sf::Event::MouseMoved)
        {
            if (isOpen)
            {
                bool isIn = false;
                for (int i = 0; i < options.size(); i++)
                {
                    if (optionRects[i].getGlobalBounds().contains((float)_gameData.gameEvent.mouseMove.x, (float)_gameData.gameEvent.mouseMove.y))
                    {
                        currentOver = i;
                        isIn = true;
                        i = (int)options.size(); // break;
                    }
                }

                if (!isIn)
                {
                    currentOver = -1;
                }
            }
            else
            {
                currentOver = -1;
            }
        }
    }
}

void SelectMenu::SetCurrentOption(int _cur)
{
    currentOption = _cur;
}

std::string SelectMenu::GetCurrentOption()
{
    return options[currentOption].getString();
}

sf::FloatRect SelectMenu::GetOptionRect(int _optionIndex)
{
    sf::Text optionText;
    optionText.setString(options[_optionIndex].getString());
    optionText.setFont(FontsManager::Get()["DEFAULT_FONT"]);
    optionText.setCharacterSize(24);
    optionText.setFillColor(sf::Color::Black);
    optionText.setPosition(menuBox.getPosition().x, menuBox.getPosition().y + ((_optionIndex + 1) * optionText.getCharacterSize()));
    return optionText.getGlobalBounds();
}