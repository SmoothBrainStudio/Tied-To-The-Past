#include "Definitions.hpp"
#include "CandleRiddle.hpp"

CandleRiddle::CandleRiddle(const std::vector<Json::Value>& _candles, const CameraBorder& _cameraBorder)
{
	// les bougies sont déjà dans l'ordre dans le vector, pas besoin de le trier
	uniqueID = _candles[0]["name"].asString();
	if (_candles[0]["riddleType"].asString() == "SIMON")
	{
		type = CANDLE_RIDDLE_TYPE::SIMON;
		State = CANDLE_RIDDLE_STATE::OFF;
	}
	else if (_candles[0]["riddleType"].asString() == "LOGIC")
	{
		type = CANDLE_RIDDLE_TYPE::DAVID;
		State = CANDLE_RIDDLE_STATE::ON_GOING;
	}

	SetUpRiddleTimer.SetNewTimerDuration(_candles.size() * SECONDS_PER_CANDLES);
	EndSetUpTimer.SetNewTimerDuration(SECONDS_PER_CANDLES);
	resolvingTimer.SetNewTimerDuration(SECONDS_PER_CANDLES);
	for (auto& candle : _candles)
	{
		candlesPos.push_back(sf::Vector2f(candle["x"].asFloat() + candle["width"].asFloat() / 2.f, candle["y"].asFloat() + candle["height"].asFloat()));
	}
	indexSetUpRiddle = 1;

	lightShader.loadFromFile("Contents/Assets/Shaders/Light.frag", sf::Shader::Fragment);
	DrawManager::Get().SetRenderTextureShader("Exploration", &lightShader);
	DrawManager::Get().SetRenderTextureShader("MAP_BACKGROUND", &lightShader);
	texMng.Add("Contents/Assets/Sprites/Other/Light.png", "lightSource");

	auto& screen = Screen::Get();
	sf::Vector2<unsigned int> lightBackgroundScale = (sf::Vector2<unsigned int>)(screen.Scale(sf::Vector2f(_cameraBorder.maxX - _cameraBorder.offsetX, _cameraBorder.maxY - _cameraBorder.offsetY)));
	lightBackground.create(lightBackgroundScale.x, lightBackgroundScale.y);
	lightBackground.clear();
	background.setPosition(screen.Pos(_cameraBorder.offsetX, _cameraBorder.offsetY));
	background.setSize(sf::Vector2f(lightBackground.getSize()));
	background.setFillColor(sf::Color(255, 255, 255, 1));

	for (int i = 0; i < candlesPos.size(); i++)
	{
		candlesAnimation.push_back(new AnimatedSprite);
		areCandlesActivated.push_back(false);
		if (_candles[i]["alight"].asBool())
		{
			candlesAnimation[i]->LoadFromFile("Contents/Data/Animations/Props/Candle_simon.json");
			//candlesAnimation[i]->setTexture(texMng["CandleOn"], true);
			areCandlesActivated[i] = true;
		}
		else
		{
			candlesAnimation[i]->LoadFromFile("Contents/Data/Animations/Props/Candle_off.json");
			//candlesAnimation[i]->setTexture(texMng["CandleOff"], true);
		}
		candlesAnimation[i]->setPosition(candlesPos[i]);
		candlesAnimation[i]->setOrigin(candlesAnimation[i]->GetSprite().getGlobalBounds().width / 2.f, candlesAnimation[i]->GetSprite().getGlobalBounds().height);
		DrawManager::Get().AddObject("Exploration", *candlesAnimation[i]);

		lightSprites.push_back(new sf::Sprite());
		lightSprites[i]->setTexture(texMng["lightSource"], true);
		lightSprites[i]->setPosition(screen.Pos(candlesPos[i]));
		//lightSprites[i]->scale(screen.Scale(lightSprites[i]->getScale()));
		lightSprites[i]->setOrigin(lightSprites[i]->getGlobalBounds().width / 2.f, lightSprites[i]->getGlobalBounds().height);
	}

	percentPerCandle = float(100 / _candles.size());

	Audio::Get().AddSoundWithBuffer("FIREBALL_LAUNCH", "CANDLE_LIGHTNING");
	Audio::Get().AddSoundWithBuffer("RIDDLE_RESOLVED", "CANDLE_RESOLVED");
	Audio::Get().AddSoundWithBuffer("RIDDLE_FAILED", "CANDLE_FAILED", 40.f);
}

