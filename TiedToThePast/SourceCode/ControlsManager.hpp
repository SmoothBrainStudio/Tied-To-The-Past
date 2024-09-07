#ifndef CONTROLS__MANAGER__HPP
#define CONTROLS__MANAGER__HPP

#include "Common.hpp"
//Copyright 2024 Property of Olivier Maurin.All rights reserved.

static constexpr float JOYSTICK_MIN_STEP = 50.f;
static constexpr float JOYSTICK_MAX_STEP = 75.f;
static constexpr float JOYSTICK_IDLE_STEP = 10.f;
static constexpr unsigned short int JOY_CTRL_TOUCH = 6;
static constexpr float TIMER_MAX_JOYSTICK_MOVE = 0.18f;
constexpr static const char* PLAYSTATION_TEXTURE_KEY = "CONTROLLER_PS_";
constexpr static const char* XBOX_TEXTURE_KEY = "CONTROLLER_XBOX_";

enum ControllerType
{
	KEYBOARD,
	CONTROLLER
};

enum ControllerButton
{
	NONE = -1,
	A_CROSS,
	B_CIRCLE,
	X_SQUARE,
	Y_TRIANGLE,
	LB_L1,
	RB_R1,
	SELECT,
	START,
	LEFT_STICK_PUSH,
	RIGHT_STICK_PUSH,
	COUNT
};

enum ControllerAxis
{
	LEFT_JOYSTICK_X,
	LEFT_JOYSTICK_Y,
	TRIGGERS_L2_R2,
	V,                         // WHAT IS THE V AXIS ? NOT SURE... metal
	RIGHT_JOYSTICK_X,
	RIGHT_JOYSTICK_Y,
	DIR_CROSS_X,
	DIR_CROSS_Y
};

class ControlsManager
{
public:
	struct Input
	{
		std::string title;
		sf::Keyboard::Key key = sf::Keyboard::Key::Unknown;
		sf::Keyboard::Key defaultKey = sf::Keyboard::Key::Unknown;
		ControllerButton button = ControllerButton::NONE;
		ControllerButton defaultButton = ControllerButton::NONE;
		bool isKeyboardOnly = false;
		bool isControllerOnly = false;
	};

	~ControlsManager(void) = default;
	ControlsManager(const ControlsManager&) = delete;
	ControlsManager(ControlsManager&&) = delete;
	ControlsManager& operator=(const ControlsManager&) = delete;
	ControlsManager& operator=(ControlsManager&&) = delete;

	static ControlsManager& Get(void);

	void Update(void);

	const unsigned char& GetControllerType(void);
	const sf::Vector2f GetJoystickLeftAxis(void);
	const bool CanJoystickInteract(void);
	sf::Texture& GetTexture(const std::string& _playerInput);
	const char& GetControllerID(void) const;

	void SetKeyboardTouch(const std::string& _playerInput, sf::Keyboard::Key _key);
	void SetControllerButton(const std::string& _playerInput, ControllerButton _button);
	void ResetInputs(void);
	
#pragma region Event functions
	// Is keyboard key or controller button held
	const bool IsHeldInput(const std::string& _playerInput);
	// Is keyboard key or controller button pressed
	const bool IsPressedInput(sf::Event& _event, const std::string& _playerInput);
	// Is keyboard key released
	const bool IsReleasedInput(sf::Event& _event, const std::string& _playerInput);
#pragma endregion Event functions

#pragma region Update functions
	// Is controller cross pressed or left joystick moved
	const bool IsPressedDirection(const std::string& _playerInput);
	// Is controller left trigger is pressed
	const bool IsPressedTriggerLeft(void);
	// Is controller right trigger is pressed
	const bool IsPressedTriggerRight(void);

#pragma endregion Update functions

	bool HasInputChanged();
	bool IsAnyButtonPressed();
	const std::unordered_map<std::string, Input>& GetInputs() const;

private:
	ControlsManager();

	void IsControllerConnected(void);
	void UpdateJoystickAxis(void);
	int GetInputIndex(const std::string& _playerInput);
	sf::Keyboard::Key GetKey(const std::string& _name);
	ControllerButton GetControllerButton(const std::string& _name);

	std::unordered_map<std::string, Input> inputs;

	char controllerID = -1;
	unsigned char controllerType;
	sf::Vector2f leftJoystickAxis;
	float timerJoystickMove;
	bool hasInputChanged = false;
	bool buttonAlreadyPressed = false;

	constexpr static const char* KEY_NAMES[] =
	{
		"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
		"Num0", "Num1", "Num2", "Num3", "Num4", "Num5", "Num6", "Num7", "Num8", "Num9",
		"Escape", "LControl", "LShift", "LAlt", "LSystem", "RControl", "RShift", "RAlt", "RSystem", "Menu", "LBracket", "RBracket",
		"SemiColon", "Comma", "Period", "Quote", "Slash", "Backslash", "Tilde", "Equal", "Hyphen", "Space", "Return", "Backspace", "Tab",
		"PageUp", "PageDown", "End", "Home", "Insert", "Delete", "Add", "Substract", "Multiply", "Divide", "Left", "Right", "Up", "Down",
		"Numpad0", "Numpad1", "Numpad2", "Numpad3", "Numpad4", "Numpad5", "Numpad6", "Numpad7", "Numpad8", "Numpad9",
		"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "F13", "F14", "F15", "Pause"
	};

	constexpr static const char* CONTROLLER_BUTTON_NAMES[] =
	{
		"A_CROSS",
		"B_CIRCLE",
		"X_SQUARE",
		"Y_TRIANGLE",
		"LB_L1",
		"RB_R1",
		"SELECT",
		"START",
		"LEFT_STICK_PUSH",
		"RIGHT_STICK_PUSH"
	};
};

#endif // !CONTROLS__MANAGER__HPP