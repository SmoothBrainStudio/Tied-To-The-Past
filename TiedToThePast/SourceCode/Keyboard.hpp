#ifndef KEY_BOARD__HPP
#define KEY_BOARD__HPP

#include "Definitions.hpp"

class Keyboard
{
public:
	~Keyboard() = default;

	static Keyboard& Get(void);

	/// <summary>
	/// Update all the key state in the current and previous frame
	/// </summary>
	void Update(void);

	void Clear(void);

	/// <summary>
	/// Check if key was pressed in the current frame
	/// </summary>
	/// <param name="_key"> : Key to check</param>
	/// <returns>True if the key was pressed, false otherwise</returns>
	 const bool IsKeyPressed(sf::Keyboard::Key _key) const;

	/// <summary>
	/// Check if key was released in the current frame
	/// </summary>
	/// <param name="_key"> : Key to check</param>
	/// <returns>True if the key was released, false otherwise</returns>
	 const bool IsKeyReleased(sf::Keyboard::Key _key) const;

	/// <summary>
	/// Check if key was held in the current frame.
	/// This function do the same thing as sf::KeyBoard::isKeyPressed
	/// but for the sake of readability we use this one.
	/// </summary>
	/// <param name="_key"> : Key to check</param>
	/// <returns>True if the key was held, false otherwise</returns>
	 const bool& IsKeyHeld(sf::Keyboard::Key _key) const;

	 const bool IsAnyKeyPressed() const;

	 const bool IsAnyKeyReleased() const;

	 const bool IsAnyKeyHeld() const;

private:

	Keyboard() = default;

	std::array<bool, sf::Keyboard::KeyCount> currentKeyState; // curent frame key state
	std::array<bool, sf::Keyboard::KeyCount> previousKeyState; // previous frame key state
};

#endif // !KEY_BOARD__HPP