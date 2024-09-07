#include "ControlsManager.hpp"
//Copyright 2024 Property of Olivier Maurin.All rights reserved.

ControlsManager& ControlsManager::Get(void)
{
	static ControlsManager myInstance;
	return myInstance;
}

ControlsManager::ControlsManager(void)
{
	JsonManager::Get().Add("Contents/Data/Controls.json", "CONTROLS");
	Json::Value& json = JsonManager::Get()["CONTROLS"];
	for (Json::Value& inputJson : json)
	{
		Input input;
		input.title = inputJson["Title"].asString();
		input.key = GetKey(inputJson["Key"].asString());
		input.defaultKey = GetKey(inputJson["DefaultKey"].asString());
		input.button = GetControllerButton(inputJson["ControllerButton"].asString());
		input.defaultButton = GetControllerButton(inputJson["DefaultControllerButton"].asString());
		input.isKeyboardOnly = inputJson["KeyboardOnly"].asBool();
		input.isControllerOnly = inputJson["ControllerOnly"].asBool();
		inputs.insert({ inputJson["Name"].asString(), input });
	}

	controllerID = 0;
	leftJoystickAxis = { 0, 0 };
	if (sf::Joystick::isConnected(controllerID))
	{
		controllerType = CONTROLLER;
	}
	else
	{
		controllerType = KEYBOARD;
	}

	timerJoystickMove = 0.f;

	TexturesManager::Get().Add("Contents/Assets/Sprites/UI/Inputs/KeyboardMouse/Dark/None.png", "KEY_NONE");
	for (sf::Uint32 i = 0; i < sf::Keyboard::KeyCount; ++i)
	{
		TexturesManager::Get().Add("Contents/Assets/Sprites/UI/Inputs/KeyboardMouse/Dark/" + std::string(KEY_NAMES[i]) + ".png", "KEY_" + std::string(KEY_NAMES[i]));
	}

	for (sf::Uint32 i = 0; i < ControllerButton::COUNT; ++i)
	{
		TexturesManager::Get().Add("Contents/Assets/Sprites/UI/Inputs/XBOX_ONE/" + std::string(CONTROLLER_BUTTON_NAMES[i]) + ".png", std::string(XBOX_TEXTURE_KEY) + std::string(CONTROLLER_BUTTON_NAMES[i]));
	}

	for (sf::Uint32 i = 0; i < ControllerButton::COUNT; ++i)
	{
		TexturesManager::Get().Add("Contents/Assets/Sprites/UI/Inputs/PS5/" + std::string(CONTROLLER_BUTTON_NAMES[i]) + ".png", std::string(PLAYSTATION_TEXTURE_KEY) + std::string(CONTROLLER_BUTTON_NAMES[i]));
	}
}

void ControlsManager::Update(void)
{
	IsControllerConnected();
	UpdateJoystickAxis();
	if (timerJoystickMove >= 0.f)
	{
		timerJoystickMove -= GameClock::Get().GetDeltaTime();
	}
}

// Check if controller is connected
void ControlsManager::IsControllerConnected(void)
{
	if (sf::Joystick::isConnected(controllerID))
	{

		controllerType = CONTROLLER;
	}
	else
	{
		controllerType = KEYBOARD;
	}
}

sf::Keyboard::Key ControlsManager::GetKey(const std::string& _name)
{
	for (size_t i = 0; i < sf::Keyboard::KeyCount; ++i)
	{
		if (KEY_NAMES[i] == _name)
		{
			return sf::Keyboard::Key(i);
		}
	}
	return sf::Keyboard::Unknown;
}

ControllerButton ControlsManager::GetControllerButton(const std::string& _name)
{
	for (size_t i = 0; i < ControllerButton::COUNT; ++i)
	{
		if (CONTROLLER_BUTTON_NAMES[i] == _name)
		{
			return ControllerButton(i);
		}
	}
	return ControllerButton::NONE;
}