CandleRiddle::~CandleRiddle()
{
	for (auto& sprite : candlesAnimation)
	{
		DrawManager::Get().DeleteObject("Exploration", sprite);
		delete sprite;
		sprite = nullptr;
	}
	candlesAnimation.clear();

	for (auto& sprite : lightSprites)
	{
		delete sprite;
		sprite = nullptr;
	}
	lightSprites.clear();

	Audio::Get().DeleteSound("CANDLE_LIGHTNING");
	Audio::Get().DeleteSound("CANDLE_RESOLVED");
	Audio::Get().DeleteSound("CANDLE_FAILED");

	texMng.Delete("lightSource");

	DrawManager::Get().SetRenderTextureShader("Exploration", nullptr);
	DrawManager::Get().SetRenderTextureShader("MAP_BACKGROUND", nullptr);
}

void CandleRiddle::ChangeLight(const int& _indexCandle)
{
	if (areCandlesActivated[_indexCandle] == true)
	{
		candlesAnimation[_indexCandle]->LoadFromFile("Contents/Data/Animations/Props/Candle_off.json");
		areCandlesActivated[_indexCandle] = false;
	}
	else
	{
		candlesAnimation[_indexCandle]->LoadFromFile("Contents/Data/Animations/Props/Candle_simon.json");
		areCandlesActivated[_indexCandle] = true;
	}

	const sf::FloatRect candleGlobalBounds = candlesAnimation[_indexCandle]->GetSprite().getGlobalBounds();
	this->candlesAnimation[_indexCandle]->setOrigin(candleGlobalBounds.width / 2.f, candleGlobalBounds.height);
}

void CandleRiddle::Update(Player& _player, Camera& _camera)
{
	for (auto& candle : candlesAnimation)
	{
		candle->Update();
	}

	if (this->type == CANDLE_RIDDLE_TYPE::SIMON)
	{
		switch (this->State)
		{
		case CANDLE_RIDDLE_STATE::OFF:
			this->SimonOffStateUpdate(_player);
			break;
		case CANDLE_RIDDLE_STATE::SET_UP:
			SetUpRiddleTimer.Update();
			this->SimonSetUpUpdate();
			break;
		case CANDLE_RIDDLE_STATE::END_SET_UP:
			EndSetUpTimer.Update();
			this->SimonEndSetUpUpdate();
			break;
		case CANDLE_RIDDLE_STATE::ON_GOING:
			this->SimonOnGoingUpdate(_player);
			break;
		case CANDLE_RIDDLE_STATE::RESOLVE:
			resolvingTimer.Update();
			this->SimonResolveUpdate();
			break;
		default:
			break;
		}
	}
	else if (this->type == CANDLE_RIDDLE_TYPE::DAVID)
	{
		switch (this->State)
		{
		case CANDLE_RIDDLE_STATE::ON_GOING:
			this->DavidOnGoingUpdate(_player);
			break;
		case CANDLE_RIDDLE_STATE::RESOLVE:
			resolvingTimer.Update();
			this->DavidResolveUpdate();
			break;
		default:
			break;
		}
	}


	lightBackground.clear();
	lightBackground.draw(background);

	for (size_t i = 0; i < lightSprites.size(); i++)
	{
		if (candlesAnimation[i]->GetName() == "CANDLE_SIMON")
		{
			lightBackground.draw(*lightSprites[i], sf::BlendAdd);
		}
	}

	UpdatePlayerLight(_player);

	lightBackground.setView(_camera.GetView());
	lightBackground.display();
	lightShader.setUniform("textureLumiere", lightBackground.getTexture());

	// If one candle collide
	bool currentCollide = false;
	for (int i = 0; i < candlesAnimation.size(); i++)
	{
		if (Maths::GetSquaredLength(_player.getPosition(), candlesAnimation[i]->getPosition()) < LENGTH_TO_INTERACT_WITH_CANDLE_RIDDLE * LENGTH_TO_INTERACT_WITH_CANDLE_RIDDLE && _player.GetCostumeState() == CostumeState::CANDLE)
		{
			currentCollide = true;
			wasCollide = true;
			break;
		}
	}

	if (!currentCollide && wasCollide)
	{
		_player.SetEnablePhylactery(false);
		wasCollide = false;
	}
}

