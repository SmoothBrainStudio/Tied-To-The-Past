#include "Definitions.hpp"
#include "Keyboard.hpp"


Keyboard& Keyboard::Get(void)
{
    static Keyboard instance;
    return instance;
}

void Keyboard::Update(void)
{
    for (unsigned short int i = 0; i < sf::Keyboard::KeyCount; i++)
    {
        // Save the keyboard's state from the previous frame
        previousKeyState[i] = currentKeyState[i];

        // Save the keyboard's state in the current frame
        currentKeyState[i] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key(i));
    }
}

void Keyboard::Clear(void)
{
    for (unsigned short int i = 0; i < sf::Keyboard::KeyCount; i++)
    {
        previousKeyState[i] = false;

        currentKeyState[i] = false;
    }
}

const bool Keyboard::IsKeyPressed(sf::Keyboard::Key _key) const
{
    return (currentKeyState[_key] && !previousKeyState[_key]);
}

const bool Keyboard::IsKeyReleased(sf::Keyboard::Key _key) const
{
    return (!currentKeyState[_key] && previousKeyState[_key]);
}

const bool& Keyboard::IsKeyHeld(sf::Keyboard::Key _key) const
{
    return currentKeyState[_key];
}

const bool Keyboard::IsAnyKeyPressed() const
{
    for (unsigned short int i = 0; i < sf::Keyboard::KeyCount; i++)
    {
        if (IsKeyPressed(sf::Keyboard::Key(i)))
        {
            return true;
        }
    }

    return false;
}

const bool Keyboard::IsAnyKeyReleased() const
{
    for (unsigned short int i = 0; i < sf::Keyboard::KeyCount; i++)
    {
        if (IsKeyReleased(sf::Keyboard::Key(i)))
        {
            return true;
        }
    }

    return false;
}

const bool Keyboard::IsAnyKeyHeld() const
{
    for (unsigned short int i = 0; i < sf::Keyboard::KeyCount; i++)
    {
        if (IsKeyHeld(sf::Keyboard::Key(i)))
        {
            return true;
        }
    }
    
    return false;
}