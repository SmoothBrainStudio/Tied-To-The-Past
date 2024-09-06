#include "Item.hpp"
#include "Consumable.hpp"
#include "Tie.hpp"

std::unordered_map<std::string, std::shared_ptr<Item>> Item::prefabs;

void Item::LoadPrefabs()
{
	TexturesManager& texturesMng = TexturesManager::Get();
	Json::Value json = JsonManager::LoadFromFile(PREFABS_PATH);
	for (sf::Uint32 i = 0; i < json.size(); ++i)
	{
		std::shared_ptr<Item> item;
		if (json[i]["Type"].asString() == "CONSUMABLE")
		{
			std::shared_ptr<Consumable> consumable = std::make_shared<Consumable>();
			std::string bigId = "BIG_" + json[i]["ID"].asString();
			texturesMng.Add(json[i]["BigTexture"].asString(), bigId);
			consumable->SetEffect(json[i]["Effect"].asString());
			item = consumable;
		}
		else if (json[i]["Type"].asString() == "IMPORTANT")
		{
			item = std::make_shared<Item>();
			item->SetType(Item::IMPORTANT);
		}
		else if (json[i]["Type"].asString() == "TIE")
		{
			std::shared_ptr<Tie> tie = std::make_shared<Tie>(json[i]);

			item = tie;
		}
		else
		{
			item = std::make_shared<Item>();
		}

		std::string itemId = json[i]["ID"].asString();
		item->SetID(itemId);
		item->SetName(json[i]["Name"].asString());
		item->SetDescription(json[i]["Description"].asString());
		item->SetPrice(json[i]["Price"].asInt());
		texturesMng.Add(json[i]["Texture"].asString(), itemId);
		item->SetTexture(texturesMng[itemId]);

		prefabs.insert({ itemId, item });
	}
}

void Item::UnloadPrefabs()
{
	TexturesManager& texturesMng = TexturesManager::Get();
	Json::Value json = JsonManager::LoadFromFile(PREFABS_PATH);
	for (sf::Uint32 i = 0; i < json.size(); ++i)
	{
		std::string itemId = json[i]["ID"].asString();
		texturesMng.Delete(itemId);

		if (json[i]["Type"].asString() == "CONSUMABLE")
		{
			texturesMng.Delete("BIG_" + json[i]["ID"].asString());
		}
	}

	Item::prefabs.clear();
}

Item::Item(const std::string& _id)
{
	*this = *prefabs[_id];
}

void Item::SetID(const std::string& _id)
{
	this->id = _id;
}

void Item::SetName(const std::string& _name)
{
	this->name = _name;
}

void Item::SetDescription(const std::string& _description)
{
	this->description = _description;
}

void Item::SetPrice(sf::Uint16 _price)
{
	this->price = _price;
}

void Item::SetType(Type _type)
{
	this->type = _type;
}

void Item::SetColor(const sf::Color& _color)
{
	this->sprite.setColor(_color);
}

void Item::SetTexture(const sf::Texture& _texture)
{
	this->sprite.setTexture(_texture);
}

const std::string& Item::GetId() const
{
	return this->id;
}

const std::string& Item::GetName() const
{
	return this->name;
}

const std::string& Item::GetDescription() const
{
	return this->description;
}

const sf::Texture* Item::GetTexture() const
{
	return this->sprite.getTexture();
}

sf::FloatRect Item::GetRect() const
{
	return this->sprite.getGlobalBounds();
}

sf::Uint16 Item::GetPrice() const
{
	return this->price;
}

Item::Type Item::GetType() const
{
	return this->type;
}

void Item::Save(std::ostream& _file)
{
	_file << this->id;
}

void Item::Load(std::istream & _file)
{
	/*
	std::string itemId;
	_file >> itemId;
	auto it = Item::prefabs.find(itemId);
	if (it == Item::prefabs.end())
	{
		Debug.error(itemId + " is not an item id");
	}
	else
	{
		*this = *it->second;
	}
	*/
}

std::vector<std::string> Item::GetStrings(const std::string& _filePath)
{
	std::vector<std::string> strings;
	std::string currentString;
	std::ifstream file(_filePath);
	while (!file.eof())
	{
		std::string word;
		file >> word;
		if (word == "|")
		{
			strings.push_back(word);
			word = "";
		}
		else
		{
			currentString.append(word);
		}
	}
	return strings;
}

void Item::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
{
	_target.draw(this->sprite, _states);
}