void CandleRiddle::UpdatePlayerLight(Player& _player)
{
	sf::Uint8 alpha = 255;
	float scaleMultiply = 1.f;
	if (_player.GetCostumeState() != CostumeState::CANDLE)
	{
		alpha = 50;
		scaleMultiply = 0.5f;
	}

	playerLightSize = 1.75f + sin(5.f * GameClock::Get().GetTotalTime() + 1.f) * sin(1.5f * GameClock::Get().GetTotalTime()) * Maths::PreciseRand(0.15f, 0.05f);
	playerLightSize = Maths::Clamp(playerLightSize, 1.6f, 1.9f);

	sf::Sprite sprite;
	sprite.setTexture(texMng["lightSource"], true);
	sprite.setOrigin(texMng["lightSource"].getSize().x / 2.f, texMng["lightSource"].getSize().y / 2.f);
	sprite.setColor(sf::Color(255, 255, 255, alpha));
	sprite.scale(Screen::Get().Scale(playerLightSize * scaleMultiply, playerLightSize * scaleMultiply));
	sprite.setPosition(Screen::Get().Pos(_player.getPosition().x, _player.getPosition().y - _player.GetGlobalAABB().height * scaleMultiply));
	lightBackground.draw(sprite, sf::BlendAdd);

	// draw a second time the sprite for a better effect
	sprite.setTexture(texMng["lightSource"], true);
	sprite.setOrigin(texMng["lightSource"].getSize().x / 2.f, texMng["lightSource"].getSize().y / 2.f);
	sprite.setColor(sf::Color(255, 255, 255, alpha));
	lightBackground.draw(sprite, sf::BlendAdd);
}

void CandleRiddle::PollEvent(Player& _player, sf::Event& _event)
{
	if (this->type == CANDLE_RIDDLE_TYPE::SIMON)
	{
		switch (this->State)
		{
		case CANDLE_RIDDLE_STATE::OFF:
			this->SimonOffStatePollEvent(_player, _event);
			break;
		case CANDLE_RIDDLE_STATE::SET_UP:
			break;
		case CANDLE_RIDDLE_STATE::END_SET_UP:
			break;
		case CANDLE_RIDDLE_STATE::ON_GOING:
			this->SimonOnGoingPollEvent(_player, _event);
			break;
		case CANDLE_RIDDLE_STATE::RESOLVE:
			break;
		default:
			break;
		}
	}
	else if (this->type == CANDLE_RIDDLE_TYPE::DAVID)
	{
		switch (this->State)
		{

		case CANDLE_RIDDLE_STATE::ON_GOING:
			this->DavidOnGoingPollEvent(_player, _event);
			break;
		default:
			break;
		}
	}
}

void CandleRiddle::SimonOffStateUpdate(Player& _player)
{
	for (int i = 0; i < candlesAnimation.size(); i++)
	{
		if (Maths::GetSquaredLength(_player.getPosition(), candlesAnimation[i]->getPosition()) < LENGTH_TO_INTERACT_WITH_CANDLE_RIDDLE * LENGTH_TO_INTERACT_WITH_CANDLE_RIDDLE && _player.GetCostumeState() == CostumeState::CANDLE)
		{
			_player.SetEnablePhylactery(true, Player::PhylacteryType::CANDLE);
			break;
		}
	}

	if (isPlayerInteract)
	{
		SetUpRiddleTimer.Run();
		this->State = CANDLE_RIDDLE_STATE::SET_UP;
	}
}

void CandleRiddle::SimonOffStatePollEvent(const Player& _player, sf::Event& _event)
{
	if ((_player.GetCostumeState() == CostumeState::CANDLE && (ControlsManager::Get().IsPressedInput(_event, "INTERACT") && !isSetUp)) || isRestarting)
	{
		for (int i = 0; i < candlesAnimation.size(); i++)
		{
			if (Maths::GetSquaredLength(_player.getPosition(), candlesAnimation[i]->getPosition()) < LENGTH_TO_INTERACT_WITH_CANDLE_RIDDLE * LENGTH_TO_INTERACT_WITH_CANDLE_RIDDLE || isRestarting)
			{
				isPlayerInteract = true;
				break;
			}
		}
	}
}

