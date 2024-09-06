#include "Definitions.hpp"
#include "CharacterMenu.hpp"

#pragma region MIRROR

void Mirror::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(background, states);
	target.draw(player, states);
	target.draw(reflection, states);
	target.draw(hearth, states);
	target.draw(candy, states);
	target.draw(healthText, states);
	target.draw(candyText, states);
}

void Mirror::Init(GameData& _gameData, std::shared_ptr<Player> _player)
{
	background.setSize(SIZE_BACKGROUND);
	background.setTexture(&texMng["INVENTORY_MIROR"]);

	reflection.setSize(SIZE_BACKGROUND);
	reflection.setTexture(&texMng["INVENTORY_MIROR_REFLECTION"]);

	healthText = CreateText(fntMng["SPOOKY"]);
	candyText = CreateText(fntMng["SPOOKY"]);
	SetOutlined(healthText, 2);
	SetOutlined(candyText, 2);
	healthText.setPosition(POSITION_HEALTH_TEXT);
	candyText.setPosition(POSITION_CANDY_TEXT);

	hearth.setTexture(texMng["INVENTORY_HEARTH"], true);
	candy.setTexture(texMng["INVENTORY_CANDY"], true);
	hearth.setOrigin((sf::Vector2f)texMng["INVENTORY_HEARTH"].getSize() / 2.f);
	candy.setOrigin((sf::Vector2f)texMng["INVENTORY_CANDY"].getSize() / 2.f);
	hearth.setPosition(POSITION_HEARTH);
	candy.setPosition(POSITION_CANDY);

	SetCostume(_player, _player->GetCostumeState());

	healthText.setString(std::to_string(_player->GetHP()));
	candyText.setString(std::to_string(_player->GetPossessedSweets()));
}

void Mirror::Update(std::shared_ptr<Player> _playerData)
{
	player.Update();

	float factorY = 1 + sin(glMng.GetTotalTime()) / 8.f;
	hearth.setScale(factorY, factorY);
	candy.setScale(factorY, factorY);
}

void Mirror::SetCostume(std::shared_ptr<Player> _player, const CostumeState& _costume)
{
	player = AnimatedSprite(*_player->GetAnims()[_costume][(int)PlayerState::IDLE]);
	player.Play();
	player.setScale(-1, 1);
	player.SetColor(sf::Color(255, 255, 255, 200));

	sf::FloatRect plyRect = player.GetSprite().getGlobalBounds();
	sf::FloatRect bgRect = background.getGlobalBounds();
	player.setOrigin(plyRect.width / 2.f, plyRect.height / 2.f);
	player.setPosition(bgRect.width / 2.f, bgRect.height / 2.f + 30.f);
}

const sf::FloatRect Mirror::GetGlobalBounds()
{
	sf::FloatRect result;
	sf::Vector2f position = getPosition();

	result.width = SIZE_BACKGROUND.x;
	result.height = SIZE_BACKGROUND.y;
	result.left = position.x;
	result.top = position.y;

	return result;
}

#pragma endregion

#pragma region SUITCASE

void Suitcase::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(background, states);
	for (int i = 0; i < 4; i++)
	{
		target.draw(objectBoxs[i], states);
	}
	target.draw(front2, states);
	for (int i = 4; i < 8; i++)
	{
		target.draw(objectBoxs[i], states);
	}
	target.draw(front1, states);
}

void Suitcase::Init(GameData& _gameData, std::shared_ptr<Player> _player)
{
	items.clear();
	background.setSize(SIZE_BACKGROUND);
	background.setTexture(&texMng["INVENTORY_SUITCASE"]);
	front1.setSize(SIZE_BACKGROUND);
	front1.setTexture(&texMng["INVENTORY_SUITCASE_FRONT_1"]);
	front2.setSize(SIZE_BACKGROUND);
	front2.setTexture(&texMng["INVENTORY_SUITCASE_FRONT_2"]);

	for (int i = 0; i < _player->GetItems().size(); i++)
	{
		const Item* curItem = _player->GetItems()[i].get();
		if (curItem->GetType() != Item::TIE && ((Consumable*)curItem)->GetCount() > 0)
		{
			items.push_back((Consumable*)curItem);
		}
	}

	for (int i = 0; i < 8; i++)
	{
		if (i < items.size())
		{		
			scrollValue = 0;

			sf::Vector2f size = (sf::Vector2f)items[i]->GetTexture()->getSize();
			objectBoxs[i].setTexture(*items[i]->GetTexture(), true);
			objectBoxs[i].setOrigin(size / 2.f);
		}
		else
		{
			objectBoxs[i].setTexture(sf::Texture(), true);
		}

		mouseCollision[i].width = SIZE_ONE_BOX.x;
		mouseCollision[i].height = SIZE_ONE_BOX.y;
		mouseCollision[i].left = getPosition().x + START_ORIGIN.x + (SIZE_ONE_BOX.x + SPACE_BETWEEN_BOXS) * (i % 4);
		mouseCollision[i].top = getPosition().y + START_ORIGIN.y + (SIZE_ONE_BOX.y + SPACE_BETWEEN_BOXS) * (float)std::floor(i / 4);
	}
}

