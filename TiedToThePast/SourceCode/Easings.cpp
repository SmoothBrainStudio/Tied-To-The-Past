#include "Definitions.hpp"
#include "Easings.hpp"


//for cerp and lerp, returns a vector that is the current position between a and b according to t that represents the progression of the action between 0 and 1
sf::Vector2f Easings::Cerp(const sf::Vector2f& _a, const sf::Vector2f& _b, const float& _t)
{
	float temp;
	temp = (1.f - cosf(_t * PI)) * 0.5f;

	return sf::Vector2f(
		(_a.x * (1.f - temp) + _b.x * temp),
		(_a.y * (1.f - temp) + _b.y * temp)
	);

}

sf::Vector2f Easings::Lerp(const sf::Vector2f& _a, const sf::Vector2f& _b, const float& _t)
{
	return sf::Vector2f
	(
		(1.f - _t) * _a.x + _t * _b.x,
		(1.f - _t) * _a.y + _t * _b.y
	);
}

sf::Vector2f Easings::Lerp2(const sf::Vector2f& _a, const sf::Vector2f& _b, const float& _t)
{
	return sf::Vector2f
	(
		_a.x + (_b.x - _a.x) * _t,
		_a.y + (_b.y - _a.y) * _t
	);
}

sf::Vector2f Easings::LerpX(const sf::Vector2f& _a, const sf::Vector2f& _b, const float& _timeFactor, const float& _easingFactor)
{
	return sf::Vector2f
	(
		_a.x + (_b.x - _a.x) * _easingFactor,
		_a.y + (_b.y - _a.y) * _timeFactor
	);
}

sf::Vector2f Easings::LerpY(const sf::Vector2f& _a, const sf::Vector2f& _b, const float& _timeFactor, const float& _easingFactor)
{
	return sf::Vector2f
	(
		_a.x + (_b.x - _a.x) * _timeFactor,
		_a.y + (_b.y - _a.y) * _easingFactor
	);
}

//for easings, just give the progression between 0 and 1

float Easings::InSine(const float& _t)
{
	return 1 - cosf((_t * PI) / 2.f);
}

float Easings::OutSine(const float& _t)
{
	return sinf((_t * PI) / 2.f);
}

float Easings::InOutSine(const float& _t)
{
	return -(cosf(PI * _t) - 1.f) / 2.f;
}

const float Easings::InQuad(const float& _t)
{
	return _t * _t;
}

float Easings::OutQuad(const float& _t)
{
	return 1.f - (1.f - _t) * (1.f - _t);
}

float Easings::InOutQuad(const float& _t)
{
	return _t < 0.5f ? 2.f * _t * _t : 1.f - powf(-2.f * _t + 2.f, 2.f) / 2.f;
}

float Easings::InCubic(const float& _t)
{
	return _t * _t * _t;
}

float Easings::OutCubic(const float& _t)
{
	return 1.f - powf(1.f - _t, 3.f);
}

float Easings::InOutCubic(const float& _t)
{
	return _t < 0.5f ? 4.f * _t * _t * _t : 1.f - powf(-2.f * _t + 2.f, 3.f) / 2.f;
}

float Easings::InQuart(const float& _t)
{
	return _t * _t * _t * _t;
}

float Easings::OutQuart(const float& _t)
{
	return 1.f - powf(1.f - _t, 4.f);
}

float Easings::InOutQuart(const float& _t)
{
	return _t < 0.5f ? 8.f * _t * _t * _t * _t : 1.f - powf(-2.f * _t + 2.f, 4.f) / 2.f;
}

float Easings::InQuint(const float& _t)
{
	return _t * _t * _t * _t * _t;
}

float Easings::OutQuint(const float& _t)
{
	return 1.f - powf(1.f - _t, 5.f);
}

float Easings::InOutQuint(const float& _t)
{
	return _t < 0.5f ? 16.f * _t * _t * _t * _t * _t : 1.f - powf(-2.f * _t + 2.f, 5.f) / 2.f;
}

float Easings::InExpo(const float& _t)
{
	return _t == 0.f ? 0.f : powf(2.f, 10.f * _t - 10.f);
}

float Easings::OutExpo(const float& _t)
{
	return _t == 1.f ? 1.f : 1.f - powf(2.f, -10.f * _t);
}

