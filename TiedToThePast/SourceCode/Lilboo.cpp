#include "Definitions.hpp"
#include "Lilboo.hpp"

Lilboo::Lilboo(const Json::Value& _mapNpcJson) : NPC(_mapNpcJson)
{
	collideBox.setSize(sf::Vector2f(100.f, 184.f));
	collideBox.setOrigin(collideBox.getSize().x / 2.f, collideBox.getSize().y);

	boolNeeded = _mapNpcJson["boolNeeded"].asString();

	// Init shadow
	shadow.setFillColor(sf::Color(0, 0, 0, 50));
}

void Lilboo::Update(Player& _player)
{
	if (boolNeeded.empty() || BoolManager::Get()[boolNeeded])
	{
		DrawManager::Get().SetObjectVisible("Exploration", this, true);

		UpdateAnimations();
		UpdateShadow();

		sf::FloatRect rect = curAnim->GetSprite().getGlobalBounds();
		setOrigin(rect.width / 2.f, rect.height);

		if (isFollowing)
		{
			_player.SetEnablePhylactery(false); // to prevent phylactery showing when dialog/scenematic playing

			sf::Vector2f boxPos = getPosition();
			boxPos.y -= GetLocalAABB().height - DIALOG_BOX_OFFSET;
			dialogMap->SetPos(boxPos);
			MoveTowardsPlayer(_player);
		}
	}
	else
	{
		DrawManager::Get().SetObjectVisible("Exploration", this, false);
	}
}

void Lilboo::SetDialog(DialogMap& _dialogMap, Player& _player)
{
	isFollowing = true;
	dialogMap = &_dialogMap;

	_player.SetEnablePhylactery(false);
	_player.SetIsSpeaking(true);
}

void Lilboo::MoveTowardsPlayer(Player& _player)
{
	if (Maths::GetSquaredLength(_player.getPosition(), getPosition()) > LENGHT_TALKING * LENGHT_TALKING)
	{
		const float dt = GameClock::Get().GetDeltaTime();
		const float angleWithPlayer = Maths::GetAngle(getPosition(), _player.getPosition());
		setScale((abs(Maths::RadiansToDegrees(angleWithPlayer)) > 45.f ? 1.f : -1.f), 1.f);
		setPosition(getPosition().x + cosf(angleWithPlayer) * speed * dt, getPosition().y + sinf(angleWithPlayer) * speed * dt);
	}
	else
	{
		isFollowing = false;

		sf::Vector2f boxPos = getPosition();
		boxPos.y -= GetGlobalAABB().height - DIALOG_BOX_OFFSET;
		dialogMap->SetPos(boxPos);
		if (!dialogMap->IsPlaying())
		{
			dialogMap->StartDialog(_player);
			_player.SetIsSpeaking(true);
			_player.SetEnablePhylactery(false);
		}
	}
}

const bool Lilboo::IsVisible(void)
{
	return boolNeeded.empty() || BoolManager::Get()[boolNeeded];
}