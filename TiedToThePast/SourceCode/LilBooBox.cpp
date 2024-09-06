#include "Definitions.hpp"
#include "InGameDatas.hpp"
#include "LilBooBox.hpp"

LilBooBox::LilBooBox(void)
{
	animation = AnimatedSprite("Contents/Data/Animations/Entities/NPCs/LilBoo/idle_front.json");
	curAnim = &animation;
	curAnim->Update();

	sf::FloatRect rect = animation.GetSprite().getGlobalBounds();
	setOrigin(rect.top + rect.width / 2.f, rect.left + rect.height / 2.f);

	DrawManager::Get().AddObject("ExplorationUnSortables", *this);
	DrawManager::Get().SetObjectVisible("ExplorationUnSortables", this, false);
}

LilBooBox::~LilBooBox(void)
{
	DrawManager::Get().DeleteObject("ExplorationUnSortables", this);
}

void LilBooBox::AddBox(DialogMap& _dialogMap)
{
	dialogsMap.push_back(&_dialogMap);
}

void LilBooBox::Update(Player& _player)
{
	sf::FloatRect playerRect = _player.GetGlobalAABB();
	sf::Vector2f playerPos = _player.getPosition();
	playerPos.y -= playerRect.height;
	playerPos.x += playerRect.width * (_player.GetXDir() ? 1.f : -1.f) * 1.25f;
	setPosition(playerPos);

	setScale((_player.GetXDir() ? 1.f : -1.f), 1.f);

	bool isPlaying = false;
	for (auto& box : dialogsMap)
	{
		playerPos.y -= DIALOG_BOX_OFFSET;
		box->SetPos(playerPos);

		if (box->Ready())
		{
			box->StartDialog(_player);
		}
		if (box->IsPlaying())
		{
			isPlaying = true;
			break;
		}
	}

	DrawManager::Get().SetObjectVisible("ExplorationUnSortables", this, isPlaying);
	curAnim->Update();
}

void LilBooBox::Clear(void)
{
	dialogsMap.clear();
}