void ControlsManager::UpdateJoystickAxis(void)
{
	leftJoystickAxis = sf::Vector2f(sf::Joystick::getAxisPosition(controllerID, sf::Joystick::Axis::X),
		sf::Joystick::getAxisPosition(controllerID, sf::Joystick::Axis::Y));
}

int ControlsManager::GetInputIndex(const std::string& _playerInput)
{
	Json::Value& json = JsonManager::Get()["CONTROLS"];
	for (int i = 0; i < static_cast<int>(json.size()); ++i)
	{
		if (json[i]["Name"] == _playerInput)
		{
			return i;
		}
	}
	return -1;
}

const unsigned char& ControlsManager::GetControllerType(void)
{
	return controllerType;
}

const sf::Vector2f ControlsManager::GetJoystickLeftAxis(void)
{
	if (CanJoystickInteract())
	{
		return leftJoystickAxis;
	}
	return sf::Vector2f(0.f, 0.f);
}

void ControlsManager::SetKeyboardTouch(const std::string& _playerInput, sf::Keyboard::Key _key)
{
	Json::Value& json = JsonManager::Get()["CONTROLS"];
	int inputIndex = GetInputIndex(_playerInput);
	for (auto& [key, input] : inputs)
	{
		if (input.key == _key)
		{
			input.key = sf::Keyboard::Unknown;
			json[inputIndex]["Key"] = "";
			break;
		}
	}
	inputs[_playerInput].key = _key;
	hasInputChanged = true;
	if (_key == sf::Keyboard::Unknown)
	{
		json[inputIndex]["Key"] = "";
	}
	else
	{
		json[inputIndex]["Key"] = KEY_NAMES[_key];
	}
	JsonManager::Get().SaveToFile("CONTROLS");
}

void ControlsManager::SetControllerButton(const std::string& _playerInput, ControllerButton _button)
{
	Json::Value& json = JsonManager::Get()["CONTROLS"];
	int inputIndex = GetInputIndex(_playerInput);
	for (auto& [key, input] : inputs)
	{
		if (input.button == _button)
		{
			input.button = ControllerButton::NONE;
			json[inputIndex]["ControllerButton"] = "";
			break;
		}
	}
	inputs[_playerInput].button = _button;
	hasInputChanged = true;
	if (_button == ControllerButton::NONE)
	{
		json[inputIndex]["ControllerButton"] = "";
	}
	else
	{
		json[inputIndex]["ControllerButton"] = CONTROLLER_BUTTON_NAMES[_button];
	}
	JsonManager::Get().SaveToFile("CONTROLS");
}

void ControlsManager::ResetInputs()
{
	for (auto& [key, value] : inputs)
	{
		inputs[key].key = inputs[key].defaultKey;
		inputs[key].button = inputs[key].defaultButton;
	}
	Json::Value& json = JsonManager::Get()["CONTROLS"];
	for (Json::Value& inputJson : json)
	{
		inputJson["Key"] = inputJson["DefaultKey"];
		inputJson["ControllerButton"] = inputJson["DefaultControllerButton"];
	}
	JsonManager::Get().SaveToFile("CONTROLS");
}

const bool ControlsManager::CanJoystickInteract(void)
{
	return leftJoystickAxis.x > JOYSTICK_MIN_STEP || leftJoystickAxis.x < -JOYSTICK_MIN_STEP ||
		leftJoystickAxis.y > JOYSTICK_MIN_STEP || leftJoystickAxis.y < -JOYSTICK_MIN_STEP;
}

