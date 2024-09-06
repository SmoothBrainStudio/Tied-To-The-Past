#ifndef FADE__HPP
#define FADE__HPP

#include"Common.hpp"
#include"Shapes.hpp"

class Fade : public sf::Drawable, public sf::Transformable
{
public:

	Fade() = default;
	Fade(const float& _timerMax, const bool& _isFadingIn, const bool& _isFadingOut);
	Fade(const float& _timerMax, const float& _maxVolume, const bool& _isFadingIn, const bool& _isFadingOut);
	~Fade() = default;

	void Update(const float& _dt);
	void Update(const float& _dt, const std::string& _musicKey);
	void UpdateMusic(const float& _dt, const std::string& _musicKey);

	void SetIsFadingIn(const bool& _bool);
	void SetIsFadingOut(const bool& _bool);
	void SetAlpha(const float& _alpha);
	void SetColor(const sf::Color& _color);

	const float& GetTimer(void) const;
	const float& GetTimerMax(void) const;
	const float& GetTimerMin(void) const;
	const bool& GetIsFadingIn(void) const;
	const bool& GetIsFadingOut(void) const;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	bool isFadingIn = false;
	bool isFadingOut = false;

	float timerMin = 0.f;
	float timerMax = 2.f;
	float timer = 0.f;

	float alphaMin = 0.f;
	float alphaMax = 0.f;
	float alpha = 0.f;

	float maxVolume = 0.f;

	sf::Color color;
	sf::RectangleShape rect;
};

#endif // !FADE_H



