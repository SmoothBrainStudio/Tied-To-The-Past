#include "Definitions.hpp"
#include "Tutorial.hpp"

Tutorial::Tutorial(const std::vector<std::string>& _tutoTextureKeys, const std::vector<std::string>& _descriptionPaths, const std::string& _keyDraw)
{
	tutoTextureKeys = _tutoTextureKeys;
	nbPages = (unsigned int)tutoTextureKeys.size();
	nbBasePages = nbPages;
	curPage = 1;
	keyDraw = _keyDraw;
	nbPagesText = CreateText(FontsManager::Get()["SPOOKY"], std::to_string(curPage));
	nbPagesText.setCharacterSize(55);
	SetCenteredText(nbPagesText);
	SetOutlined(nbPagesText, 2);

	screenShot.setTexture(TexturesManager::Get()[tutoTextureKeys[(size_t)(curPage - 1)]]);
	screenShot.setOrigin(screenShot.getGlobalBounds().width / 2.f, screenShot.getGlobalBounds().height / 2.f);

	setPosition(WORKING_WIDTH / 2.f, WORKING_HEIGHT / 2.f - 100.f);

	InitButtons();

	descriptions.resize(_descriptionPaths.size());
	for (unsigned int i = 0; i < descriptions.size(); i++)
	{
		descriptions[i] = new Dialog(_descriptionPaths[i]);
		descriptions[i]->setOrigin(descriptions[i]->GetSize().x / 2.f, descriptions[i]->GetSize().y / 2.f);
		descriptions[i]->setPosition(getPosition().x, getPosition().y + screenShot.getGlobalBounds().height / 2.f + TEXT_OFFSET);
		descriptions[i]->SetDialogDisappearing(false);
		DrawManager::Get().AddObject(keyDraw, *descriptions[i], false);
	}

	descriptions[(size_t)(curPage - 1)]->StartDialog();

	nbPagesText.setPosition(getPosition().x, arrows[LEFT]->getPosition().y);

	DrawManager::Get().AddObject(keyDraw, nbPagesText);

	for (auto& arrow : arrows)
	{
		DrawManager::Get().AddObject(keyDraw, *arrow);
	}

	DrawManager::Get().SetObjectVisible(keyDraw, descriptions[(size_t)(curPage - 1)], true);
}

Tutorial::~Tutorial()
{
	while (!descriptions.empty())
	{
		delete descriptions.back();
		descriptions.pop_back();
	}
}

void Tutorial::PollEvent(GameData& _gameData)
{
	descriptions[(size_t)(curPage - 1)]->PollEvent(_gameData.gameEvent);

	if (curPage > 1)
	{
		arrows[LEFT]->CallOnOut();

	}

	if (curPage < nbPages)
	{
		arrows[RIGHT]->CallOnOut();
	}

	if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "LEFT"))
	{
		if (curPage > 1)
		{
			--currentButton;

			if (currentButton < 0)
			{
				if (curPage < nbPages)
				{
					currentButton = (int)(arrows.size() - 1);
				}
				else
				{
					currentButton = 0;
				}
			}

			Audio::Get().PlaySoundNoSecurity("BUTTON_SWAP");
		}
	}
	if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "RIGHT"))
	{
		if (curPage < nbPages)
		{
			++currentButton;

			if (currentButton > (int)arrows.size() - 1)
			{
				if (curPage > 1)
				{
					currentButton = 0;
				}
				else
				{
					currentButton = (int)arrows.size() - 1;
				}
			}

			Audio::Get().PlaySoundNoSecurity("BUTTON_SWAP");
		}
	}

	if (curPage > 1)
	{
		arrows[LEFT]->PollEvent(_gameData);

	}

	if (curPage < nbPages)
	{
		arrows[RIGHT]->PollEvent(_gameData);
	}


	if (!arrows.empty())
	{
		//arrows[currentButton]->CallOnOver();
		if (ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "INTERACT_2")
			|| ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "INTERACT"))
		{
			if ((currentButton == RIGHT && curPage < nbPages) || (currentButton == LEFT && curPage == 1))
			{
				currentButton = RIGHT;
			}
			else
			{
				currentButton = LEFT;
			}

			arrows[currentButton]->CallOnOut();
			arrows[currentButton]->CallOnClick();
		}
	}
}