sf::Texture& ControlsManager::GetTexture(const std::string& _playerInput)
{
	IsControllerConnected();
	if (controllerType == KEYBOARD)
	{
		sf::Keyboard::Key key = inputs[_playerInput].key;
		if (key == sf::Keyboard::Unknown)
		{
			return TexturesManager::Get()["KEY_NONE"];
		}
		else
		{
			return TexturesManager::Get()["KEY_" + std::string(KEY_NAMES[key])];
		}
	}
	else
	{
		ControllerButton button = inputs[_playerInput].button;
		if (button == ControllerButton::NONE)
		{
			return TexturesManager::Get()["KEY_NONE"];
		}
		else if (sf::Joystick::getIdentification(controllerID).name == "Wireless Controller" || sf::Joystick::getIdentification(controllerID).name == "DualSense Wireless Controller")
		{
			return TexturesManager::Get()[std::string(PLAYSTATION_TEXTURE_KEY) + std::string(CONTROLLER_BUTTON_NAMES[button])];
		}
		else
		{
			return TexturesManager::Get()[std::string(XBOX_TEXTURE_KEY) + std::string(CONTROLLER_BUTTON_NAMES[button])];
		}
	}
}

const char& ControlsManager::GetControllerID(void) const
{
	return controllerID;
}

const bool ControlsManager::IsHeldInput(const std::string& _playerInput)
{
	if (controllerType == KEYBOARD)
	{
		return sf::Keyboard::isKeyPressed(inputs[_playerInput].key);
	}
	else
	{
		if (_playerInput == "UP")
			return leftJoystickAxis.y < -JOYSTICK_MIN_STEP;
		else if (_playerInput == "DOWN")
			return leftJoystickAxis.y > JOYSTICK_MIN_STEP;
		else if (_playerInput == "LEFT")
			return leftJoystickAxis.x < -JOYSTICK_MIN_STEP;
		else if (_playerInput == "RIGHT")
			return leftJoystickAxis.x > JOYSTICK_MIN_STEP;
		else if (_playerInput == "INTERACT")
			return sf::Joystick::isButtonPressed(controllerID, inputs["INTERACT"].button);
		else if (_playerInput == "CANCEL")
			return sf::Joystick::isButtonPressed(controllerID, inputs["CANCEL"].button);
		else if (_playerInput == "INVENTORY")
			return sf::Joystick::isButtonPressed(controllerID, inputs["INVENTORY"].button);
		else if (_playerInput == "CIRCULAR_MENU")
			return sf::Joystick::isButtonPressed(controllerID, inputs["CIRCULAR_MENU"].button);
		//else
			//std::cerr << "Can't find held joystick input" << std::endl;
	}
	return false;
}

const bool ControlsManager::IsPressedInput(sf::Event& _event, const std::string& _playerInput)
{
	if (controllerType == KEYBOARD)
	{
		return _event.type == sf::Event::KeyPressed && _event.key.code == inputs[_playerInput].key;
	}
	else
	{
		const bool IsJoyMovedY = _event.type == sf::Event::JoystickMoved && _event.joystickMove.axis == ControllerAxis::LEFT_JOYSTICK_Y;
		const bool IsJoyMovedX = _event.type == sf::Event::JoystickMoved && _event.joystickMove.axis == ControllerAxis::LEFT_JOYSTICK_X;
		const bool isJoyPressed = _event.type == sf::Event::JoystickButtonPressed;
		unsigned int joyButton = _event.joystickButton.button;
		return isJoyPressed && joyButton == inputs[_playerInput].button;
	}
	return false;
}

const bool ControlsManager::IsPressedDirection(const std::string& _playerInput)
{
	float crossAxisX = sf::Joystick::getAxisPosition(controllerID, sf::Joystick::PovX);
	float crossAxisY = sf::Joystick::getAxisPosition(controllerID, sf::Joystick::PovY);

	if (controllerType == CONTROLLER)
	{
		if (timerJoystickMove <= 0.f)
		{
			if (_playerInput == "UP")
			{
				if (crossAxisY > JOYSTICK_IDLE_STEP || leftJoystickAxis.y < -JOYSTICK_MIN_STEP)
				{
					timerJoystickMove = TIMER_MAX_JOYSTICK_MOVE;
					return true;
				}
			}
			else if (_playerInput == "DOWN")
			{
				if (crossAxisY < -JOYSTICK_IDLE_STEP || leftJoystickAxis.y > JOYSTICK_MIN_STEP)
				{
					timerJoystickMove = TIMER_MAX_JOYSTICK_MOVE;
					return true;
				}
			}
			else if (_playerInput == "LEFT")
			{
				if (crossAxisX < -JOYSTICK_IDLE_STEP || leftJoystickAxis.x < -JOYSTICK_MIN_STEP)
				{
					timerJoystickMove = TIMER_MAX_JOYSTICK_MOVE;
					return true;
				}
			}
			else if (_playerInput == "RIGHT")
			{
				if (crossAxisX > JOYSTICK_IDLE_STEP || leftJoystickAxis.x > JOYSTICK_MIN_STEP)
				{
					timerJoystickMove = TIMER_MAX_JOYSTICK_MOVE;
					return true;
				}
			}
			else
			{
				Debug.warning("Can't find pressed joystick input");
			}
		}
	}
	/*
	else if (sf::Keyboard::isKeyPressed(inputs[_playerInput].key))
	{
		return true;
	}
	*/
	return false;
}

