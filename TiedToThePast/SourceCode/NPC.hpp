#ifndef NPC__HPP
#define NPC__HPP

#include "Entity.hpp"
#include "Player.hpp"
#include "DialogBox.hpp"

static constexpr float DIALOG_BOX_OFFSET = 5.f;
static constexpr float DISTANCE_FOR_SPEAK = 70.f;

class NPC: public Entity
{
public:
	NPC() = default;
	NPC(const sf::Vector2f& _pos, const float& _speed, const std::string& _animFile, const std::string& _dialogFile = "NPC1.txt");
	NPC(const Json::Value& _mapNpcJson);
	~NPC() override;
	
	 virtual void Update(std::shared_ptr<Player>& _player);

	// Starting dialog when PJ is in boxCollision and INTERACT input is pressed
	virtual void Interact(sf::Event& _event, std::shared_ptr<Player>& _player);

	void ChangeDialog(const std::string& _file);

	// Change status of canSpeak boolean
	void SetCanSpeak(const bool& _bool);

	// Get status of canSpeak boolean
	const bool& GetIfCanSpeak(void) const;

	const bool& CanCollide(void) const;

	void UpdateShadow(void);

protected:
	Dialog* dialogList = nullptr;
	bool canSpeak;
	AnimatedSprite animation;
	bool canCollide = true;

	//void Move(std::shared_ptr<Player>& _player);
	void UpdateAnimations(void) override;
	
	// Configure sprite for NPC and draw manager 
	void InitNPCSprite(const sf::Vector2f& _pos, const std::string& _animFile, const std::string& _dialogFile);

	// Place the dialog box under the NPC
	void UpdateDialogBoxPos(void);
};

#endif // !NPC__HPP