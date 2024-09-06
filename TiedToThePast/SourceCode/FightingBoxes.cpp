#include "Definitions.hpp"
#include "FightingBoxes.hpp"

FightingBoxes::FightingBoxes(void)
{
}

FightingBoxes::~FightingBoxes(void)
{
	for (auto& elem : boxes)
	{
		DrawManager::Get().DeleteObject("FIGHT_BOXES", &elem);
	}
}

void FightingBoxes::Init(void)
{
	boxSelected = ActionType(0);

	boxes[1].setSize(sf::Vector2f(TEMPLATE_BOX_SIZE, TEMPLATE_BOX_SIZE));
	boxes[1].setOrigin(TEMPLATE_BOX_SIZE / 2.f, TEMPLATE_BOX_SIZE / 2.f);
	boxes[1].setPosition(PLAYER_FIGHT_POS - sf::Vector2f(0, BOX_Y_POSITION));

	boxes[0].setSize(boxes[1].getSize() * 0.75f);
	boxes[0].setOrigin(boxes[1].getOrigin() * 0.75f);
	boxes[0].setPosition(PLAYER_FIGHT_POS - sf::Vector2f(TEMPLATE_BOX_SIZE, BOX_Y_POSITION + TEMPLATE_BOX_SIZE / 4.f));
	boxes[0].setFillColor(sf::Color(255, 255, 255, 200));

	boxes[2].setSize(boxes[1].getSize() * 0.75f);
	boxes[2].setOrigin(boxes[1].getOrigin() * 0.75f);
	boxes[2].setPosition(PLAYER_FIGHT_POS - sf::Vector2f(-TEMPLATE_BOX_SIZE, BOX_Y_POSITION + TEMPLATE_BOX_SIZE / 4.f));
	boxes[2].setFillColor(sf::Color(255, 255, 255, 200));

	boxes[3].setSize(boxes[1].getSize() * 0.5f);
	boxes[3].setOrigin(boxes[1].getOrigin() * 0.5f);
	boxes[3].setPosition(PLAYER_FIGHT_POS - sf::Vector2f(0, BOX_Y_POSITION + TEMPLATE_BOX_SIZE / 2.f));
	boxes[3].setFillColor(sf::Color(255, 255, 255, 150));

	for (auto& elem : boxes)
	{
		DrawManager::Get().AddObject("FIGHT_BOXES", elem);
	}
}

void FightingBoxes::Event(sf::Event& _event, const size_t& _boxesSize)
{
	if (ControlsManager::Get().IsPressedInput(_event, "LEFT"))
	{
		boxSelected = boxSelected == 0 ? _boxesSize - 1: boxSelected - 1;
		Audio::Get().PlaySoundNoSecurity("BUTTON_SWAP");
	}
	else if (ControlsManager::Get().IsPressedInput(_event, "RIGHT"))
	{
		boxSelected = boxSelected == _boxesSize - 1 ? 0 : boxSelected + 1;
		Audio::Get().PlaySoundNoSecurity("BUTTON_SWAP");
	}
}

void FightingBoxes::Update(Player& _player, const size_t& _boxesSize)
{
	const float& dt = GameClock::Get().GetTotalTime();
	const float deltaPos1 = sin(dt * 4.f) * 5.f;
	const float deltaPos2 = sin(dt * 2.f) * 5.f;
	const float deltaPos3 = sin(dt) * 5.f;

	boxes[1].setPosition(PLAYER_FIGHT_POS - sf::Vector2f(0.f, BOX_Y_POSITION + deltaPos1));
	boxes[0].setPosition(PLAYER_FIGHT_POS - sf::Vector2f(TEMPLATE_BOX_SIZE, BOX_Y_POSITION + TEMPLATE_BOX_SIZE / 4.f + deltaPos2));
	boxes[2].setPosition(PLAYER_FIGHT_POS - sf::Vector2f(-TEMPLATE_BOX_SIZE, BOX_Y_POSITION + TEMPLATE_BOX_SIZE / 4.f + deltaPos2));
	boxes[3].setPosition(PLAYER_FIGHT_POS - sf::Vector2f(0.f, BOX_Y_POSITION + TEMPLATE_BOX_SIZE / 2.f + deltaPos3));

	if (ControlsManager::Get().IsPressedDirection("LEFT"))
	{
		boxSelected = boxSelected == 0 ? _boxesSize - 1 : boxSelected - 1;
		Audio::Get().PlaySoundNoSecurity("BUTTON_SWAP");
	}
	else if (ControlsManager::Get().IsPressedDirection("RIGHT"))
	{
		boxSelected = boxSelected == _boxesSize - 1 ? 0 : boxSelected + 1;
		Audio::Get().PlaySoundNoSecurity("BUTTON_SWAP");
	}
}

