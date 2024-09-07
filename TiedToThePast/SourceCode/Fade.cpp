#include "Definitions.hpp"
#include "Fade.hpp"
//Copyright 2024 Property of Olivier Maurin.All rights reserved.

Fade::Fade(const float& _timerMax, const bool& _isFadingIn, const bool& _isFadingOut)
{
	isFadingIn = _isFadingIn;
	isFadingOut = _isFadingOut;

	timerMin = 0.f;
	timerMax = _timerMax;

	alphaMin = 0.f;
	alphaMax = 255.f;

	maxVolume = 0.f;

	if (_isFadingIn)
	{
		timer = timerMin;
		alpha = alphaMin;
	}
	else
	{
		timer = timerMax;
		alpha = alphaMax;
	}


	color = sf::Color(0, 0, 0, (sf::Uint8)alpha);
	rect = Shapes::CreateRectangle(sf::Vector2f(0.f, 0.f), Screen::Get().Pos(WORKING_WIDTH, WORKING_HEIGHT), color, false);
}

Fade::Fade(const float& _timerMax, const float& _maxVolume, const bool& _isFadingIn, const bool& _isFadingOut)
{
	isFadingIn = _isFadingIn;
	isFadingOut = _isFadingOut;

	timerMin = 0.f;
	timerMax = _timerMax;

	alphaMin = 0.f;
	alphaMax = 255.f;

	maxVolume = _maxVolume;

	if (_isFadingIn)
	{
		timer = timerMin;
		alpha = alphaMin;
	}
	else
	{
		timer = timerMax;
		alpha = alphaMax;
	}

	color = sf::Color(0, 0, 0, (sf::Uint8)alpha);
	rect = Shapes::CreateRectangle(sf::Vector2f(0.f, 0.f), Screen::Get().Pos(WORKING_WIDTH, WORKING_HEIGHT), color, false);
}

void Fade::Update(const float& _dt)
{
	if (rect.getSize().x != Screen::Get().GetWidth() || rect.getSize().y != Screen::Get().GetHeight())
	{
		rect.setSize(sf::Vector2f(Screen::Get().GetWidth(), Screen::Get().GetHeight()));
	}

	if (isFadingOut)
	{
		timer = timer - _dt;
		alpha = fmaxf((timer / timerMax) * alphaMax, alphaMin);
		color = sf::Color(0, 0, 0, (sf::Uint8)alpha);
		rect.setFillColor(color);

		if (timer <= timerMin)
		{
			isFadingOut = false;
		}
	}
	else if (isFadingIn)
	{
		timer = timer + _dt;
		alpha = fminf((timer / timerMax) * alphaMax, alphaMax);
		color = sf::Color(0, 0, 0, (sf::Uint8)alpha);
		rect.setFillColor(color);

		if (timer >= timerMax)
		{
			isFadingIn = false;
		}
	}

}

void Fade::Update(const float& _dt, const std::string& _musicKey)
{
	if (rect.getSize().x != Screen::Get().GetWidth() || rect.getSize().y != Screen::Get().GetHeight())
	{
		rect.setSize(sf::Vector2f(Screen::Get().GetWidth(), Screen::Get().GetHeight()));
	}

	if (isFadingOut)
	{
		timer = timer - _dt;
		alpha = fmaxf((timer / timerMax) * alphaMax, alphaMin);
		color = sf::Color(0, 0, 0, (sf::Uint8)alpha);
		rect.setFillColor(color);

		Audio::Get().SetMusicVolume(_musicKey, fmaxf(((timerMax - timer) / timerMax) * (maxVolume * Audio::Get().GetSoundMultiplier()), 0.f));

		if (timer <= timerMin)
		{
			isFadingOut = false;
			
		}
	}
	else if (isFadingIn)
	{
		timer = timer + _dt;
		alpha = fminf((timer / timerMax) * alphaMax, alphaMax);
		color = sf::Color(0, 0, 0, (sf::Uint8)alpha);
		rect.setFillColor(color);

		Audio::Get().SetMusicVolume(_musicKey, fminf(((timerMax - timer) / timerMax) * (maxVolume * Audio::Get().GetSoundMultiplier()), maxVolume * Audio::Get().GetSoundMultiplier()));

		if (timer >= timerMax)
		{
			isFadingIn = false;
			Audio::Get().StopMusic(_musicKey);
		}
	}

}

void Fade::UpdateMusic(const float& _dt, const std::string& _musicKey)
{
	if (isFadingOut)
	{
		timer = timer - _dt;

		Audio::Get().SetMusicVolume(_musicKey, fmaxf(((timerMax - timer) / timerMax) * (maxVolume * Audio::Get().GetSoundMultiplier()), 0.f));

		if (timer <= timerMin)
		{
			isFadingOut = false;
		}
	}
	else if (isFadingIn)
	{
		timer = timer + _dt;

		Audio::Get().SetMusicVolume(_musicKey, fminf(((timerMax - timer) / timerMax) * (maxVolume * Audio::Get().GetSoundMultiplier()), maxVolume * Audio::Get().GetSoundMultiplier()));

		if (timer >= timerMax)
		{
			isFadingIn = false;
			Audio::Get().StopMusic(_musicKey);
		}
	}
}

void Fade::SetIsFadingIn(const bool& _bool)
{
	isFadingIn = _bool;

	if (isFadingIn)
	{
		timer = timerMin;
		alpha = alphaMin;
	}
}

void Fade::SetIsFadingOut(const bool& _bool)
{
	isFadingOut = _bool;

	if (isFadingOut)
	{
		timer = timerMax;
		alpha = alphaMax;
	}
}

void Fade::SetAlpha(const float& _alpha)
{
	if (_alpha >= alphaMin && _alpha <= alphaMax)
	{
		alpha = _alpha;
	}
}

void Fade::SetColor(const sf::Color& _color)
{
	color = _color;
	rect.setFillColor(color);
}

const float& Fade::GetTimer(void) const
{
	return timer;
}

const float& Fade::GetTimerMax(void) const
{
	return timerMax;
}

const float& Fade::GetTimerMin(void) const
{
	return timerMin;
}

const bool& Fade::GetIsFadingIn(void) const
{
	return isFadingIn;
}

const bool& Fade::GetIsFadingOut(void) const
{
	return isFadingOut;
}

void Fade::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(rect);
}
