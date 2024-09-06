#include "Definitions.hpp"
#include "Mouse.hpp"


Mouse& Mouse::GetInstance(void)
{
	static Mouse instance;
	return instance;
}

void Mouse::Update(const sf::RenderWindow& _window)
{
	velocity = (sf::Vector2f)sf::Mouse::getPosition(_window) - pos;
	pos = (sf::Vector2f)sf::Mouse::getPosition(_window);

	for (unsigned char i = 0; i < sf::Mouse::ButtonCount; i++)
	{
		// Save the button's state from the previous frame
		previousButtonState[i] = currentButtonState[i];

		// Save the button's state in the current frame
		currentButtonState[i] = sf::Mouse::isButtonPressed(sf::Mouse::Button(i));
	}
}

const sf::Vector2f& Mouse::GetPos(void) const
{
	return pos;
}

const sf::Vector2f& Mouse::GetVelocity(void) const
{
	return velocity;
}

const bool Mouse::IsButtonPressed(const sf::Mouse::Button& _button) const
{
	return (currentButtonState[_button] && !previousButtonState[_button]);
}

const bool Mouse::IsButtonReleased(const sf::Mouse::Button& _button) const
{
	return (!currentButtonState[_button] && previousButtonState[_button]);
}

const bool& Mouse::IsButtonHeld(const sf::Mouse::Button& _button) const
{
	return currentButtonState[_button];
}