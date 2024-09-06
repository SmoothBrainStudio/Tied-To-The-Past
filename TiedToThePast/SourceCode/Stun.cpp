#include "Definitions.hpp"
#include "Stun.hpp"

Stun::Stun()
{
	this->effectName = "Stun";
	//init sprite and texts to display effect and add it to drawManager
	statusDisplay.icon.setTexture(TexturesManager::Get()["STUN"]);
	type = DEBUFF;
	id = Entity::Effects::STUN;
}

void Stun::Init(Entity* _attacker, std::shared_ptr<Entity>& _target, const int& _nbTurn)
{
	this->target = _target;
	this->attacker = _attacker;
	this->nbTurn = _nbTurn;
	this->isOver = false;
	this->currentTurn = 0;
	nbTurnsLeft = nbTurn;
	stackable = false;
	nbStacks = 1;
	_target->SetStun(true);

	statusDisplay.nbTurnsLeft = CreateText(FontsManager::Get()["DEFAULT"], std::to_string(nbTurnsLeft));
	statusDisplay.nbTurnsLeft.setPosition(statusDisplay.icon.getGlobalBounds().width - statusDisplay.nbTurnsLeft.getGlobalBounds().width, statusDisplay.icon.getGlobalBounds().height - statusDisplay.nbTurnsLeft.getGlobalBounds().height);
	SetOutlined(statusDisplay.nbTurnsLeft, 1);

	statusDisplay.nbStacks = CreateText(FontsManager::Get()["DEFAULT"], std::to_string(nbStacks));
	statusDisplay.nbStacks.setPosition(0.f, statusDisplay.icon.getGlobalBounds().height - statusDisplay.nbTurnsLeft.getGlobalBounds().height);
	SetOutlined(statusDisplay.nbStacks, 1);

	isAppearing = true;
	isGoingBigger = true;

	DrawManager::Get().AddObject("FIGHT_HUD", *this, false);
}

Stun* Stun::Clone() const
{
    return new Stun(*this);
}

void Stun::Run()
{
	currentTurn++;
	nbTurnsLeft--;

	//updating texts
	statusDisplay.nbTurnsLeft.setString(std::to_string(nbTurnsLeft));

	if (nbTurn == currentTurn)
	{
		Stop();
		Erase();
	}
}

void Stun::Stop()
{
	target->SetStun(false);
	isOver = true;
	//deleted from drawManager
	DrawManager::Get().DeleteObject("FIGHT_HUD", this);
}
