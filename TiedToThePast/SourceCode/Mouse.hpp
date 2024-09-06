#ifndef MOUSE__HPP
#define MOUSE__HPP

#include "Definitions.hpp"

class Mouse
{
public:
	~Mouse(void) = default;

	static Mouse& GetInstance(void);

	void Update(const sf::RenderWindow& _window);

	const sf::Vector2f& GetPos(void) const;
	const sf::Vector2f& GetVelocity(void) const;

	/// <summary>
	/// Check if button was pressed in the current frame
	/// </summary>
	/// <param name="_button"> : button to check</param>
	/// <returns>True if the button was pressed, false otherwise</returns>
	const bool IsButtonPressed(const sf::Mouse::Button& _button) const;

	/// <summary>
	/// Check if button was released in the current frame
	/// </summary>
	/// <param name="_button"> : button to check</param>
	/// <returns>True if the button was released, false otherwise</returns>
	const bool IsButtonReleased(const sf::Mouse::Button& _button) const;

	/// <summary>
	/// Check if button was held in the current frame.
	/// This function do the same thing as sf::Mouse::isButtonPressed
	/// but for the sake of readability & optimisation we use this one.
	/// </summary>
	/// <param name="_button"> : button to check</param>
	/// <returns>True if the button was held, false otherwise</returns>
	const bool& IsButtonHeld(const sf::Mouse::Button& _button) const;

private:

	Mouse(void) = default;

	std::array<bool, sf::Mouse::ButtonCount> currentButtonState;
	std::array<bool, sf::Mouse::ButtonCount> previousButtonState;

	sf::Vector2f pos = sf::Vector2f(0.f, 0.f);
	sf::Vector2f velocity = sf::Vector2f(0.f, 0.f);
};

#endif //!MOUSE__HPP