void CandleRiddle::SimonSetUpUpdate()
{
	if (!isSetUp)
	{
		if (SetUpRiddleTimer.GetPourcentageTimePassed() / indexSetUpRiddle > percentPerCandle)
		{
			candlesAnimation[size_t(indexSetUpRiddle - 1)]->LoadFromFile("Contents/Data/Animations/Props/Candle_simon.json");
			Audio::Get().PlaySound("CANDLE_LIGHTNING");

			const sf::FloatRect candleGlobalBounds = candlesAnimation[size_t(indexSetUpRiddle - 1)]->GetSprite().getGlobalBounds();
			candlesAnimation[size_t(indexSetUpRiddle - 1)]->setOrigin(candleGlobalBounds.width / 2.f, candleGlobalBounds.height);
			indexSetUpRiddle++;
		}
	}
	if (SetUpRiddleTimer.IsOver())
	{

		candlesAnimation[candlesAnimation.size() - 1]->LoadFromFile("Contents/Data/Animations/Props/Candle_simon.json");
		const sf::FloatRect candleGlobalBounds = candlesAnimation[candlesAnimation.size() - 1]->GetSprite().getGlobalBounds();
		candlesAnimation[candlesAnimation.size() - 1]->setOrigin(candleGlobalBounds.width / 2.f, candleGlobalBounds.height);
		SetUpRiddleTimer.Stop();
		isSetUp = true;
		this->State = CANDLE_RIDDLE_STATE::END_SET_UP;
		EndSetUpTimer.Run();
	}


}

void CandleRiddle::SimonEndSetUpUpdate()
{
	isRestarting = false;
	if (EndSetUpTimer.IsOver())
	{
		for (int i = 0; i < this->candlesAnimation.size(); i++)
		{
			candlesAnimation[i]->LoadFromFile("Contents/Data/Animations/Props/Candle_off.json");
			const sf::FloatRect candleGlobalBounds = candlesAnimation[i]->GetSprite().getGlobalBounds();
			candlesAnimation[i]->setOrigin(candleGlobalBounds.width / 2.f, candleGlobalBounds.height);
		}
		this->State = CANDLE_RIDDLE_STATE::ON_GOING;
	}
}

void CandleRiddle::SimonOnGoingUpdate(Player& _player)
{
	for (int i = 0; i < candlesAnimation.size(); i++)
	{
		if (Maths::GetSquaredLength(_player.getPosition(), candlesAnimation[i]->getPosition()) < LENGTH_TO_INTERACT_WITH_CANDLE_RIDDLE * LENGTH_TO_INTERACT_WITH_CANDLE_RIDDLE && _player.GetCostumeState() == CostumeState::CANDLE)
		{
			_player.SetEnablePhylactery(true, Player::PhylacteryType::CANDLE);
			break;
		}
	}

	if (activationOrder.size() == candlesAnimation.size())
	{
		resolvingTimer.Run();
		State = CANDLE_RIDDLE_STATE::RESOLVE;
	}
}

void CandleRiddle::SimonOnGoingPollEvent(Player& _player, sf::Event& _event)
{
	if (_player.GetCostumeState() == CostumeState::CANDLE && ControlsManager::Get().IsPressedInput(_event, "INTERACT"))
	{
		for (int i = 0; i < candlesPos.size(); i++)
		{
			if (Maths::GetSquaredLength(_player.getPosition(), candlesAnimation[i]->getPosition()) < LENGTH_TO_ACTIVATE_CANDLE * LENGTH_TO_ACTIVATE_CANDLE && !Maths::IsIn(activationOrder, i))
			{
				candlesAnimation[i]->LoadFromFile("Contents/Data/Animations/Props/Candle_simon.json");
				const sf::FloatRect candleGlobalBounds = candlesAnimation[i]->GetSprite().getGlobalBounds();
				candlesAnimation[i]->setOrigin(candleGlobalBounds.width / 2.f, candleGlobalBounds.height);
				activationOrder.push_back(i);
				Audio::Get().PlaySound("CANDLE_LIGHTNING");
				break;
			}
		}
	}
}

