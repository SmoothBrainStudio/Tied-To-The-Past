#ifndef ITEM__H
#define ITEM__H

#include "GameData.hpp"

class Item : public sf::Drawable
{
public:
	constexpr static char PREFABS_PATH[] = "Contents/Data/Item.json";

	enum Type
	{
		IMPORTANT,
		TIE,
		CONSUMABLE,
		OTHER
	};
	
	static void LoadPrefabs();
	static void UnloadPrefabs();
	static std::unordered_map<std::string, std::shared_ptr<Item>> prefabs;

	Item() = default;
	Item(const std::string& _id);
	
	void SetID(const std::string& _id);
	void SetName(const std::string& _name);
	void SetDescription(const std::string& _descritption);
	void SetPrice(sf::Uint16 _price);
	void SetType(Type _type);
	virtual void SetColor(const sf::Color& _color);
	virtual void SetTexture(const sf::Texture& _texture);

	const std::string& GetId() const;
	const std::string& GetName() const;
	const std::string& GetDescription() const;
	virtual const sf::Texture* GetTexture() const;
	virtual sf::FloatRect GetRect() const;
	sf::Uint16 GetPrice() const;
	Type GetType() const;

	virtual void Save(std::ostream& _file);
	virtual void Load(std::istream& _file);

protected:
	static std::vector<std::string> GetStrings(const std::string& _filePath);

	void draw(sf::RenderTarget& _target, sf::RenderStates _states) const override;

	std::string id;
	std::string name;
	std::string description;
	sf::Sprite sprite;
	sf::Uint16 price = 0;
	Type type = OTHER;
};

#endif