void Suitcase::Update(GameData& _gameData, std::shared_ptr<Player> _playerData, DialogPlace& _dialog)
{
	sf::Vector2f mousePosition = scrMng.GetMousePosition(_gameData.window);

	for (int i = 0; i < 8; i++)
	{
		int valCurItem = i + scrollValue * 4;
		if (valCurItem < items.size() && mouseCollision[i].contains(mousePosition))
		{
			_dialog.SetTitle(items[valCurItem]->GetName());
			_dialog.SetDescription(items[valCurItem]->GetDescription());
			objectBoxs[i].setPosition(START_ORIGIN + SIZE_ONE_BOX / 2.f + sf::Vector2f((i % 4) * SIZE_ALL_BOX.x / 4.f, float(std::floor(i / 4)) * SIZE_ALL_BOX.y / 2.f));
		}
		else
		{
			objectBoxs[i].setPosition(START_ORIGIN + SIZE_ONE_BOX / 2.f + sf::Vector2f((i % 4) * SIZE_ALL_BOX.x / 4.f, float(std::floor(i / 4)) * SIZE_ALL_BOX.y / 2.f + OFFSET_OVER_MOUSE));
		}
	}
}

void Suitcase::PollEvent(sf::Event& _event)
{
	if (_event.type == sf::Event::MouseWheelMoved)
	{
		scrollValue = (unsigned short)Maths::Clamp(float((int)scrollValue - _event.mouseWheel.delta), 0.f, float(std::floor(((int)items.size() - 8) / 4)));

		for (int i = 0; i < 8; i++)
		{
			int valCurItem = i + scrollValue * 4;
			if (valCurItem < items.size())
			{
				sf::Vector2f size = (sf::Vector2f)items[valCurItem]->GetTexture()->getSize();
				objectBoxs[i].setTexture(*items[valCurItem]->GetTexture(), true);
				objectBoxs[i].setOrigin(size / 2.f);
			}
		}
	}
}

const sf::FloatRect Suitcase::GetGlobalBounds()
{
	sf::FloatRect result;
	sf::Vector2f position = getPosition();
	sf::Vector2f size = background.getSize();

	result.width = size.x;
	result.height = size.y;
	result.left = position.x;
	result.top = position.y;

	return result;
}

#pragma endregion

#pragma region TIESBOX

void TiesBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(background, states);
	for (int i = 0; i < 4; i++)
	{
		target.draw(tieBoxs[i], states);
	}
	target.draw(front, states);
}

void TiesBox::Init(GameData& _gameData, std::shared_ptr<Player> _player)
{
	background.setSize(BACKGROUND_SIZE);
	background.setTexture(&texMng["INVENTORY_TIES"]);
	front.setSize(BACKGROUND_SIZE);
	front.setTexture(&texMng["INVENTORY_TIES_FRONT"]);

	for (int i = 0; i < 4; i++)
	{
		tieBoxs[i].setOrigin(SIZE_BOX / 2.f);
	}

	tieBoxs[0].setTexture(texMng["CHARACTER_TIE"], true);
	for (int i = 0; i < _player->GetTies().size(); i++)
	{
		tieBoxs[(size_t)(i + 1)].setTexture(*_player->GetTies()[i]->GetTexture(), true);
	}

	for (int i = 0; i < 4; i++)
	{
		mouseCollision[i].width = SIZE_BOX_NORMAL.x;
		mouseCollision[i].height = SIZE_BOX_NORMAL.y;
		mouseCollision[i].left = getPosition().x + START_ORIGIN.x + (SIZE_BOX_NORMAL.x + SPACE_BETWEEN_BOXS) * (i % 4);
		mouseCollision[i].top = getPosition().y + START_ORIGIN.y + (SIZE_BOX_NORMAL.y + SPACE_BETWEEN_BOXS) * (float)std::floor(i / 4);
	}
}