void CandleRiddle::SimonResolveUpdate()
{
	if (resolvingTimer.IsOver())
	{
		if (Maths::IsSorted(this->activationOrder))
		{
			this->State = CANDLE_RIDDLE_STATE::END;
			DrawManager::Get().SetRenderTextureShader("Exploration", nullptr);
			DrawManager::Get().SetRenderTextureShader("MAP_BACKGROUND", nullptr);

			BoolManager::Get()[uniqueID] = true; // set riddle to true to not load it again
			BoolManager::Get()["FIRST_CANDLE_SIMON"] = false;

			Audio::Get().PlaySound("CANDLE_RESOLVED");
		}
		else
		{
			Audio::Get().PlaySound("CANDLE_FAILED");
			this->SimonRestart();
		}
	}

}


void CandleRiddle::SimonRestart()
{
	isRestarting = true;
	SetUpRiddleTimer.Restart();
	EndSetUpTimer.Restart();
	resolvingTimer.Restart();
	indexSetUpRiddle = 1;
	for (int i = 0; i < this->candlesAnimation.size(); i++)
	{
		candlesAnimation[i]->LoadFromFile("Contents/Data/Animations/Props/Candle_off.json");
		const sf::FloatRect candleGlobalBounds = candlesAnimation[i]->GetSprite().getGlobalBounds();
		candlesAnimation[i]->setOrigin(candleGlobalBounds.width / 2.f, candleGlobalBounds.height);
	}
	this->State = CANDLE_RIDDLE_STATE::OFF;
	isSetUp = false;
	SetUpRiddleTimer.Restart();
	EndSetUpTimer.Restart();
	activationOrder.clear();
}

void CandleRiddle::DavidOnGoingPollEvent(Player& _player, sf::Event& _event)
{
	if (_player.GetCostumeState() == CostumeState::CANDLE && ControlsManager::Get().IsPressedInput(_event, "INTERACT"))
	{
		for (int i = 0; i < this->candlesPos.size(); i++)
		{
			int previousIndex = (int)(i - 1 == -1 ? candlesAnimation.size() - 1 : (size_t)i - 1);
			int nextIndex = (int)((size_t)i + 1 == candlesAnimation.size() ? 0 : i + 1);
			if (Maths::GetSquaredLength(_player.getPosition(), candlesAnimation[i]->getPosition()) < LENGTH_TO_ACTIVATE_CANDLE * LENGTH_TO_ACTIVATE_CANDLE && !Maths::IsIn(activationOrder, i))
			{
				ChangeLight(i);
				ChangeLight(previousIndex);
				ChangeLight(nextIndex);
				Audio::Get().PlaySound("CANDLE_LIGHTNING");
				break;
			}
		}
	}
}

void CandleRiddle::DavidOnGoingUpdate(Player& _player)
{
	for (int i = 0; i < candlesAnimation.size(); i++)
	{
		if (Maths::GetSquaredLength(_player.getPosition(), candlesAnimation[i]->getPosition()) < LENGTH_TO_INTERACT_WITH_CANDLE_RIDDLE * LENGTH_TO_INTERACT_WITH_CANDLE_RIDDLE && _player.GetCostumeState() == CostumeState::CANDLE)
		{
			_player.SetEnablePhylactery(true, Player::PhylacteryType::CANDLE);
			break;
		}
	}

	if ((std::find(areCandlesActivated.begin(), areCandlesActivated.end(), false) == areCandlesActivated.end()))
	{
		this->resolvingTimer.Run();
		this->State = CANDLE_RIDDLE_STATE::RESOLVE;
	}
}

void CandleRiddle::DavidResolveUpdate()
{
	if (resolvingTimer.IsOver())
	{
		this->State = CANDLE_RIDDLE_STATE::END;
		DrawManager::Get().SetRenderTextureShader("Exploration", nullptr);
		DrawManager::Get().SetRenderTextureShader("MAP_BACKGROUND", nullptr);

		BoolManager::Get()[uniqueID] = true; // set riddle to true to not load it again
		BoolManager::Get()["FIRST_CANDLE_DAVID"] = false;

		Audio::Get().PlaySound("CANDLE_RESOLVED");
	}

}

sf::RenderTexture& CandleRiddle::GetRenderTexture()
{
	return lightBackground;
}