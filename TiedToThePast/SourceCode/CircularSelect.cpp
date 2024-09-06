#include "Definitions.hpp"
#include "CircularSelect.hpp"
#include "Screen.hpp"
#include "DrawManager.hpp"
#include "Maths.hpp"
#include "Collisions.hpp"
#include "TexturesManager.hpp"
#include "Player.hpp"

CircularSelect::CircularSelect(sf::Texture& _texture, const int& _nbPieces, const std::vector<std::string>& _tiesName)
{
	TexturesManager& textureManager = TexturesManager::Get();
	DrawManager& drawManager = DrawManager::Get();

	textureManager.Add("Contents/Assets/Sprites/Fighting/Ties/UnknownTie.png", "UNKNOWN_TIE");
	textureManager.Add("Contents/Assets/Sprites/Fighting/Ties/CharacterTie.png", "CHARACTER_TIE");

	wheelBG.setTexture(textureManager["CIRCULAR_WHEEL"]);
	wheelBG.setOrigin(wheelBG.getGlobalBounds().width / 2.f, wheelBG.getGlobalBounds().height / 2.f);

	circularPieces.resize(_nbPieces);
	float factor = 360.f / _nbPieces;
	float angle = 0.f;

	for (auto& elem : circularPieces)
	{
		elem.sprite = AnimatedSprite("Contents/Data/Animations/Other/circularSelect.json");
		elem.sprite.Reset();

		if (elem.isSelected)
		{
			elem.sprite.SetCurrentFrame(elem.sprite.GetFrameCount() - 1);
		}
		else
		{
			elem.sprite.SetCurrentFrame(0);
		}
		elem.sprite.setOrigin(elem.sprite.GetSprite().getGlobalBounds().width / 2.f, elem.sprite.GetSprite().getGlobalBounds().height);
		elem.sprite.setRotation(angle);
		angle += factor;
	}

	this->setPosition(WORKING_WIDTH / 2.f, WORKING_HEIGHT / 2.f);

	tiesSpriteVect.resize(_nbPieces);

	angle = -90.f;

	nbTiesAvailable = static_cast<const short>(_tiesName.size());

	for (unsigned short int i = 0; i < tiesSpriteVect.size(); ++i)
	{
		if (i == 0)
		{
			tiesSpriteVect[i].setTexture(textureManager["CHARACTER_TIE"]);
		}
		else if (i > 0 && i <= _tiesName.size())
		{
			tiesSpriteVect[i].setTexture(textureManager["Tie " + _tiesName[i - 1]]);
		}
		else
		{
			tiesSpriteVect[i].setTexture(textureManager["UNKNOWN_TIE"]);
		}

		tiesSpriteVect[i].setOrigin(tiesSpriteVect[i].getGlobalBounds().width / 2.f, tiesSpriteVect[i].getGlobalBounds().height / 2.f);
		tiesSpriteVect[i].setScale(0.75f, 0.75f);
		tiesSpriteVect[i].setPosition(Maths::ProjectPoint(sf::Vector2f(WORKING_WIDTH / 2.f, WORKING_HEIGHT / 2.f), 175.f, Maths::DegreesToRadians(angle)));
		angle += factor;
	}

	drawManager.AddObject("HUD_CIRCULAR", *this, isVisible);

	for (auto& elem : tiesSpriteVect)
	{
		drawManager.AddObject("HUD_CIRCULAR", elem);
	}
}

CircularSelect::~CircularSelect()
{
	TexturesManager::Get().Delete("UNKNOWN_TIE");
	TexturesManager::Get().Delete("CHARACTER_TIE");

	DrawManager::Get().DeleteObject("HUD_CIRCULAR", this);

	for (auto& elem : tiesSpriteVect)
	{
		DrawManager::Get().DeleteObject("HUD_CIRCULAR", &elem);
	}
}

void CircularSelect::PollEvent(sf::Event& _event)
{
	
}

