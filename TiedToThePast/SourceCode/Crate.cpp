#include "Definitions.hpp"
#include "Crate.hpp"
#include "InGameDatas.hpp"

Crate::Crate(const Json::Value& _data)
{
	itemName = _data["item"].asString();
	uniqueID = _data["uniqueID"].asString();

	const auto& boxMaterial = _data["material"];
	if (boxMaterial == "CARDBOARD")
	{
		sprite.setTexture(TexturesManager::Get()["CRATE"]);
		nbPunchesSupported = NB_PUNCHES_CARDBOARD;
		type = Crate::CARDBOARD;
	}
	else if (boxMaterial == "WOOD")
	{
		sprite.setTexture(TexturesManager::Get()["CRATE"]);
		nbPunchesSupported = NB_PUNCHES_WOOD;
		type = Crate::WOOD;
	}
	else if (boxMaterial == "METAL")
	{
		sprite.setTexture(TexturesManager::Get()["CRATE"]);
		nbPunchesSupported = NB_PUNCHES_METAL;
		type = Crate::METAL;
	}
	else
	{
		Debug.warning("Invalide type for ITEM_BOX : ", boxMaterial);
	}

	setOrigin(sprite.getGlobalBounds().width / 2.f, sprite.getGlobalBounds().height / 2.f);
	cratePieces.setPosition(getPosition() + getOrigin());
	setPosition(sf::Vector2f(_data["x"].asFloat() + _data["width"].asFloat() / 2.f, _data["y"].asFloat() + _data["height"].asFloat() / 2.f));

	//à modifier si ça devient une animation
	collideBox.setSize(sf::Vector2f(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height));
	collideBox.setPosition(getPosition());
	collideBox.setFillColor(sf::Color::Transparent);
	collideBox.setOutlineColor(sf::Color::Blue);
	collideBox.setOutlineThickness(1.f);
	collideBox.setOrigin(getOrigin());

	DrawManager::Get().AddObject("Exploration", *this);
}

Crate::~Crate()
{
	DrawManager::Get().DeleteObject("Exploration", &sprite);
	DrawManager::Get().DeleteObject("Exploration", this);
}

void Crate::Update(InGameDatas& _inGameDatas)
{
	if (!isDestroyed)
	{
		sf::FloatRect rect = sprite.getGlobalBounds();
		rect.left = getPosition().x - getOrigin().x;
		rect.top = getPosition().y - getOrigin().y;

		if (_inGameDatas.player->GetCostumeState() == CostumeState::BOXER &&
			Collision::AABBs(_inGameDatas.player->GetPunchCollider().getGlobalBounds(), rect) &&
			_inGameDatas.player->GetIsPunching() && !isHit && _inGameDatas.player->GetIsInPunchingFrame())
		{
			nbPunchesSupported--;
			isHit = true;
			Audio::Get().PlaySoundNoSecurity("PUNCH_HIT");

			if (nbPunchesSupported <= 0)
			{
				BoolManager::Get()[uniqueID] = true; // pass crateID to true to never spawn it again
				cratePieces.Play();
				isDestroyed = true;
				Audio::Get().PlaySoundNoSecurity("CRATE_BREAK_SMALL");
			}
		}

		if (isHit)
		{
			isHitTimer += GameClock::Get().GetDeltaTime();
			if (isHitTimer >= 0.3f)
			{
				isHit = false;
				isHitTimer = 0.f;
			}
		}
	}
	else
	{
		cratePieces.Update();

		if (cratePieces.GetStatue() == ParticleSystem::Statue::STOPPED)
		{
			isDelete = true;
		}
	}

	collideBox.setPosition(getPosition());
}

void Crate::SetItemIsDrop(const bool& _drop)
{
	itemIsDrop = _drop;
}

const bool& Crate::GetIsDelete(void) const
{
	return isDelete;
}

const bool& Crate::GetIsDestroy(void) const
{
	return isDestroyed;
}

const bool& Crate::GetItemIsDrop(void) const
{
	return itemIsDrop;
}

const std::string& Crate::GetItemName() const
{
	return itemName;
}

const sf::FloatRect Crate::GetCollisionBox(void) const
{
	sf::FloatRect rect = sprite.getGlobalBounds();
	rect.left = getPosition().x - getOrigin().x;
	rect.top = getPosition().y - getOrigin().y;

	rect.top += rect.height / 2.f;
	rect.height /= 2.f;

	return rect;
}

void Crate::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	if (!isDestroyed)
	{
		target.draw(shadow, states);
		target.draw(sprite, states);
	}

	target.draw(cratePieces, states);
}

void Crate::UpdateAnimations(void)
{
}