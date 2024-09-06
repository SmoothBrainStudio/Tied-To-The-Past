#include "Definitions.hpp"
#include "SaveButton.hpp"
#include "SaveSystem.hpp"

SaveButton::SaveButton()
{
	this->Init();
}

SaveButton::SaveButton(const sf::Vector2f& _size, const sf::String& _text)
{
	this->nameText.setString(_text);
	this->SetSize(_size);
	this->Init();
}

void SaveButton::Init()
{
	this->rect.setFillColor(sf::Color::Transparent);

	this->nameText.setFont(FontsManager::Get()["DEFAULT"]);
	this->nameText.setCharacterSize(40);
	this->nameText.setStyle(sf::Text::Bold);
	SetCenteredText(this->nameText);
	this->nameText.setOrigin(0.f, this->nameText.getOrigin().y);
	this->nameText.setPosition(NAME_POS_X, this->rect.getSize().y * 0.25f);

	this->hourglass.setTexture(TexturesManager::Get()["HOURGLASS"]);
	this->hourglass.setOrigin(0.f, this->hourglass.getGlobalBounds().height * 0.5f);
	this->hourglass.setPosition(NAME_POS_X, this->rect.getSize().y * 0.5f);

	this->timeText.setFont(FontsManager::Get()["DEFAULT"]);
	this->timeText.setString("0");
	SetCenteredText(this->timeText);
	this->timeText.setString("");
	this->timeText.setOrigin(0.f, this->timeText.getOrigin().y);
	this->timeText.setPosition(this->hourglass.getPosition().x + this->hourglass.getGlobalBounds().width + 20.f, this->hourglass.getPosition().y);

	this->clickCallBack = [] {};
	this->outCallBack = [] {};
	this->overCallBack = [] {};
}

void SaveButton::Update(GameData& _gameData)
{
	const sf::String& name = this->nameText.getString();
	// Check if it is not the new save button
	if (name.substring(0, 4) == "Save")
	{
		sf::Uint32 playTime = (sf::Uint32)SaveSystem::Get().GetSaveTime(name);
		sf::Uint32 hours = playTime / 3600;
		sf::Uint32 minutes = playTime % 3600 / 60;
		sf::Uint32 seconds = playTime % 60;
		std::string hoursText = std::to_string(hours);
		if (hoursText.size() == 1)
		{
			hoursText.insert(0, "0");
		}
		std::string minutesText = std::to_string(minutes);
		if (minutesText.size() == 1)
		{
			minutesText.insert(0, "0");
		}
		std::string secondsText = std::to_string(seconds);
		if (secondsText.size() == 1)
		{
			secondsText.insert(0, "0");
		}
		this->timeText.setString(hoursText + ':' + minutesText + ':' + secondsText);
	}
	else
	{
		this->hourglass.setColor(sf::Color::Transparent);
	}
}

void SaveButton::PollEvent(GameData& _gameData)
{
	ControlsManager& controlsMng = ControlsManager::Get();

	if (controlsMng.GetControllerType() == KEYBOARD)
	{
		if (_gameData.gameEvent.type == sf::Event::MouseMoved)
		{
			sf::FloatRect transformedRect = this->getTransform().transformRect(this->rect.getGlobalBounds());
			if (transformedRect.contains(Screen::Get().GetMousePosition(_gameData.window)))
			{
				_gameData.window.setMouseCursor(_gameData.cursor);
				this->rect.setFillColor(sf::Color(255, 255, 255, 50));
				this->isHovered = true;
			}
			else
			{
				_gameData.window.setMouseCursor(_gameData.cursorGrey);
				this->rect.setFillColor(sf::Color::Transparent);
				this->isHovered = false;
			}
		}
		if (this->isHovered)
		{
			if (controlsMng.IsPressedInput(_gameData.gameEvent, "INTERACT")
				|| controlsMng.IsPressedInput(_gameData.gameEvent, "INTERACT_2"))
			{
				this->CallOnClick();
			}
			else if (controlsMng.GetControllerType() == KEYBOARD && _gameData.gameEvent.type == sf::Event::MouseButtonPressed)
			{
				if (_gameData.gameEvent.mouseButton.button == sf::Mouse::Left)
				{
					this->CallOnClick();
				}
			}
		}
	}
}

sf::FloatRect SaveButton::GetRect() const
{
	sf::FloatRect rectGlobalBounds = this->rect.getGlobalBounds();
	return { this->getPosition(), { rectGlobalBounds.width, rectGlobalBounds.height } };
}

void SaveButton::SetSelected(bool _selected)
{
	if (_selected)
	{
		this->rect.setFillColor(sf::Color(255, 255, 255, 100));
	}
	else
	{
		this->rect.setFillColor(sf::Color::Transparent);
	}
}

void SaveButton::SetSize(const sf::Vector2f& _size)
{
	this->rect.setSize(_size);
	this->nameText.setPosition(this->rect.getGlobalBounds().width * 0.5f, this->rect.getGlobalBounds().height * 0.5f);
}

void SaveButton::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
{
	_states.transform *= this->getTransform();
	_target.draw(this->nameText, _states);
	_target.draw(this->hourglass, _states);
	_target.draw(this->timeText, _states);
	_target.draw(this->rect, _states);
	for (auto& tie : this->ties)
	{
		_target.draw(tie, _states);
	}
}