float Easings::InOutExpo(const float& _t)
{
	if (_t == 0)
	{
		return 0;
	}
	else if (_t == 1)
	{
		return 1;
	}
	else
	{
		return  _t < 0.5f ? powf(2.f, 20.f * _t - 10.f) / 2.f : (2.f - powf(2.f, -20.f * _t + 10.f)) / 2.f;
	}
}

float Easings::InCirc(const float& _t)
{
	return 1.f - sqrtf(1.f - powf(_t, 2.f));
}

float Easings::OutCirc(const float& _t)
{
	return sqrtf(1.f - powf(_t - 1.f, 2.f));
}

float Easings::InOutCirc(const float& _t)
{
	return _t < 0.5f
		? (1.f - sqrtf(1.f - powf(2.f * _t, 2.f))) / 2.f
		: (sqrtf(1.f - powf(-2.f * _t + 2.f, 2.f)) + 1.f) / 2.f;
}

float Easings::InBack(const float& _t)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1.f;

	return c3 * _t * _t * _t - c1 * _t * _t;
}

float Easings::OutBack(const float& _t)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1.f;

	return 1.f + c3 * powf(_t - 1.f, 3.f) + c1 * powf(_t - 1.f, 2.f);
}

float Easings::InOutBack(const float& _t)
{
	const float c1 = 1.70158f;
	const float c2 = c1 * 1.525f;

	return _t < 0.5f
		? (powf(2.f * _t, 2.f) * ((c2 + 1.f) * 2.f * _t - c2)) / 2.f
		: (powf(2.f * _t - 2.f, 2.f) * ((c2 + 1.f) * (_t * 2.f - 2.f) + c2) + 2.f) / 2.f;
}

float Easings::InElastic(const float& _t)
{
	const float c4 = (2.f * PI) / 3.f;

	if (_t == 0)
	{
		return 0;
	}
	else if (_t == 1)
	{
		return 1;
	}
	else
	{
		return -powf(2.f, 10.f * _t - 10.f) * sinf((_t * 10.f - 10.75f) * c4);
	}
}

float Easings::OutElastic(const float& _t)
{
	const float c4 = (2.f * PI) / 3.f;

	if (_t == 0)
	{
		return 0;
	}
	else if (_t == 1)
	{
		return 1;
	}
	else
	{
		return powf(2.f, -10.f * _t) * sinf((_t * 10.f - 0.75f) * c4) + 1.f;
	}
}

float Easings::InOutElastic(const float& _t)
{
	const float c5 = (2.f * PI) / 4.5f;

	if (_t == 0)
	{
		return 0;
	}
	else if (_t == 1)
	{
		return 1;
	}
	else
	{
		return _t < 0.5f
			? -(powf(2.f, 20.f * _t - 10.f) * sinf((20.f * _t - 11.125f) * c5)) / 2.f
			: (powf(2.f, -20.f * _t + 10.f) * sinf((20.f * _t - 11.125f) * c5)) / 2.f + 1.f;
	}
}

float Easings::InBounce(const float& _t)
{
	return 1.f - Easings::OutBounce(1.f - _t);
}

float Easings::OutBounce(float _t)
{
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (_t < 1.f / d1)
	{
		return n1 * _t * _t;
	}
	else if (_t < 2.f / d1)
	{
		return n1 * (_t -= 1.5f / d1) * _t + 0.75f;
	}
	else if (_t < 2.5f / d1)
	{
		return n1 * (_t -= 2.25f / d1) * _t + 0.9375f;
	}
	else
	{
		return n1 * (_t -= 2.625f / d1) * _t + 0.984375f;
	}
}

float Easings::InOutBounce(const float& _t)
{
	return _t < 0.5f
		? (1.f - Easings::OutBounce(1.f - 2.f * _t)) / 2.f
		: (1.f + Easings::OutBounce(2.f * _t - 1.f)) / 2.f;
}

float Easings::InOutOutInSine(const float& _t)
{
	return (1 - powf(1 - 2 * _t, 2));
}

float Easings::Square(const float& _t)
{
	return (-4 * (_t * _t) + 4 * _t);
}

float Easings::InCubicInvert(const float& _t)
{
	return (-(_t * _t * _t) + 1);
}

float Easings::SmoothSin(const float& _x, const float& _period)
{
	return (sin((PI / (_period / 2)) * _x - PI / 2) / 2) + 0.5f;
}