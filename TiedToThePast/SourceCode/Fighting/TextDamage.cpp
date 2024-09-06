#include "TextDamage.hpp"

TextDamage::TextDamage(const std::string& _string, const sf::Vector2f& _position)
	: timer(0), isDelete(false), positionMem(_position)
{
	TexturesManager& textureManager = TexturesManager::Get();
	FontsManager& fontManager = FontsManager::Get();

	text.setFont(fontManager["FIGHTING"]);
	text.setString(_string);
	text.setFillColor(sf::Color(228, 150, 38));
	text.setCharacterSize(100);
	text.setOutlineThickness(2);
	text.setOutlineColor(sf::Color(52, 15, 13));

	sf::FloatRect rect = text.getGlobalBounds();
	text.setOrigin(rect.left + rect.width / 2.f, rect.top + rect.height / 2.f);

	sf::Vector2u size = (&textureManager["DAMAGE"])->getSize();
	damageBackground.setTexture(textureManager["DAMAGE"]);
	damageBackground.setOrigin(sf::Vector2f(size.x / 2.f, size.y / 2.f));

	text.setPosition(positionMem);
	damageBackground.setPosition(positionMem);

	DrawManager::Get().AddObject("FIGHT_HUD", *this);
}

TextDamage::~TextDamage()
{
	DrawManager::Get().DeleteObject("FIGHT_HUD", this);
}

void TextDamage::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(damageBackground, states);
	target.draw(text, states);
}

void TextDamage::Update()
{
	GameClock& timeManager = GameClock::Get();

	timer += timeManager.GetDeltaTime();
	float factor = Easings::OutCubic(Maths::Clamp(timer / LIFE_TIME, 0.f, 1.f));

	sf::Color textColor = text.getFillColor();
	sf::Color textOutlineColor = text.getOutlineColor();
	sf::Color backgroundColor = damageBackground.getColor();
	textColor.a = (sf::Uint8)(255 - 255 * factor);
	textOutlineColor.a = (sf::Uint8)(255 - 255 * factor);
	backgroundColor.a = (sf::Uint8)(255 - 255 * factor);

	
	text.setFillColor(textColor);
	text.setOutlineColor(textOutlineColor);

	text.setPosition(positionMem + sf::Vector2f(0.f, -200.f) * factor);
	damageBackground.setPosition(text.getPosition());
	damageBackground.setColor(backgroundColor);

	if (timer >= LIFE_TIME)
	{
		isDelete = true;
	}
}

const bool& TextDamage::IsDelete()
{
	return isDelete;
}

const sf::Vector2f& TextDamage::GetPosMem(void) const
{
	return positionMem;
}

sf::Sprite& TextDamage::GetDamageBackground(void)
{
	return damageBackground;
}

sf::Text& TextDamage::GetText(void)
{
	return text;
}