void TiesBox::Update(GameData& _gameData, std::shared_ptr<Player> _playerData, DialogPlace& _dialog)
{
	sf::Vector2f mousePosition = scrMng.GetMousePosition(_gameData.window);

	for (int i = 0; i < 4; i++)
	{
		if (i < _playerData->GetTies().size() + 1 && mouseCollision[i].contains(mousePosition))
		{
			if (i == 0)
			{
				_dialog.SetTitle("Basic tie");
				_dialog.SetDescription("Boring ...");
			}
			else
			{
				std::shared_ptr curTie = _playerData->GetTies()[i - 1];
				_dialog.SetTitle(curTie->GetName() + " tie (Level " + std::to_string(curTie->GetLevel()) + ")");
				_dialog.SetDescription(curTie->GetDescription());
			}

			tieBoxs[i].setPosition(sf::Vector2f(SPACE_BETWEEN_BOXS + SIZE_BOX_NORMAL.x * i + i * SPACE_BETWEEN_BOXS, SPACE_BETWEEN_BOXS) + SIZE_BOX_NORMAL / 2.f);
		}
		else
		{
			tieBoxs[i].setPosition(sf::Vector2f(SPACE_BETWEEN_BOXS + SIZE_BOX_NORMAL.x * i + i * SPACE_BETWEEN_BOXS, SPACE_BETWEEN_BOXS + OFFSET_OVER_MOUSE) + SIZE_BOX_NORMAL / 2.f);
		}
	}
}

void TiesBox::PollEvent(GameData& _gameData, std::shared_ptr<Player> _player, Mirror& _mirror)
{
	if (_gameData.gameEvent.type == sf::Event::MouseButtonPressed)
	{
		for (int i = 0; i < 4; i++)
		{
			sf::FloatRect rect = tieBoxs[i].getGlobalBounds();
			sf::Vector2f position = getPosition();
			rect.left += position.x;
			rect.top += position.y;

			if (rect.contains(Screen::Get().GetMousePosition(_gameData.window)))
			{
				_player->SetCostumeState(CostumeState(i));
				_mirror.SetCostume(_player, CostumeState(i));
			}
		}
	}
}

const sf::FloatRect TiesBox::GetGlobalBounds()
{
	sf::FloatRect result;
	sf::Vector2f position = getPosition();
	sf::Vector2f size = background.getSize();

	result.width = size.x;
	result.height = size.y;
	result.left = position.x;
	result.top = position.y;

	return result;
}

#pragma endregion

#pragma region DIALOGPLACES

void DialogPlace::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(background, states);
	target.draw(title, states);
	target.draw(description, states);
}

void DialogPlace::Init(GameData& _gameData, std::shared_ptr<Player> _player)
{
	background.setSize(sf::Vector2f(1315, 224));
	background.setTexture(&texMng["INVENTORY_CHAT"]);

	title = CreateText(fntMng["SPOOKY"], "", 48);
	SetOutlined(title, 2);
	title.setPosition(POSITION_TITLE);

	description = CreateText(fntMng["SPOOKY"], "", 32);
	SetOutlined(description, 2);
	description.setPosition(POSITION_DESCRIPTION);
}

void DialogPlace::Update()
{
}

const sf::FloatRect DialogPlace::GetGlobalBounds()
{
	sf::FloatRect result;
	sf::Vector2f position = getPosition();
	sf::Vector2f size = background.getSize();

	result.width = size.x;
	result.height = size.y;
	result.left = position.x;
	result.top = position.y;

	return result;
}

void DialogPlace::SetTitle(const sf::String& _val)
{
	title.setString(_val);
}

void DialogPlace::SetDescription(const sf::String& _val)
{
	description.setString(_val);
}

#pragma endregion

#pragma region MENU

void CharacterMenu::LilbooGoto(const sf::Vector2f& _postition)
{
	lilbooTotalTime = 0.f;
	lilBooStartPosition = lilBoo.getPosition();
	lilBooFinalPosition = _postition;
	lilbooDistance = Maths::GetVectorLength(lilBooStartPosition, lilBooFinalPosition);
}

