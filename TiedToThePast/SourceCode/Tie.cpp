#include "Tie.hpp"

std::unordered_map<std::string, std::shared_ptr<Tie>> Tie::prefabs;

void Tie::LoadPrefabs()
{
	TexturesManager& texturesMng = TexturesManager::Get();
	Json::Value json = JsonManager::LoadFromFile(PREFABS_PATH);
	for (sf::Uint32 i = 0; i < json.size(); ++i)
	{
		std::shared_ptr<Tie> tie = std::make_shared<Tie>(json[i]);
		prefabs[tie->GetId()] = std::move(tie);
	}
}

void Tie::UnloadPrefabs()
{
	TexturesManager& texturesMng = TexturesManager::Get();
	Json::Value json = JsonManager::LoadFromFile(PREFABS_PATH);
	for (sf::Uint32 i = 0; i < json.size(); ++i)
	{
		std::string tieName = json[i]["Name"].asString();
		std::string tieId = "Tie " + tieName;
		texturesMng.Delete(tieId);
	}

	for (auto& tie : prefabs)
	{
		delete tie.second->miniGame;
		tie.second->miniGame = nullptr;
	}

	Tie::prefabs.clear();
}

Tie::Tie()
	: ID(CostumeState::BASIC)
{
	this->type = TIE;
	for (auto& resistance : resistances)
	{
		resistance = 1.f;
	}

	initialResistances = resistances;
}

Tie::Tie(Json::Value& _json)
{
	this->type = TIE;
	this->id = _json["ID"].asString();
	this->name = _json["Name"].asString();
	this->description = _json["Description"].asString();
	for (sf::Uint32 i = 0; i < _json["Upgrades"].size(); ++i)
	{
		this->upgrades.push_back({ _json["Upgrades"][i]["Price"].asInt(), _json["Upgrades"][i]["Description"].asString() });
	}
	std::string tieTextureId = "Tie " + this->name;
	TexturesManager::Get().Add(_json["Texture"].asString(), tieTextureId);
	this->SetTexture(TexturesManager::Get()[tieTextureId]);

	if (this->id == "BOXER")
	{
		this->SetMiniGame(new BoxerMiniGame());
	}
	else if(this->id == "CANDLE")
	{
		this->SetMiniGame(new CandleMiniGame());
	}
	else if (this->id == "MUSIC")
	{
		this->SetMiniGame(new MusicMiniGame());
	}

	for (const auto& resistance : _json["Resistances"])
	{
		this->SetResistance(resistance["id"].asInt(), resistance["coef"].asFloat());
	}

	initialResistances = resistances;

	for (const auto& effects : _json["effects"])
	{
		StatusProperties newStatusProperties = {};
		newStatusProperties.probability = effects["probability"].asFloat();
		newStatusProperties.id = effects["id"].asInt();
		this->StatutesProperties.push_back(newStatusProperties);
	}
	this->SetAttackType((AttackType)_json["AttackType"].asInt());
}

Tie::Tie(const std::string& _id)
{
	*this = *Tie::prefabs[_id];
}

Tie::~Tie()
{
}

void Tie::IncreaseLevel()
{
	if (!IsLevelMax())
	{
		++currentLevel;
	}
}

bool Tie::IsLevelMax() const
{
	return currentLevel == upgrades.size();
}

void Tie::SetMiniGame(MiniGame* _toSet)
{
	miniGame = _toSet;
}

sf::Uint32 Tie::GetLevel() const
{
	return currentLevel + 1;
}

int Tie::GetNextLevelPrice() const
{
	if (IsLevelMax())
	{
		return -1;
	}
	return upgrades[currentLevel].price;
}

const std::string Tie::GetNextLevelDescription() const
{
	if (IsLevelMax())
	{
		return "";
	}
	return upgrades[currentLevel].description;
}

MiniGame* Tie::GetMiniGame() const
{
	return miniGame;
}

const std::vector<Tie::StatusProperties>& Tie::GetStatutesProperties(void)
{
	return this->StatutesProperties;
}

const CostumeState& Tie::GetCostumeID(void)
{
	return ID;
}

void Tie::SetCostumeID(const CostumeState& _costumeState)
{
	ID = _costumeState;
}

void Tie::SetResistance(const int& _id, const float& _coef)
{
	resistances[_id] = _coef;
}

void Tie::SetAttackType(const AttackType& _attackType)
{
	attackType = _attackType;
}

const std::array<float, NB_ATTACK_TYPES>& Tie::GetResistances(void) const
{
	return resistances;
}

const AttackType& Tie::GetAttackType(void) const
{
	return attackType;
}

const float Tie::GetLevelCoef(void) const
{
	return ((float)currentLevel / (float)(upgrades.size() - 1)) * 0.5f + 1.f;
}

const float Tie::GetNextLevelCoef() const
{
	return ((float)(currentLevel + 1) / (float)(upgrades.size() - 1)) * 0.5f + 1.f;
}

const std::array<float, NB_ATTACK_TYPES>& Tie::GetInitialResistances(void) const
{
	return initialResistances;
}

void Tie::Save(std::ostream& _file)
{
	Item::Save(_file);
	_file << ' ' << this->currentLevel;
}

void Tie::Load(std::istream& _file)
{
	_file >> this->currentLevel;
}