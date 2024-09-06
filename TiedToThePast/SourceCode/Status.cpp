#include "Status.hpp"

Status::Status()
{
	/*statusDisplay.icon.setPosition(-500.f, -500.f);*/
}

Status::~Status()
{
	DrawManager::Get().DeleteObject("FIGHT_HUD", this);
}

void Status::Erase()
{
	target->DeleteStatus(this->effectName);
}

void Status::Update(void)
{
	float dt = GameClock::Get().GetDeltaTime();

	if (isAppearing)
	{
		scaleTimer += dt;

		if (isGoingSmaller)
		{
			setScale(MAX_SCALE - Easings::InQuad(std::clamp(scaleTimer, 0.f, 1.f)) * (MAX_SCALE - MIN_SCALE), MAX_SCALE - Easings::InQuad(std::clamp(scaleTimer, 0.f, 1.f)) * (MAX_SCALE - MIN_SCALE));
			if (getScale().x <= MIN_SCALE)
			{
				setScale(MIN_SCALE, MIN_SCALE);
				//Debug.log(scaleTimer);
				scaleTimer = 0.f;
				isGoingSmaller = false;
				isAppearing = false;
			}
		}
		else if (isGoingBigger)
		{
			setScale(Easings::InQuad(std::clamp(scaleTimer, 0.f, 1.f)) * (MAX_SCALE - MIN_SCALE) + MIN_SCALE, Easings::InQuad(std::clamp(scaleTimer, 0.f, 1.f)) * (MAX_SCALE - MIN_SCALE) + MIN_SCALE);
			if (getScale().x >= MAX_SCALE)
			{
				setScale(MAX_SCALE, MAX_SCALE);
				scaleTimer = 0.f;
				isGoingBigger = false;
				isGoingSmaller = true;
			}
		}
	}
}

void Status::AddNewStack(void)
{
	if (stackable)
	{
		if (nbStacks < nbStacksMax)
		{
			nbStacks++;
		}

		ResetCooldown();
	}
}

void Status::AddTurns(const int& _turn)
{
	this->nbTurnsLeft += _turn;
	nbTurn += _turn;
	statusDisplay.nbTurnsLeft.setString(std::to_string(nbTurnsLeft));
	isAppearing = true;
	isGoingBigger = true;
}

void Status::ResetCooldown(void)
{
	if ((nbTurnsLeft != nbTurn) || (stackable && nbStacks < nbStacksMax))
	{
		isAppearing = true;
		isGoingBigger = true;
	}

	currentTurn = 0;
	nbTurnsLeft = nbTurn;
	statusDisplay.nbStacks.setString(std::to_string(nbStacks));
	statusDisplay.nbTurnsLeft.setString(std::to_string(nbTurnsLeft));
}

void Status::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(statusDisplay.icon, states);
	target.draw(statusDisplay.nbTurnsLeft, states);

	if (stackable)
	{
		target.draw(statusDisplay.nbStacks, states);
	}
}
