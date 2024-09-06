#include "Definitions.hpp"
#include "LightPoint.hpp"

void LightPoint::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	//states.blendMode = sf::BlendNone;
	states.shader = lightShader;

	target.draw(light, states);
}

void LightPoint::Reset()
{
	light.setRadius(radius);
	light.setOrigin(radius, radius);

	lightShader->setUniform("color", sf::Vector3f(230, 126, 34));
	lightShader->setUniform("intensity", intensity / 255);
}

LightPoint::LightPoint() : intensity(0.f), radius(0.f)
{
	DrawManager::Get().AddObject("ExplorationUnSortables", *this);
	light.setPointCount(50);
	lightShader = new sf::Shader();
	lightShader->loadFromFile("Contents/assets/Shaders/CircleShapeLight.frag", sf::Shader::Fragment);
	Reset();
}

LightPoint::LightPoint(float _intensity, float _radius) : intensity(_intensity), radius(_radius)
{
	DrawManager::Get().AddObject("ExplorationUnSortables", *this);
	light.setPointCount(50);
	lightShader = new sf::Shader();
	lightShader->loadFromFile("Contents/assets/Shaders/CircleShapeLight.frag", sf::Shader::Fragment);
	Reset();
}

LightPoint::~LightPoint()
{
	DrawManager::Get().DeleteObject("ExplorationUnSortables", this);
}

void LightPoint::SetIntensity(const float& _intensity)
{
	intensity = _intensity;
	Reset();
}

void LightPoint::SetRadius(const float& _radius)
{
	radius = _radius;
	Reset();
}

const float LightPoint::GetIntensity(void)
{
	return intensity;
}

const float LightPoint::GetRadius(void)
{
	return radius;
}
