#ifndef EVENT_MANAGER__HPP
#define EVENT_MANAGER__HPP

#include "Definitions.hpp"
//Copyright 2024 Property of Olivier Maurin.All rights reserved.
class EventManager
{
public:
	~EventManager() = default;
	EventManager(const EventManager&) = delete;
	EventManager(EventManager&&) = delete;
	EventManager& operator=(const EventManager&) = delete;
	EventManager& operator=(EventManager&&) = delete;

	static EventManager& Get(void);

	const bool Add(const std::string& _key, std::function<void(void)>&& _event, std::function<bool(void)>&& _testEvent = [](void)
	{
		return true;
	});

	const bool SetTest(const std::string& _key, std::function<bool(void)>&& _testEvent);

	const bool Delete(const std::string& _key);

	void operator[](const std::string& _key);

private:
	EventManager() = default;

	struct EventContainer
	{
		std::function<bool(void)> testEvent;
		std::function<void(void)> callEvent;
	};

	std::unordered_map<std::string, std::unique_ptr<EventContainer>> eventMap;
};

#endif // !EVENT_MANAGER__HPP