CharacterMenu::CharacterMenu() : player(nullptr), miror(nullptr)
{
	texMng.Add("Contents/Assets/Sprites/UI/Inventory/Mirror_Inventory.png", "INVENTORY_MIROR");
	texMng.Add("Contents/Assets/Sprites/UI/Inventory/Mirror_Reflection_And_Tape.png", "INVENTORY_MIROR_REFLECTION");
	texMng.Add("Contents/Assets/Sprites/UI/Inventory/Briefcase.png", "INVENTORY_SUITCASE");
	texMng.Add("Contents/Assets/Sprites/UI/Inventory/Briefcase_Front_1.png", "INVENTORY_SUITCASE_FRONT_1");
	texMng.Add("Contents/Assets/Sprites/UI/Inventory/Briefcase_Front_2.png", "INVENTORY_SUITCASE_FRONT_2");
	texMng.Add("Contents/Assets/Sprites/UI/Inventory/Tie_Box.png", "INVENTORY_TIES");
	texMng.Add("Contents/Assets/Sprites/UI/Inventory/Tie_Box_Front.png", "INVENTORY_TIES_FRONT");
	texMng.Add("Contents/Assets/Sprites/UI/Inventory/Inventory_Chat.png", "INVENTORY_CHAT");
	texMng.Add("Contents/Assets/Sprites/UI/Inventory/heath.png", "INVENTORY_HEARTH");
	texMng.Add("Contents/Assets/Sprites/UI/Inventory/Candy.png", "INVENTORY_CANDY");

	sdrMng.Add("Contents/Assets/Shaders/Blur.frag", "Contents/Assets/Shaders/Blur.vert", "BLUR");
	sdrMng["BLUR"].setUniform("sigma", 0.f);
	sdrMng["BLUR"].setUniform("textureSize", sf::Vector2f(scrMng.GetWidth(), scrMng.GetHeight()));

	DrawManager::Get().AddSection("CHARACTER_MENU");
}

CharacterMenu::~CharacterMenu()
{
	texMng.Delete("INVENTORY_MIROR");
	texMng.Delete("INVENTORY_MIROR_REFLECTION");
	texMng.Delete("INVENTORY_SUITCASE");
	texMng.Delete("INVENTORY_SUITCASE_FRONT_1");
	texMng.Delete("INVENTORY_SUITCASE_FRONT_2");
	texMng.Delete("INVENTORY_TIES");
	texMng.Delete("INVENTORY_TIES_FRONT");
	texMng.Delete("INVENTORY_HEARTH");
	texMng.Delete("INVENTORY_CANDY");

	delete miror;
	miror = nullptr;

	delete suitcase;
	suitcase = nullptr;

	delete tiesBox;
	tiesBox = nullptr;

	delete dialogPlace;
	dialogPlace = nullptr;

	DrawManager::Get().DeleteSection("CHARACTER_MENU");
}

void CharacterMenu::Init(GameData& _gameData)
{
	miror = new Mirror;
	miror->setPosition(POSITION_MIROR);

	suitcase = new Suitcase;
	suitcase->setPosition(POSITION_SUITCASE);

	tiesBox = new TiesBox;
	tiesBox->setPosition(POSITION_TIES);

	dialogPlace = new DialogPlace;
	dialogPlace->setPosition(POSITION_DIALOG);

	lilBoo = AnimatedSprite("Contents/Data/Animations/Entities/NPCs/LilBoo/idle_front.json");
	lilBoo.setPosition(POSITION_LILBOO_DIALOG);
	lilBoo.setOrigin(99, 92);
	lilBooFinalPosition = POSITION_LILBOO_DIALOG;
	lilBooStartPosition = lilBooFinalPosition;
	lilbooTotalTime = 0.f;
	lilbooDistance = 0.f;
	valueShader = 0.f;

	scrMng.SetWinShader(&sdrMng["BLUR"]);

	blackBackground.setFillColor(sf::Color(0, 0, 0, 80));
	blackBackground.setSize(sf::Vector2f(scrMng.GetWidth(), scrMng.GetHeight()));

	DrawManager::Get().AddObject("CHARACTER_MENU", blackBackground);
	DrawManager::Get().AddObject("CHARACTER_MENU", *miror);
	DrawManager::Get().AddObject("CHARACTER_MENU", *suitcase);
	DrawManager::Get().AddObject("CHARACTER_MENU", *tiesBox);
	DrawManager::Get().AddObject("CHARACTER_MENU", *dialogPlace);
	DrawManager::Get().AddObject("CHARACTER_MENU", lilBoo);
}