void FightingBoxes::UpdateActionTexture(Player& _player, const bool& _canRun)
{
	for (int i = 0; i < BOX_TYPE_SIZE; i++)
	{
		std::string textureKey;

		const ActionType boxType = ActionType((boxSelected + i + BOX_TYPE_SIZE - 1) % BOX_TYPE_SIZE);
		switch (boxType)
		{
		case ATTACK:
			textureKey = "ATTACK_BOX";
			break;
		case TIES:
			textureKey = _player.GetTies().size() > 1 ? "SELECT_BOX" : "SELECT_BOX_GREY";
			break;
		case RUN:
			textureKey = _canRun ? "RUN_BOX" : "RUN_BOX_GREY";
			break;
		case ITEMS:
			textureKey = _player.GetConsumables().size() > 0 ? "ITEM_BOX" : "ITEM_BOX_GREY";
			break;
		default:
			Debug.warning("Can't find select box texture : set to SELECT_BOX_GREY by default");
			textureKey = "SELECT_BOX_GREY";
			break;
		}
		boxes[i].setTexture(&TexturesManager::Get()[textureKey], true);
	}
}
void FightingBoxes::UpdateItemTexture(Player& _player)
{
	for (int i = 0; i < FightingBoxes::BOX_TYPE_SIZE; i++)
	{
		const int consumablesSize = (int)_player.GetConsumables().size();
		const int selectItems = (boxSelected + i + consumablesSize - 1) % consumablesSize;
		boxes[i].setTexture(_player.GetConsumables()[selectItems]->GetTexture(), true);
	}
}
void FightingBoxes::UpdateTieTexture(Player& _player)
{
	auto playerTies = _player.GetTies();

	const size_t tiesSize = playerTies.size();
	for (size_t i = 0; i < FightingBoxes::BOX_TYPE_SIZE; i++)
	{
		const size_t selectTie = (boxSelected + i - 1) % tiesSize;
		const sf::Texture* const iconTexture = playerTies[selectTie]->GetTexture();
		boxes[i].setTexture(iconTexture, true);
	}
}
void FightingBoxes::UpdatePartitionTexture(Player& _player)
{
	auto& applicableEffects = _player.GetApplicableEffects();

	const size_t applicableEffectsSize = applicableEffects.size();
	for (int i = 0; i < FightingBoxes::BOX_TYPE_SIZE; i++)
	{
		const size_t selectEffect = (boxSelected + i - 1) % applicableEffectsSize;
		const sf::Texture *const iconTexture = applicableEffects[selectEffect].status->statusDisplay.icon.getTexture();
		boxes[i].setTexture(iconTexture, true);
	}
}

void FightingBoxes::SetSelectedBox(const int& _var)
{
	boxSelected = _var;
}
const size_t& FightingBoxes::GetSelectedBox(void) const
{
	return boxSelected;
}

void FightingBoxes::SetVisible(const bool& _visible)
{
	for (auto& box : boxes)
	{
		DrawManager::Get().SetObjectVisible("FIGHT_BOXES", &box, _visible);
	}
}