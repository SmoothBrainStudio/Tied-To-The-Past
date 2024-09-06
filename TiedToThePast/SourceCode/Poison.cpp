#include "Definitions.hpp"
#include "Poison.hpp"

Poison::Poison()
{
	this->effectName = "poison";
	type = DOT;
	id = Entity::Effects::POISON;
}

void Poison::Init(Entity* _attacker, std::shared_ptr<Entity>& _target, const int& _nbTurn)
{
	this->target = _target;
	this->attacker = _attacker;
	this->nbTurn = _nbTurn;
	this->isOver = false;
	this->currentTurn = 0;
	nbTurnsLeft = nbTurn;
	stackable = true;
	nbStacks = 1;

	//init sprite and texts to display effect and add it to drawManager
	statusDisplay.icon.setTexture(TexturesManager::Get()["POISON"]);

	statusDisplay.nbTurnsLeft = CreateText(FontsManager::Get()["DEFAULT"], std::to_string(nbTurnsLeft));
	statusDisplay.nbTurnsLeft.setPosition(statusDisplay.icon.getGlobalBounds().width - statusDisplay.nbTurnsLeft.getGlobalBounds().width, statusDisplay.icon.getGlobalBounds().height - statusDisplay.nbTurnsLeft.getGlobalBounds().height);
	SetOutlined(statusDisplay.nbTurnsLeft, 1);

	statusDisplay.nbStacks = CreateText(FontsManager::Get()["DEFAULT"], std::to_string(nbStacks));
	statusDisplay.nbStacks.setPosition(0.f, statusDisplay.icon.getGlobalBounds().height - statusDisplay.nbTurnsLeft.getGlobalBounds().height);
	SetOutlined(statusDisplay.nbStacks, 1);

	DrawManager::Get().AddObject("FIGHT_HUD", *this, false);
	isAppearing = true;
	isGoingBigger = true;
}

Poison* Poison::Clone() const
{
	return new Poison(*this);
}

void Poison::Run()
{
	if (target->GetHP() > 0)
	{
		int poisonDamage = static_cast<int>(target->GetHPMax() * POISON_COEF * nbStacks);
		this->target->AddHP(-poisonDamage);

		std::string toDisplay = std::to_string(poisonDamage);
		this->target->textDamages.push_back(std::make_shared<TextDamage>(toDisplay, this->target->getPosition() - sf::Vector2f(0.f, target->GetGlobalAABB().height / 4.f) + sf::Vector2f(Maths::PreciseRand(50.f, -50.f), -target->GetGlobalAABB().height)));

		target->GetParticles()[(int)Entity::ParticleTypes::POISON]->setPosition(target->getPosition().x, target->getPosition().y - target->GetGlobalAABB().height / 2.f);
		target->GetParticles()[(int)Entity::ParticleTypes::POISON]->Reset();
		target->GetParticles()[(int)Entity::ParticleTypes::POISON]->Play();

		Audio::Get().PlaySound("AILMENT_HIT");
	}

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

void Poison::Stop()
{
	isOver = true;
	//deleted from drawManager
	DrawManager::Get().DeleteObject("FIGHT_HUD", this);
}