void Tutorial::Update(GameData& _gameData)
{
	descriptions[(size_t)(curPage - 1)]->Update();
	descriptions[(size_t)(curPage - 1)]->setOrigin(descriptions[(size_t)(curPage - 1)]->GetSize().x / 2.f, descriptions[(size_t)(curPage - 1)]->GetSize().y / 2.f);

	if (curPage > 1)
	{
		arrows[LEFT]->CallOnOut();
	}
	if (curPage < nbPages)
	{
		arrows[RIGHT]->CallOnOut();
	}

	if (ControlsManager::Get().IsPressedDirection("LEFT"))
	{
		if (curPage > 1)
		{
			--currentButton;

			if (currentButton < 0)
			{
				if (curPage < nbPages)
				{
					currentButton = RIGHT;
				}
				else
				{
					currentButton = LEFT;
				}
			}

			Audio::Get().PlaySoundNoSecurity("BUTTON_SWAP");
		}
	}
	if (ControlsManager::Get().IsPressedDirection("RIGHT"))
	{
		if (curPage < nbPages)
		{
			++currentButton;

			if (currentButton > arrows.size() - 1)
			{
				if (curPage > 1)
				{
					currentButton = LEFT;
				}
				else
				{
					currentButton = RIGHT;
				}
			}

			Audio::Get().PlaySoundNoSecurity("BUTTON_SWAP");
		}
	}

	//for (int i = 0; i < arrows.size(); i++)
	//{
	//	if (arrows[i]->GetRect().contains(Screen::Get().GetMousePosition(_gameData.window)))
	//	{
	//		currentButton = i;
	//		if (i == LEFT)
	//		{
	//			arrows[i]->SetTextureSprite(TexturesManager::Get()["LEFT_ARROW"]);
	//		}
	//		else
	//		{
	//			arrows[i]->SetTextureSprite(TexturesManager::Get()["RIGHT_ARROW"]);
	//		}
	//	}
	//}

	arrows[currentButton]->CallOnOver();

	DrawManager::Get().SetObjectVisible(keyDraw, arrows[LEFT].get(), curPage > 1);
	DrawManager::Get().SetObjectVisible(keyDraw, arrows[RIGHT].get(), curPage < nbPages);
}

void Tutorial::ClearTiesPages()
{
	for (size_t i = (size_t)(nbPages - 1); i >= nbBasePages; --i)
	{
		DrawManager::Get().DeleteObject(keyDraw, descriptions[i]);
		delete descriptions[i];
		descriptions.pop_back();
		tutoTextureKeys.pop_back();
	}
	nbPages = nbBasePages;
}

void Tutorial::AddNewTuto(const std::vector<std::string>& _tutoTextureKeys, const std::vector<std::string>& _descriptionPaths)
{
	for (auto& elem : _tutoTextureKeys)
	{
		tutoTextureKeys.push_back(elem);
	}

	size_t initialSize = descriptions.size();
	descriptions.resize(initialSize + _descriptionPaths.size());

	for (size_t i = initialSize; i < descriptions.size(); i++)
	{
		descriptions[i] = new Dialog(_descriptionPaths[i - initialSize]);
		descriptions[i]->setOrigin(descriptions[i]->GetSize().x / 2.f, descriptions[i]->GetSize().y / 2.f);
		descriptions[i]->setPosition(getPosition().x, getPosition().y + screenShot.getGlobalBounds().height / 2.f + TEXT_OFFSET);
		descriptions[i]->SetDialogDisappearing(false);
		DrawManager::Get().AddObject(keyDraw, *descriptions[i], false);
	}

	nbPages = (unsigned int)tutoTextureKeys.size();

	BoolManager::Get()["FIGHT_TUTO"] = false;
	if (_tutoTextureKeys[0] != "TUTO_FIGHT_BOXER")
	{
		SetCurPage((unsigned int)initialSize + 1);
	}
}

void Tutorial::SetCurPage(const unsigned int& _page)
{
	if (_page <= nbPages)
	{
		DrawManager::Get().SetObjectVisible(keyDraw, descriptions[(size_t)(curPage - 1)], false);
		curPage = _page;
		screenShot.setTexture(TexturesManager::Get()[tutoTextureKeys[(size_t)(curPage - 1)]]);
		SetCenteredString(nbPagesText, std::to_string(curPage));
		DrawManager::Get().SetObjectVisible(keyDraw, descriptions[(size_t)(curPage - 1)], true);
		descriptions[(size_t)(curPage - 1)]->StartDialog();

		if ((currentButton == RIGHT && curPage < nbPages) || (currentButton == LEFT && curPage == 1))
		{
			currentButton = RIGHT;
		}
		else
		{
			currentButton = LEFT;
		}
	}
}

