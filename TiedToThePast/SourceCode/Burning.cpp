#include "Definitions.hpp"
#include "Burning.hpp"

Burning::Burning()
{
	this->effectName = "burning";
	//init sprite and texts to display effect and add it to drawManager
	statusDisplay.icon.setTexture(TexturesManager::Get()["BURNING"]);
	type = DOT;
	id = Entity::Effects::BURNING;
}

void Burning::Init(Entity* _attacker, std::shared_ptr<Entity>& _target, const int& _nbTurn)
{
	this->target = _target;
	this->attacker = _attacker;
	this->nbTurn = _nbTurn;
	this->isOver = false;
	this->currentTurn = 0;
	this->nbTurnsLeft = nbTurn;
	stackable = false;
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

Burning* Burning::Clone() const
{
	return new Burning(*this);
}

void Burning::Run()
{
	if (target->GetHP() > 0)
	{
		int fireDamage = static_cast<int>(target->GetHPMax() * FIRE_COEF);
		this->target->AddHP(-fireDamage);

		std::string toDisplay = std::to_string(fireDamage);
		this->target->textDamages.push_back(std::make_shared<TextDamage>(toDisplay, this->target->getPosition() - sf::Vector2f(0.f, target->GetGlobalAABB().height / 4.f) + sf::Vector2f(Maths::PreciseRand(50.f, -50.f), -target->GetGlobalAABB().height)));

		target->GetParticles()[(int)Entity::ParticleTypes::BURNING]->setPosition(target->getPosition().x, target->getPosition().y - target->GetGlobalAABB().height / 2.f);
		target->GetParticles()[(int)Entity::ParticleTypes::BURNING]->Reset();
		target->GetParticles()[(int)Entity::ParticleTypes::BURNING]->Play();

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

void Burning::Stop()
{
	isOver = true;
	//deleted from drawManager
	DrawManager::Get().DeleteObject("FIGHT_HUD", this);
}
