#ifndef TIMER_MANAGER__HPP
#define TIMER_MANAGER__HPP

#include "GameClock.hpp"

class Timer
{
public:
	enum State
	{
		RUNNING, // timer is running
		PAUSED, // timer is paused
		STOPPED // timer is not running
	};

public:
	Timer() = default;
	Timer(const float& _timerDuration, const float& _speed = 1.f);

	/// <summary>
	/// Run timer.
	/// This function substract deltaTime to timer and update the state of the timer.
	/// </summary>
	void Update(void);

	void Restart(void);

	/// <summary>
	/// Start or resume running the timer.
	/// This function starts the timer if it was stopped, resumes it if it was paused, 
	/// and restarts it from beginning if it was already running.
	/// </summary>
	void Run(void);

	/// <summary>
	/// Pause the timer.
	/// This function pauses the timer if it was running, otherwise(timer already paused or stopped) it has no effect.
	/// </summary>
	void Pause(void);

	/// <summary>
	/// stop running the timer
	/// This function stops the timer if it was running or paused, and does nothing if it was already stopped.
	/// It also resets the running position(unlike pause()).
	/// </summary>
	void Stop(void);

	/// <summary>
	/// Set whether or not the timer should loop after reaching the end.
	/// If set, the timer will restart from beginning after reaching the endand so on, until it is stopped or setLoop(false) is called.
	/// The default looping state for timer is false.
	/// </summary>
	/// <param name="_loop"> : True to run in loop, false to run once</param>
	void SetLoop(const bool& _loop);

	/// <summary>
	/// Set the speed of the sound.
	/// The default speed is 1
	/// </summary>
	/// <param name="_speed"> : New speed to apply to the timer</param>
	void SetSpeed(const float& _speed);

	void SetNewTimerDuration(const float _timerDuration);

	/// <summary>
	/// Tell whether the timer's is less than 0 or not
	/// </summary>
	/// <returns> True if the timer is less or equal than 0, false otherwise</returns>
	const bool& IsOver(void) const;

	/// <summary>
	/// Get the current state of the timer (running, paused, stopped)
	/// </summary>
	/// <returns> Current state of the timer</returns>
	const State& GetState(void) const;

	const float& GetTimerDuration(void) const;

	const float& GetTimePassed(void) const;

	const float GetTimeLeft(void) const;

	const float GetPourcentageTimeLeft(void) const;

	const float GetPourcentageTimePassed(void) const;

	~Timer() = default;

private:
	float timerDuration = 1.f;
	float time = 0.f;

	float speed = 1.f;

	State state = STOPPED;
	bool loop = false; // manage if the timer is looping when reaching 0

	bool hasFinishedCycle = false; // true if the time has looped or reach the end
};

#endif // !TIMER_MANAGER__HPP