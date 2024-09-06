#include "Definitions.hpp"
#include "SellerNPC.hpp"

SellerNPC::SellerNPC(const sf::Vector2f& _pos, const float& _speed, const std::string& _animFile) : MenuNPC(_pos, _speed, _animFile)
{
	SetEventKey("GotoShop");
}

SellerNPC::~SellerNPC()
{
	//DrawManager::Get().DeleteObject("Exploration", this);
}

void SellerNPC::Interact(sf::Event& _event, std::shared_ptr<Player>& _player)
{
	this->dialogList->PollEvent(_event);

	if ((ControlsManager::Get().IsPressedInput(_event, "INTERACT") || ControlsManager::Get().IsPressedInput(_event, "INTERACT_2"))&& this->canSpeak)
	{
		if (!this->dialogList->IsPlaying())
		{
			if (_player->GetPossessedSweets() > MIN_CANDIES_TO_TALK)
			{
				EventManager::Get()[eventKey];
			}
			else
			{
				this->ChangeDialog("SellerNoCandies.txt");
				_player->SetIsSpeaking(true);
				_player->SetEnablePhylactery(false);
				this->dialogList->StartDialog();
			}
		}
	}

	if (dialogList->IsFinish())
	{
		_player->SetIsSpeaking(false);
		_player->SetEnablePhylactery(true);
		dialogList->Reset();
	}
}