void Tutorial::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
{
	_states.transform *= getTransform();

	_target.draw(screenShot, _states);
}

void Tutorial::InitButtons(void)
{

	arrows[LEFT] = std::make_unique<Button>(
		sf::Vector2f(getPosition().x - screenShot.getGlobalBounds().width / 20.f,
			getPosition().y + screenShot.getGlobalBounds().height / 2.f + ARROW_OFFSET), TexturesManager::Get()["LEFT_ARROW"]);


	arrows[RIGHT] = std::make_unique<Button>(
		sf::Vector2f(getPosition().x + screenShot.getGlobalBounds().width / 20.f,
			getPosition().y + screenShot.getGlobalBounds().height / 2.f + ARROW_OFFSET), TexturesManager::Get()["RIGHT_ARROW"]);

	for (auto& arrow : arrows)
	{
		arrow->setOrigin(arrow->GetSprite().getGlobalBounds().width / 2.f, arrow->GetSprite().getGlobalBounds().height / 2.f);
		arrow->setScale(2.f, 2.f);
	}

	arrows[LEFT]->setPosition(sf::Vector2f(getPosition().x - screenShot.getGlobalBounds().width / 10.f,
		getPosition().y + screenShot.getGlobalBounds().height / 2.f + ARROW_OFFSET));

	arrows[RIGHT]->setPosition(sf::Vector2f(getPosition().x + screenShot.getGlobalBounds().width / 10.f,
		getPosition().y + screenShot.getGlobalBounds().height / 2.f + ARROW_OFFSET));

	// Edit function(s) btn //
	arrows[LEFT]->SetOnClickFct([this]
		{
			DrawManager::Get().SetObjectVisible(keyDraw, descriptions[(size_t)(curPage - 1)], false);
			curPage--;
			screenShot.setTexture(TexturesManager::Get()[tutoTextureKeys[(size_t)(curPage - 1)]]);
			SetCenteredString(nbPagesText, std::to_string(curPage));
			DrawManager::Get().SetObjectVisible(keyDraw, descriptions[(size_t)(curPage - 1)], true);
			descriptions[(size_t)(curPage - 1)]->StartDialog();

			if ((currentButton == RIGHT && curPage < nbPages) || (currentButton == LEFT && curPage == 1))
			{
				currentButton = RIGHT;
			}
			else
			{
				currentButton = LEFT;
			}

			Audio::Get().PlaySoundNoSecurity("BUTTON_SELECT");
		});

	arrows[RIGHT]->SetOnClickFct([this]
		{
			DrawManager::Get().SetObjectVisible(keyDraw, descriptions[(size_t)(curPage - 1)], false);
			curPage++;
			screenShot.setTexture(TexturesManager::Get()[tutoTextureKeys[(size_t)(curPage - 1)]]);
			SetCenteredString(nbPagesText, std::to_string(curPage));
			DrawManager::Get().SetObjectVisible(keyDraw, descriptions[(size_t)(curPage - 1)], true);
			descriptions[(size_t)(curPage - 1)]->StartDialog();

			if ((currentButton == RIGHT && curPage < nbPages) || (currentButton == LEFT && curPage == 1))
			{
				currentButton = RIGHT;
			}
			else
			{
				currentButton = LEFT;
			}

			Audio::Get().PlaySoundNoSecurity("BUTTON_SELECT");
		});

	arrows[LEFT]->SetOnOverFct([&]
		{
			arrows[LEFT]->SetTextureSprite(TexturesManager::Get()["LEFT_ARROW"]);

			currentButton = LEFT;
		});


	arrows[RIGHT]->SetOnOverFct([&]
		{

			arrows[RIGHT]->SetTextureSprite(TexturesManager::Get()["RIGHT_ARROW"]);
			currentButton = RIGHT;
		});

	for (int i = 0; i < arrows.size(); i++)
	{
		arrows[i]->setOrigin(arrows[i]->GetSprite().getGlobalBounds().width / 2.f, arrows[i]->GetSprite().getGlobalBounds().height / 2.f);

		arrows[i]->SetOnOutFct([this, i]
			{
				if (i == LEFT)
				{
					arrows[i]->SetTextureSprite(TexturesManager::Get()["LEFT_ARROW_OVER"]);
				}
				else
				{
					arrows[i]->SetTextureSprite(TexturesManager::Get()["RIGHT_ARROW_OVER"]);
				}
			});
	}
}
