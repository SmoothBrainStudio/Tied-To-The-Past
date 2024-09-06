#include "Definitions.hpp"
#include "NPC.hpp"

NPC::NPC(const sf::Vector2f& _pos, const float& _speed, const std::string& _animFile, const std::string& _dialogFile) : Entity()
{
	speed = _speed;
	canSpeak = false;

	InitNPCSprite(_pos, _animFile, _dialogFile);
}

NPC::NPC(const Json::Value& _mapNpcJson) : Entity()
{
	const std::string npcName = _mapNpcJson["class"].asString();
	const sf::Vector2f npcPos(_mapNpcJson["x"].asFloat() + _mapNpcJson["width"].asFloat() / 2.f, _mapNpcJson["y"].asFloat() + _mapNpcJson["height"].asFloat());

	speed = 300.f;
	canSpeak = false;

	std::string animatioPath = _mapNpcJson["animation"].asString();
	animatioPath = animatioPath.substr(animatioPath.find("/NPCs/") + sizeof("/NPCs/") - 1);

	std::string dialogsPath = _mapNpcJson["dialogs"].asString();
	if (!dialogsPath.empty())
	{
		dialogsPath = dialogsPath.substr(dialogsPath.find("/Dialogs/") + sizeof("/Dialogs/") - 1);
	}
	else
	{
		dialogsPath = "NPC1.txt";
	}

	canCollide = _mapNpcJson["collision"].asBool();
	InitNPCSprite(npcPos, animatioPath, dialogsPath);
}

NPC::~NPC()
{
	DrawManager::Get().DeleteObject("Exploration", this);
	DrawManager::Get().DeleteObject("ExplorationUnSortables", dialogList);
	delete dialogList;
}

void NPC::UpdateAnimations(void)
{
	curAnim->Update();
}

void NPC::InitNPCSprite(const sf::Vector2f& _pos, const std::string& _animFile, const std::string& _dialogFile)
{
	DrawManager& drawMng = DrawManager::Get();
	TexturesManager& TextursMng = TexturesManager::Get();

	// Init NPC Sprite
	animation = AnimatedSprite("Contents/Data/Animations/Entities/NPCs/" + _animFile);
	curAnim = &animation;
	curAnim->Update();

	setPosition(_pos);
	drawMng.AddObject("Exploration", *this);

	// Init dialog box sprite
	dialogList = new Dialog(_dialogFile);
	drawMng.AddObject("ExplorationUnSortables", *dialogList);

	// Init shadow
	shadow.setFillColor(sf::Color(0, 0, 0, 50));
	shadow.setScale(1.f, 0.5f);

	// Init collides + debug
	collideBox.setSize(sf::Vector2f(100, 200));
	collideBox.setOrigin(collideBox.getSize().x / 2.f, collideBox.getSize().y);

	feetHitbox.setSize(sf::Vector2f(GetFeetCollisionBox().width, GetFeetCollisionBox().height));
	feetHitbox.setOrigin(sf::Vector2f(feetHitbox.getSize().x / 2.f, feetHitbox.getSize().y));

	position.setOrigin(position.getRadius(), position.getRadius());
}

void NPC::Update(std::shared_ptr<Player>& _player)
{
	sf::FloatRect rect = curAnim->GetSprite().getGlobalBounds();
	setOrigin(rect.width / 2.f, rect.height);

	// Subs updates
	UpdateDialogBoxPos();
	UpdateAnimations();
	UpdateShadow();

	UpdateCollide();
}

void NPC::UpdateDialogBoxPos(void)
{
	sf::Vector2f boxPos = getPosition();
	boxPos.y -= GetLocalAABB().height - DIALOG_BOX_OFFSET;
	dialogList->Update();
	dialogList->setPosition(boxPos);
}

void NPC::Interact(sf::Event& _event, std::shared_ptr<Player>& _player)
{
	dialogList->PollEvent(_event);

	if ((ControlsManager::Get().IsPressedInput(_event, "INTERACT") || ControlsManager::Get().IsPressedInput(_event, "INTERACT_2")) && canSpeak)
	{
		if (!dialogList->IsPlaying())
		{
			_player->SetIsSpeaking(true);
			_player->SetEnablePhylactery(false);
			dialogList->StartDialog();
		}
	}

	if (dialogList->IsFinish())
	{
		_player->SetIsSpeaking(false);
		_player->SetEnablePhylactery(true);
		dialogList->Reset();
	}
}

void NPC::ChangeDialog(const std::string& _file)
{
	DrawManager::Get().DeleteObject("ExplorationUnSortables", dialogList);
	delete dialogList;

	dialogList = new Dialog(_file);
	DrawManager::Get().AddObject("ExplorationUnSortables", *dialogList);
}

const bool& NPC::CanCollide(void) const
{
	return canCollide;
}

void NPC::UpdateShadow(void)
{
	sf::FloatRect rect = GetGlobalAABB();

	shadow.setScale(1.f, 0.5f);
	shadow.setRadius(rect.width * 0.5f);
	shadow.setOrigin(shadow.getRadius(), shadow.getRadius());
	shadow.setPosition(getOrigin());
}

const bool& NPC::GetIfCanSpeak(void) const
{
	return canSpeak;
}

void NPC::SetCanSpeak(const bool& _bool)
{
	canSpeak = _bool;
}