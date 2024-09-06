#include "ProgressionBar.hpp"

ProgressionBar::ProgressionBar(const sf::Vector2f& _position, const sf::Vector2f& _size, float _value, const sf::Color& _backgroundColor, const sf::Color& _loadingColor)
{
	this->className = "progression bar";

	this->backgroundRect.setSize(_size);

	this->setPosition(_position);
	this->SetValue(_value);
	this->SetBackgroundColor(_backgroundColor);
	this->SetLoadingColor(_loadingColor);
}

void ProgressionBar::PollEvent(GameData& _gameData)
{
	
}

float ProgressionBar::GetValue() const
{
	return this->value;
}

const sf::Color& ProgressionBar::GetBackgroundColor() const
{
	return this->backgroundRect.getFillColor();
}

const sf::Color& ProgressionBar::GetLoadingColor() const
{
	return this->loadingRect.getFillColor();
}

void ProgressionBar::SetValue(float _value)
{
	this->value = fmaxf(0.f, fminf(_value, 1.f));

	const sf::Vector2f& size = this->backgroundRect.getSize();
	this->loadingRect.setSize({ size.x * this->value, size.y });
}

void ProgressionBar::SetBackgroundColor(const sf::Color& _color)
{
	this->backgroundRect.setFillColor(_color);
}

void ProgressionBar::SetLoadingColor(const sf::Color& _color)
{
	this->loadingRect.setFillColor(_color);
}

void ProgressionBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= this->getTransform();
	target.draw(this->backgroundRect, states);
	target.draw(this->loadingRect, states);
}