void CharacterMenu::InitOnOpen(GameData& _gameData, std::shared_ptr<Player> _player)
{
	miror->Init(_gameData, _player);
	suitcase->Init(_gameData, _player);
	tiesBox->Init(_gameData, _player);
	dialogPlace->Init(_gameData, _player);

	Audio::Get().PlaySound("OPEN_SELLING");
}

void CharacterMenu::Update(GameData& _gameData)
{
	if (blackBackground.getSize().x != Screen::Get().GetWidth() || blackBackground.getSize().y != Screen::Get().GetHeight())
	{
		blackBackground.setSize(sf::Vector2f(Screen::Get().GetWidth(), Screen::Get().GetHeight()));
		blackBackground.setSize(sf::Vector2f(Screen::Get().GetWidth(), Screen::Get().GetHeight()));
	}

	miror->Update(player);
	suitcase->Update(_gameData, player, *dialogPlace);
	tiesBox->Update(_gameData, player, *dialogPlace);
	dialogPlace->Update();

	lilBoo.Update();

	float dt = glMng.GetDeltaTime();

	lilbooTotalTime += dt;
	float factor = Maths::Clamp(lilbooTotalTime / (TIME_LILBOO_MOVEMENT * lilbooDistance), 0.f, 1.f);
	if (factor < 1)
	{
		float easing = Easings::InOutSine(factor);
		sf::Vector2f newPos = Maths::InterpolateVector(lilBooStartPosition, lilBooFinalPosition, easing);
		lilBoo.setPosition(newPos);
		lilBoo.setScale((lilBooStartPosition.x < lilBooFinalPosition.x ? -1.f : 1.f), 1.f);
	}
	else
	{
		lilBoo.setPosition(lilBooFinalPosition);
		lilBoo.setScale(1.f, 1.f);
	}

	valueShader = Maths::Clamp(valueShader + dt, 0, TIME_SHADER);
	sdrMng["BLUR"].setUniform("sigma", valueShader / TIME_SHADER * BLUR_MAX);

	sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(_gameData.window));
	if (miror->GetGlobalBounds().contains(mousePosition) && lilBooFinalPosition != POSITION_LILBOO_MIROR)
	{
		LilbooGoto(POSITION_LILBOO_MIROR);
		currentEvent = CurrentEvent::NONE;
	}
	if (suitcase->GetGlobalBounds().contains(mousePosition) && lilBooFinalPosition != POSITION_LILBOO_SUITCASE)
	{
		LilbooGoto(POSITION_LILBOO_SUITCASE);
		currentEvent = CurrentEvent::SUITCASE;
	}
	if (tiesBox->GetGlobalBounds().contains(mousePosition) && lilBooFinalPosition != POSITION_LILBOO_TIES)
	{
		LilbooGoto(POSITION_LILBOO_TIES);
		currentEvent = CurrentEvent::TIES;
	}
	if (dialogPlace->GetGlobalBounds().contains(mousePosition) && lilBooFinalPosition != POSITION_LILBOO_DIALOG)
	{
		LilbooGoto(POSITION_LILBOO_DIALOG);
		currentEvent = CurrentEvent::NONE;
	}
}

void CharacterMenu::PollEvent(GameData& _gameData)
{
	switch (currentEvent)
	{
	case CharacterMenu::CurrentEvent::SUITCASE:
		suitcase->PollEvent(_gameData.gameEvent);
		break;
	case CharacterMenu::CurrentEvent::TIES:
		tiesBox->PollEvent(_gameData, player, *miror);
		break;
	default:
		break;
	}
}

void CharacterMenu::Display(sf::RenderWindow& _window)
{
	_window.setView(_window.getDefaultView());
	scrMng.DrawWinShader(_window);
	drwMng.Draw(_window, "CHARACTER_MENU");
}

void CharacterMenu::SetPlayer(std::shared_ptr<Player> _player)
{
	player = _player;
}

void CharacterMenu::DisableShader()
{
	sdrMng["BLUR"].setUniform("sigma", 0.f);
	valueShader = 0.f;
}

#pragma endregion