void CircularSelect::Update(sf::RenderWindow& _window)
{
	for (auto& elem : circularPieces)
	{
		sf::Vector2f tempOrigin = elem.sprite.getOrigin();
		float tempRotation = elem.sprite.getRotation();

		if (elem.isSelected)
		{
			elem.sprite.SetCurrentFrame(elem.sprite.GetFrameCount() - 1);
		}
		else
		{
			elem.sprite.SetCurrentFrame(0);
		}

		elem.sprite.setOrigin(tempOrigin);
		elem.sprite.setRotation(tempRotation);
	}

	if (isVisible)
	{
		float angleOffset = 360.f / circularPieces.size() * 2.5f;
		sf::Vector2f mousePos;
		ControlsManager& controlsM = ControlsManager::Get();

		if (controlsM.GetControllerType() != CONTROLLER)
		{
			mousePos = (sf::Vector2f)Screen::Get().GetMousePosition(_window);
			angleWithDevice = Maths::RadiansToDegrees(Maths::GetAngle(this->getPosition(), mousePos)) + angleOffset;
		}
		else
		{
			if (controlsM.CanJoystickInteract())
			{
				angleWithDevice = Maths::RadiansToDegrees(atan2f(controlsM.GetJoystickLeftAxis().y, controlsM.GetJoystickLeftAxis().x)) + angleOffset;
			}
		}

		// Make sure to have a positive number for the next division
		if (angleWithDevice < 0)
		{
			angleWithDevice += 360;
		}

		if (Collision::CirclePoint(circularPieces[0].sprite.GetSprite().getLocalBounds().height, this->getPosition(), mousePos) ||
			controlsM.GetControllerType() == CONTROLLER)
		{

			// Divide the circle in parts count to choose the right sprite depending on player's direction
			currentSelectedPiece = (int)abs(angleWithDevice / 360 * circularPieces.size());

			for (auto& elem : circularPieces)
			{
				elem.isSelected = false;
			}

			circularPieces[currentSelectedPiece].isSelected = true;

			if (currentSelectedPiece <= nbTiesAvailable)
			{
				tieIndex = currentSelectedPiece;
			}
			else
			{
				tieIndex = -1;
			}
		}
		else
		{
			for (auto& elem : circularPieces)
			{
				elem.isSelected = false;
			}
		}
	}

	PlaySwapingPieceSound();
}


void CircularSelect::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(wheelBG, states);

	for (const auto& elem : circularPieces)
	{
		target.draw(elem.sprite, states);
	}
}

void CircularSelect::PlaySwapingPieceSound(void)
{
	if (currentSelectedPiece != lastSelectedPiece)
	{
		audio.PlaySoundNoSecurity("BUTTON_SWAP");
	}

	// update last piece selected value
	lastSelectedPiece = currentSelectedPiece;
}

const bool& CircularSelect::GetIsVisible(void)
{
	return isVisible;
}

void CircularSelect::SetIsVisible(const bool& _visible)
{
	isVisible = _visible;
}

const int& CircularSelect::GetTieIndex(void) const
{
	return tieIndex;
}

void CircularSelect::SetTieIndex(const int& _index)
{
	tieIndex = _index;
}

void CircularSelect::ChangeTieTextures(std::shared_ptr<Player>& _player)
{
	TexturesManager& textureManager = TexturesManager::Get();
	auto ties = _player->GetTies();
	nbTiesAvailable = static_cast<const short>(ties.size());

	for (unsigned short int i = 0; i < tiesSpriteVect.size(); ++i)
	{
		if (i == 0)
		{
			tiesSpriteVect[i].setTexture(textureManager["CHARACTER_TIE"]);
		}
		else if (i > 0 && i <= ties.size())
		{
			tiesSpriteVect[i].setTexture(textureManager["Tie " + ties[(size_t)(i - 1)]->GetName()]);
		}
		else
		{
			tiesSpriteVect[i].setTexture(textureManager["UNKNOWN_TIE"]);
		}
	}
}