const bool ControlsManager::IsPressedTriggerLeft(void)
{
	if (controllerType == CONTROLLER)
	{
		if (timerJoystickMove <= 0.f)
		{
			if (sf::Joystick::getAxisPosition(controllerID, sf::Joystick::Axis::Z) > JOYSTICK_MIN_STEP)
			{
				timerJoystickMove = TIMER_MAX_JOYSTICK_MOVE;
				return true;
			}
		}
	}
	return false;
}

const bool ControlsManager::IsPressedTriggerRight(void)
{
	if (controllerType == CONTROLLER)
	{
		if (timerJoystickMove <= 0.f)
		{
			if (sf::Joystick::getAxisPosition(controllerID, sf::Joystick::Axis::Z) < -JOYSTICK_MIN_STEP)
			{
				timerJoystickMove = TIMER_MAX_JOYSTICK_MOVE;
				return true;
			}
		}
	}
	return false;
}

const bool ControlsManager::IsReleasedInput(sf::Event& _event, const std::string& _playerInput)
{
	if (controllerType == KEYBOARD)
	{
		return _event.type == sf::Event::KeyReleased && _event.key.code == inputs[_playerInput].key;
	}
	else
	{
		////////// Problems with button released

		const bool IsJoyMoved = _event.type == sf::Event::JoystickMoved && _event.joystickMove.axis;
		const bool isJoyReleased = _event.type == sf::Event::JoystickButtonReleased;
		const bool isReleasedX = IsJoyMoved && (int)leftJoystickAxis.x == 0;
		const bool isReleasedY = IsJoyMoved && (int)leftJoystickAxis.y == 0;
		unsigned int joyButton = _event.joystickButton.button;
		if (_playerInput == "UP")
			return isReleasedY;
		else if (_playerInput == "DOWN")
			return isReleasedY;
		else if (_playerInput == "LEFT")
			return isReleasedX;
		else if (_playerInput == "RIGHT")
			return isReleasedX;
		else
			return isJoyReleased && joyButton == inputs[_playerInput].button;
	}
	return false;
}

bool ControlsManager::HasInputChanged()
{
	if (this->hasInputChanged)
	{
		return true;
	}
	this->hasInputChanged = false;
	return false;
}

bool ControlsManager::IsAnyButtonPressed()
{
	// Init bool to use this function in update and use only one time the pressed effect
	bool anyButtonPressed = false;

	for (size_t i = 0; i < ControllerButton::COUNT; ++i)
	{
		if (sf::Joystick::isButtonPressed(controllerID, unsigned int(i)))
		{
			// Change the bool if any button is pressing
			anyButtonPressed = true;

			// Reverse state of general button only if not already pressed
			if (!buttonAlreadyPressed)
			{
				buttonAlreadyPressed = !buttonAlreadyPressed;
				return true;
			}
		}
	}

	// Reverse state of general button if no button pressed
	if (!anyButtonPressed)
	{
		buttonAlreadyPressed = false;
	}

	return false;
}

const std::unordered_map<std::string, ControlsManager::Input>& ControlsManager::GetInputs() const
{
	return inputs;
}