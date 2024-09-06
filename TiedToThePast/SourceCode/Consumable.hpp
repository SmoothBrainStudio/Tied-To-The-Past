#pragma once
#include "Item.hpp"
#include "Fighting/TextDamage.hpp"
#include "Status.hpp"

class Player;

class Consumable : public Item
{
public:
	enum Effect
	{
		HEAL,
		CURE,
		ATK_BUFF,
		BOMB,
		NONE
	};

	Consumable();
	Consumable(const std::string& _id, sf::Uint16 _count = 1);
	void ApplyEffect(std::shared_ptr<Entity>& _entity);
	void ApplyEffectAoE(std::vector<std::shared_ptr<Entity>> _entities);
	void Increase();
	void Decrease();

	void SetTexture(const sf::Texture& _texture) override;
	void SetEffect(const std::string& _effect);
	void SetEffect(Effect _effect);
	void SetCount(sf::Uint16 _count);

	void SetColor(const sf::Color& _color) override;
	const sf::Texture* GetTexture() const override;
	sf::FloatRect GetRect() const override;
	const sf::Texture& GetBigTexture() const;
	void CreateRenderTexture(sf::RenderStates _states = sf::RenderStates());

	Effect GetEffect() const;
	sf::Uint16 GetCount() const;
	
	void Save(std::ostream& _file) override;
	void Load(std::istream& _file) override;

private:
	void UpdateRenderTexture(sf::RenderStates _states = sf::RenderStates());
	void draw(sf::RenderTarget& _target, sf::RenderStates _states) const override;

	sf::Uint16 count = 1;
	sf::Text countText;
	sf::RenderTexture countRenderTexture;
	sf::Sprite stackSprite;
	std::shared_ptr<TextDamage> bombDamage;
	Status* status = nullptr;
	Effect effect = NONE;
};
