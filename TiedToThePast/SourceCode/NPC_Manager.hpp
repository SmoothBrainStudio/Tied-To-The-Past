#ifndef NPC_MANAGER__HPP
#define NPC_MANAGER__HPP

#include "SellerNPC.hpp"
#include "CollisionManager.hpp"

class NPC_Manager
{
public:
	NPC_Manager() = default;
	~NPC_Manager() = default;

	void Reset(void);
	void Update(std::shared_ptr<Player>& _player);
	void Interact(sf::Event& _event, std::shared_ptr<Player>& _player);
	template<typename T>
	T* AddNPC(sf::Vector2f _pos, float _speed, const std::string& _textureKey)
	{
		std::shared_ptr<T> newNPC = std::make_shared<T>(_pos, _speed, _textureKey);
		NPCVector.push_back(std::move(newNPC));
		return (T*)NPCVector.back().get();
	}
	template<typename T>
	T* AddNPC(const Json::Value& _json)
	{
		std::shared_ptr<T> newNPC = std::make_shared<T>(_json);
		NPCVector.push_back(std::move(newNPC));
		return (T*)NPCVector.back().get();
	}
	std::vector<std::shared_ptr<NPC>>& GetNPCVector(void);
	void ResolveNPCPlayerCollision(std::shared_ptr<Player>& _player);

	const bool IsAnyOneCanSpeak(void) const;

private:
	std::vector<std::shared_ptr<NPC>> NPCVector;

	void UpdateSpeakingPossibility(std::shared_ptr<NPC>& _NPC, std::shared_ptr<Player>& _player);
	void DrawOrNotPhilactery(std::shared_ptr<Player>& _player);

	bool wasCollide = false;
};

#endif // !NPC_MANAGER__HPP