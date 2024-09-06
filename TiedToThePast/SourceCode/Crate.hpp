#ifndef CRATE_HPP
#define CRATE_HPP

#include "Common.hpp"
#include "Entity.hpp"
#include "Particles/ParticleSystem.hpp"

struct InGameDatas;

class Crate : public Entity
{
public:
	enum Type
	{
		CARDBOARD,
		WOOD,
		METAL
	};

	Crate(const Json::Value& _data);
	~Crate() override;

	void Update(InGameDatas& _inGameDatas);

	void SetItemIsDrop(const bool& _drop = true);

	const bool& GetIsDelete(void) const;
	const bool& GetIsDestroy(void) const;
	const bool& GetItemIsDrop(void) const;
	const std::string& GetItemName() const;
	const sf::FloatRect GetCollisionBox(void) const;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	static constexpr unsigned short int NB_PUNCHES_CARDBOARD = 1;
	static constexpr unsigned short int NB_PUNCHES_WOOD = 3;
	static constexpr unsigned short int NB_PUNCHES_METAL = 6;

	sf::Sprite sprite;
	bool isDelete = false;
	bool isDestroyed = false;
	bool itemIsDrop = false;
	bool isHit = false;
	float isHitTimer = 0.f;
	Crate::Type type = CARDBOARD;
	std::string itemName;
	std::string uniqueID;
	short int nbPunchesSupported = 0;
	ParticleSystem cratePieces = ParticleSystem("Contents/Data/Particles/Crate.json");

	void UpdateAnimations(void) override;
};

#endif // !CRATE_HPP


