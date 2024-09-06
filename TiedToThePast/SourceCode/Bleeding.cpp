#include "Definitions.hpp"
#include "Bleeding.hpp"

Bleeding::Bleeding()
{
	this->effectName = "bleeding";
	//init sprite and texts to display effect and add it to drawManager
	statusDisplay.icon.setTexture(TexturesManager::Get()["BLEEDING"]);
	type = DOT;
	id = Entity::Effects::BLEEDING;
}

void Bleeding::Init(Entity* _attacker, std::shared_ptr<Entity>& _target, const int& _nbTurn)
{
	this->target = _target;
	this->attacker = _attacker;
	this->nbTurn = _nbTurn;
	this->isOver = false;
	this->currentTurn = 0;
	nbTurnsLeft = nbTurn;
	stackable = true;
	nbStacks = 1;

	

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

Bleeding* Bleeding::Clone() const
{
	return new Bleeding(*this);
}

void Bleeding::Run()
{
	if (target->GetHP() > 0)
	{
		int bleedingDamage = static_cast<int>(BLEEDING_DAMAGE * nbStacks);
		this->target->AddHP(-bleedingDamage);

		std::string toDisplay = std::to_string(bleedingDamage);
		this->target->textDamages.push_back(std::make_shared<TextDamage>(toDisplay, this->target->getPosition() - sf::Vector2f(0.f, target->GetGlobalAABB().height / 4.f) + sf::Vector2f(Maths::PreciseRand(50.f, -50.f), -target->GetGlobalAABB().height)));

		target->GetParticles()[(int)Entity::ParticleTypes::BLEEDING]->setPosition(target->getPosition().x, target->getPosition().y - target->GetGlobalAABB().height / 2.f);
		target->GetParticles()[(int)Entity::ParticleTypes::BLEEDING]->Reset();
		target->GetParticles()[(int)Entity::ParticleTypes::BLEEDING]->Play();

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

void Bleeding::Stop()
{
	isOver = true;
	//deleted from drawManager
	DrawManager::Get().DeleteObject("FIGHT_HUD", this);
}

