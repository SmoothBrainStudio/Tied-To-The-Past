#include "Definitions.hpp"
#include "BreakAttack.hpp"

BreakAttack::BreakAttack()
{
	this->effectName = "breakAttack";
	//init sprite and texts to display effect and add it to drawManager
	statusDisplay.icon.setTexture(TexturesManager::Get()["ATK_BREAK"]);
	type = DEBUFF;
	id = Entity::Effects::ATK_BREAK;
}

void BreakAttack::Init(Entity* _attacker, std::shared_ptr<Entity>& _target, const int& _nbTurn)
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


	target->AddDamage(-static_cast<int>((float)target->GetInitialDamage() * ATK_BREAK_COEF));


	DrawManager::Get().AddObject("FIGHT_HUD", *this, false);
}

BreakAttack* BreakAttack::Clone() const
{
	return new BreakAttack(*this);
}

void BreakAttack::Run()
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

void BreakAttack::Stop()
{
	isOver = true;
	//deleted from drawManager
	DrawManager::Get().DeleteObject("FIGHT_HUD", this);
	target->AddDamage(static_cast<int>((float)target->GetInitialDamage() * ATK_BREAK_COEF));
}
