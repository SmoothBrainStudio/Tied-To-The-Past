#include "Definitions.hpp"
#include "EventManager.hpp"

EventManager& EventManager::Get(void)
{
	static EventManager instance;
	return instance;
}

const bool EventManager::Add(const std::string& _key, std::function<void(void)>&& _event, std::function<bool(void)>&& _testEvent)
{
	auto it = eventMap.find(_key);

	if (it != eventMap.end())
	{
		Debug.warning("WRONG KEY ENTERED IN EVENT MAP\"", _key, "\"");
		return false;
	}

	std::unique_ptr<EventContainer> temp = std::make_unique<EventContainer>();
	temp->callEvent = _event;
	temp->testEvent = _testEvent;
	eventMap[_key] = std::move(temp);
	return true;
}

const bool EventManager::SetTest(const std::string& _key, std::function<bool(void)>&& _testEvent)
{
	auto it = eventMap.find(_key);

	if (it != eventMap.end())
	{
		eventMap[_key]->callEvent = _testEvent;
		return true;
	}

	Debug.warning("KEY NOT FOUND IN EVENT MAP \"", _key, "\"");
	return false;
}

const bool EventManager::Delete(const std::string& _key)
{
	auto it = eventMap.find(_key);

	if (it != eventMap.end())
	{
		eventMap.erase(_key);
		return true;
	}

	Debug.error("KEY NOT FOUND IN EVENT MAP\"", _key, "\"");
	return false;
}

void EventManager::operator[](const std::string& _key)
{
	if (eventMap[_key]->testEvent())
	{
		eventMap[_key]->callEvent();
	}
}