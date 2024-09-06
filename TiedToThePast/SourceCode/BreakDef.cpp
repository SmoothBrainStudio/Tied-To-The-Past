#include "Definitions.hpp"
#include "BreakDef.hpp"

BreakDef::BreakDef()
{
	this->effectName = "breakDef";
	//init sprite and texts to display effect and add it to drawManager
	statusDisplay.icon.setTexture(TexturesManager::Get()["DEF_BREAK"]);
	type = DEBUFF;
	id = Entity::Effects::DEF_BREAK;
}

void BreakDef::Init(Entity* _attacker, std::shared_ptr<Entity>& _target, const int& _nbTurn)
{
	this->target = _target;
	this->attacker = _attacker;
	this->nbTurn = _nbTurn;
	this->isOver = false;
	this->currentTurn = 0;
	nbTurnsLeft = nbTurn;
	stackable = false;
	nbStacks = 1;


	statusDisplay.nbTurnsLeft = CreateText(FontsManager::Get()["DEFAULT"], std::to_string(nbTurnsLeft));
	statusDisplay.nbTurnsLeft.setPosition(statusDisplay.icon.getGlobalBounds().width - statusDisplay.nbTurnsLeft.getGlobalBounds().width, statusDisplay.icon.getGlobalBounds().height - statusDisplay.nbTurnsLeft.getGlobalBounds().height);
	SetOutlined(statusDisplay.nbTurnsLeft, 1);

	statusDisplay.nbStacks = CreateText(FontsManager::Get()["DEFAULT"], std::to_string(nbStacks));
	statusDisplay.nbStacks.setPosition(0.f, statusDisplay.icon.getGlobalBounds().height - statusDisplay.nbTurnsLeft.getGlobalBounds().height);
	SetOutlined(statusDisplay.nbStacks, 1);

	isAppearing = true;
	isGoingBigger = true;

	target->AddResistanceCoef(DEF_BREAK_COEF);

	DrawManager::Get().AddObject("FIGHT_HUD", *this, false);
}

BreakDef* BreakDef::Clone() const
{
	return new BreakDef(*this);
}

void BreakDef::Run()
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

void BreakDef::Stop()
{
	isOver = true;
	//deleted from drawManager
	DrawManager::Get().DeleteObject("FIGHT_HUD", this);
	target->AddResistanceCoef(-DEF_BREAK_COEF); //reset resistance coef
}
