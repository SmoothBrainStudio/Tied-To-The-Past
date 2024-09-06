#include "Definitions.hpp"
#include "LilbooDialog.hpp"
#include "InGameDatas.hpp"

LilbooDialog::LilbooDialog(const Json::Value& _mapNpcJson) : NPC(_mapNpcJson)
{
	collideBox.setSize(sf::Vector2f(100, 184));
	collideBox.setOrigin(collideBox.getSize().x / 2.f, collideBox.getSize().y);

	boolNeeded = _mapNpcJson["boolNeeded"].asString();

	// Init shadow
	shadow.setFillColor(sf::Color(0, 0, 0, 50));
}

LilbooDialog::~LilbooDialog()
{
	/*for (auto& dialogRect : dialogRects)
	{
		DrawManager::Get().DeleteObject("ExplorationUnSortables", &dialogRects[i].rect);

	}*/
}

void LilbooDialog::Update(std::shared_ptr<Player>& _player)
{
	if (boolNeeded.empty() || BoolManager::Get()[boolNeeded])
	{
		DrawManager::Get().SetObjectVisible("Exploration", this, true);

		UpdateDialogBoxPos();
		UpdateAnimations();
		UpdateShadow();

		sf::FloatRect rect = curAnim->GetSprite().getGlobalBounds();
		setOrigin(rect.width / 2.f, rect.height);

		StartNewDialog(_player, "LilbooNPC" + std::to_string(dialCount) + ".txt");
	}
	else
	{
		DrawManager::Get().SetObjectVisible("Exploration", this, false);
	}
}

void LilbooDialog::StartNewDialog(std::shared_ptr<Player>& _player, const std::string& _file)
{
	for (int i = 0; i < dialogRects.size(); i++)
	{
		if (Collision::AABBs(dialogRects[i]->rect.getGlobalBounds(), _player->GetFeetCollisionBox()))
		{
			isAppears = true;
			ChangeDialog(dialogRects[i]->file);
			dialCount++;
			BoolManager::Get()[dialogRects[i]->uniqueID] = true;

			if (!dialogRects[i]->unlockBool.empty())
			{
				BoolManager::Get()[dialogRects[i]->unlockBool] = true;
			}

			if (!dialogRects[i]->addTie.empty())
			{
				for (auto& tie : Tie::prefabs)
				{
					if (tie.first == dialogRects[i]->addTie)
					{
						_player->AddTie(tie.second);
						_player->SetCostumeState((CostumeState)(_player->GetCostumeState() + 1));
						break;
					}
				}
			}

			dialogRects.erase(dialogRects.begin() + i);
			break;
		}
	}

	if (isAppears)
	{
		MoveTowardsPlayer(_player);
	}
	else
	{
		if (startDialog)
		{
			if (dialogList->GetStatus() != Dialog::PLAYING)
			{
				_player->SetIsSpeaking(true);
				dialogList->StartDialog();
				startDialog = false;
			}
		}
	}
}

void LilbooDialog::AddDialogRect(const Json::Value& _object)
{
	dialogRects.push_back(std::make_unique<DialogRect>(_object));
}

void LilbooDialog::MoveTowardsPlayer(std::shared_ptr<Player>& _player)
{
	if (Maths::GetSquaredLength(_player->getPosition(), getPosition()) > 150.f * 150.f)
	{
		const float dt = GameClock::Get().GetDeltaTime();
		angleWithPlayer = Maths::GetAngle(getPosition(), _player->getPosition());
		setPosition(getPosition().x + cosf(angleWithPlayer) * speed * dt, getPosition().y + sinf(angleWithPlayer) * speed * dt);
	}
	else
	{
		isAppears = false;
		startDialog = true;
	}
}
