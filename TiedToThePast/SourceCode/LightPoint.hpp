#ifndef LIGHT_POINT__HPP
#define LIGHT_POINT__HPP

#include "Definitions.hpp"
#include "Screen.hpp"
#include "TexturesManager.hpp"
#include "ShadersManager.hpp"
#include "DrawManager.hpp"

class LightPoint : public sf::Transformable, public sf::Drawable
{
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	TexturesManager& texMng = TexturesManager::Get();
	ShadersManager& sdrMng = ShadersManager::Get();
	Screen& scrMng = Screen::Get();

	sf::CircleShape light;
	float intensity;
	float radius;
	sf::Shader* lightShader;

public:
	LightPoint();
	LightPoint(float _intensity, float _radius);
	~LightPoint();

	void Reset();

	void SetIntensity(const float& _intensity);
	void SetRadius(const float& _radius);

	const float GetIntensity(void);
	const float GetRadius(void);

};

#endif // !LIGHT_POINT__HPP
