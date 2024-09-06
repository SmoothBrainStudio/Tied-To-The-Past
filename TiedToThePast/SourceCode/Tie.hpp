#ifndef TIE__HPP
#define TIE__HPP

#include "Common.hpp"
#include "Item.hpp"
#include "Fighting/CandleMiniGame.hpp"
#include "Fighting/BoxerMiniGame.hpp"
#include "Fighting/MusicMiniGame.hpp"

class Tie : public Item
{

private:
	struct StatusProperties
	{
		int id;
		float probability;
	};
public:
	
	constexpr static char PREFABS_PATH[] = "Contents/Data/Ties.json";
	static void LoadPrefabs();
	static void UnloadPrefabs();
	static std::unordered_map<std::string, std::shared_ptr<Tie>> prefabs;

	Tie();
	Tie(Json::Value& _value);
	Tie(const std::string& _id);
	~Tie();

	void IncreaseLevel();
	bool IsLevelMax() const;
	void SetMiniGame(MiniGame* _toSet);

	sf::Uint32 GetLevel() const;
	int GetNextLevelPrice() const;
	const std::string GetNextLevelDescription() const;
	MiniGame* GetMiniGame() const;
	const std::vector<StatusProperties>& GetStatutesProperties(void);
	const CostumeState& GetCostumeID(void);
	void SetCostumeID(const CostumeState& _costumeState);

	void SetResistance(const int& _id, const float& _coef);
	void SetAttackType(const AttackType& _attackType);
	const std::array<float, NB_ATTACK_TYPES>& GetResistances(void) const;
	const AttackType& GetAttackType(void) const;
	const float GetLevelCoef(void) const;
	const float GetNextLevelCoef() const;
	const std::array<float, NB_ATTACK_TYPES>& GetInitialResistances(void) const;

	void Save(std::ostream& _file) override;
	void Load(std::istream& _file) override;

private:
	struct Upgrade
	{
		int price;
		std::string description;
	};

	sf::Uint32 currentLevel = 0;
	std::vector<Upgrade> upgrades;
	std::vector<StatusProperties> StatutesProperties;
	MiniGame* miniGame = nullptr;
	CostumeState ID;

	AttackType attackType = AttackType::PHYSIC;
	std::array<float, NB_ATTACK_TYPES> resistances = std::array<float, NB_ATTACK_TYPES>();
	std::array<float, NB_ATTACK_TYPES> initialResistances = std::array<float, NB_ATTACK_TYPES>();
};

#endif //!TIE__HPP
