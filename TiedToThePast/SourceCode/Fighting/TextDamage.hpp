#ifndef TEXTDAMAGE_HPP
#define TEXTDAMAGE_HPP

#include "../Common.hpp"
#include "../Maths.hpp"
#include "../Easings.hpp"

class TextDamage : public sf::Drawable, public sf::Transformable
{
public:
	static constexpr float LIFE_TIME = 4.f;

	TextDamage(const std::string& _string, const sf::Vector2f& _position);
	~TextDamage();

	void Update();
	const bool& IsDelete();

	const sf::Vector2f& GetPosMem(void) const;
	sf::Sprite& GetDamageBackground(void);
	sf::Text& GetText(void);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::Sprite damageBackground;
	sf::Vector2f positionMem;
	float timer;
	sf::Text text;
	bool isDelete;

};

#endif // !TEXTDAMAGE_HPP
