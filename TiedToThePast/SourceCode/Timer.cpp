#include "Definitions.hpp"
#include "Timer.hpp"


Timer::Timer(const float& _timerDuration, const float& _speed)
{
	this->timerDuration = _timerDuration;
	this->speed = _speed;
}

void Timer::Update(void)
{
	if (this->state == RUNNING)
	{
		this->hasFinishedCycle = false;

		this->time += GameClock::Get().GetDeltaTime() * this->speed;

		if (this->time >= this->timerDuration)
		{
			this->hasFinishedCycle = true;

			// loop time with precision
			if (this->loop)
			{
				this->time = fmod(this->time, this->timerDuration);
			}
			else
			{
				this->state = STOPPED;
			}
		}
	}
}

void Timer::Restart(void)
{
	this->state = RUNNING;
	this->time = 0.f;
	this->hasFinishedCycle = false;
}

void Timer::Run(void)
{
	switch (this->state)
	{
	case RUNNING:
		this->Restart();
		break;
	case PAUSED:
		this->state = RUNNING;
		break;
	case STOPPED:
		this->state = RUNNING;
		break;
	default:
		break;
	}
}

void Timer::Pause(void)
{
	if (this->state == RUNNING)
	{
		this->state = PAUSED;
	}
}

void Timer::Stop(void)
{
	if (this->state == RUNNING || this->state == PAUSED)
	{
		this->state = STOPPED;
		this->time = this->timerDuration;
		this->hasFinishedCycle = false;
	}
}

void Timer::SetLoop(const bool& _loop)
{
	this->loop = _loop;
}

void Timer::SetSpeed(const float& _speed)
{
	this->speed = _speed;
}

void Timer::SetNewTimerDuration(const float _timerDuration)
{
	this->timerDuration = _timerDuration;
}

const bool& Timer::IsOver(void) const
{
	return this->hasFinishedCycle;
}

const Timer::State& Timer::GetState(void) const
{
	return this->state;
}

const float& Timer::GetTimerDuration(void) const
{
	return this->timerDuration;
}

const float& Timer::GetTimePassed(void) const
{
	return this->time;
}

const float Timer::GetTimeLeft(void) const
{
	return this->timerDuration - this->time;
}

const float Timer::GetPourcentageTimePassed(void) const
{
	return this->time / this->timerDuration * 100.f;
}

const float Timer::GetPourcentageTimeLeft(void) const
{
	return (this->timerDuration - this->time) / this->timerDuration * 100.f;
}