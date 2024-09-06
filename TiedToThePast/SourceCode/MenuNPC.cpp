#include "Definitions.hpp"
#include "MenuNPC.hpp"

MenuNPC::MenuNPC()
{

}

MenuNPC::MenuNPC(const sf::Vector2f& _pos, const float& _speed, const std::string& _animFile) : NPC(_pos, _speed, _animFile)
{

}

void MenuNPC::Interact(sf::Event& _event, std::shared_ptr<Player>& _player)
{
	if (ControlsManager::Get().IsPressedInput(_event, "INTERACT") && this->canSpeak)
	{
		if (!eventKey.empty())
		{
			EventManager::Get()[eventKey];
		}
		else
		{
			Debug.warning("Empty key for eventMng in MenuNPC::Interact");
		}
	}
}

void MenuNPC::SetEventKey(const std::string& _key)
{
	eventKey = _key;
}