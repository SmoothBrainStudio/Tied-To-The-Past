#ifndef BAR__HPP
#define BAR__HPP

#include "Common.hpp"
#include "MenuItem.hpp"

class Bar : public MenuItem
{
public:
	Bar();
	Bar(const sf::Vector2f& _pos, sf::Texture& _textureBack, sf::Texture& _textureBar, sf::Texture& _textureOutline, sf::Texture& _textureSelector, const std::string& _text = "");
	~Bar() override = default;

	void InitText(const std::string& _text);
	void PollEvent(GameData& _gameData) override;
	void Update(GameData& _gameData) override;

	void SetBackSprite(const sf::Texture& _texture);
	void SetBarSprite(const sf::Texture& _texture);
	void SetOutlineSprite(const sf::Texture& _texture);
	void SetSelectorSprite(const sf::Texture& _texture);
	void SetValue(const short int& _val);
	void SetMax(const short int& _val);
	void SetMin(const short int& _val);

	sf::FloatRect GetRect() const override;
	sf::FloatRect GetGlobalBounds(void) const;
	const short int& GetValue(void);
	const short int& GetMax(void);
	const short int& GetMin(void);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::Sprite back;
	sf::Sprite bar;
	sf::Sprite outline;
	sf::Sprite selector;
	sf::Text text;

	short int lastValue;
	short int value;
	short int max;
	short int min;
};

#endif // !BAR__HPP



