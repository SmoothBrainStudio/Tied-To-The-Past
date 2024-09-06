#ifndef EASINGS__HPP
#define EASINGS__HPP

#include "Definitions.hpp"

namespace Easings
{

	sf::Vector2f Cerp(const sf::Vector2f& _a, const sf::Vector2f& _b, const float& _t);

	sf::Vector2f Lerp(const sf::Vector2f& _a, const sf::Vector2f& _b, const float& _t);

	sf::Vector2f Lerp2(const sf::Vector2f& _a, const sf::Vector2f& _b, const float& _t);

	sf::Vector2f LerpX(const sf::Vector2f& _a, const sf::Vector2f& _b, const float& _timeFactor, const float& _factor);

	sf::Vector2f LerpY(const sf::Vector2f& _a, const sf::Vector2f& _b, const float& _timeFactor, const float& _easingFactor);

	float InSine(const float& _t);

	float OutSine(const float& _t);

	float InOutSine(const float& _t);

	const float InQuad(const float& _t);

	float OutQuad(const float& _t);

	float InOutQuad(const float& _t);

	float InCubic(const float& _t);

	float OutCubic(const float& _t);

	float InOutCubic(const float& _t);

	float InQuart(const float& _t);

	float OutQuart(const float& _t);

	float InOutQuart(const float& _t);

	float InQuint(const float& _t);

	float OutQuint(const float& _t);

	float InOutQuint(const float& _t);

	float InExpo(const float& _t);

	float OutExpo(const float& _t);

	float InOutExpo(const float& _t);

	float InCirc(const float& _t);

	float OutCirc(const float& _t);

	float InOutCirc(const float& _t);

	float InBack(const float& _t);

	float OutBack(const float& _t);

	float InOutBack(const float& _t);

	float InElastic(const float& _t);

	float OutElastic(const float& _t);

	float InOutElastic(const float& _t);

	float InBounce(const float& _t);

	float OutBounce(float _t);

	float InOutBounce(const float& _t);

	float InOutOutInSine(const float& _t);

	float Square(const float& _t);

	float InCubicInvert(const float& _t);

	float SmoothSin(const float& _x, const float& _period);
};

#endif // !EASINGS_H

