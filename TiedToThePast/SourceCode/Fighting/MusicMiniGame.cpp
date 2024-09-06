#include "MusicMiniGame.hpp"
#include "../Player.hpp"
#include "../Enemy.hpp"
#include "TextDamage.hpp"
#include "../FightingState.hpp"
MusicMiniGame::MusicMiniGame()
{
	TexturesManager& texM = TexturesManager::Get();

	texM.Add("Contents/Assets/Sprites/Fighting/Other/Notes/Empty_Note.png", "EMPTY_NOTE");
	texM.Add("Contents/Assets/Sprites/Fighting/Other/Notes/Single_Note.png", "SINGLE_NOTE");
	texM.Add("Contents/Assets/Sprites/Fighting/Other/Notes/Double_Note.png", "DOUBLE_NOTE");
	texM.Add("Contents/Assets/Sprites/Fighting/Other/Notes/Triple_Note.png", "TRIPLE_NOTE");
	texM.Add("Contents/Assets/Sprites/Fighting/Other/Notes/Sol/1.png", "SOL_0");
	texM.Add("Contents/Assets/Sprites/Fighting/Other/Notes/Sol/2.png", "SOL_1");
	texM.Add("Contents/Assets/Sprites/Fighting/Other/Notes/Sol/3.png", "SOL_2");
	texM.Add("Contents/Assets/Sprites/Fighting/Other/Notes/Sol/4.png", "SOL_3");
	texM.Add("Contents/Assets/Sprites/Fighting/Other/Notes/Sol/5.png", "SOL_4");
	texM.Add("Contents/Assets/Sprites/Fighting/Other/MusicRectangleOn.png", "MUSIC_RECT_ON");
	texM.Add("Contents/Assets/Sprites/Fighting/Other/MusicRectangleOff.png", "MUSIC_RECT_OFF");

	notesArr[EMPTY_NOTE] = new AnimatedSprite("Contents/Data/Animations/Other/Notes/EmptyNote.json");
	notesArr[SINGLE_NOTE] = new AnimatedSprite("Contents/Data/Animations/Other/Notes/SingleNote.json");
	notesArr[DOUBLE_NOTE] = new AnimatedSprite("Contents/Data/Animations/Other/Notes/DoubleNote.json");
	notesArr[TRIPLE_NOTE] = new AnimatedSprite("Contents/Data/Animations/Other/Notes/TripleNote.json");
	notesArr[SOL] = new AnimatedSprite("Contents/Data/Animations/Other/Notes/Sol.json");

	InitSounds();
}

MusicMiniGame::~MusicMiniGame()
{
	TexturesManager::Get().Delete("EMPTY_NOTE");
	TexturesManager::Get().Delete("SINGLE_NOTE");
	TexturesManager::Get().Delete("DOUBLE_NOTE");
	TexturesManager::Get().Delete("TRIPLE_NOTE");
	TexturesManager::Get().Delete("SOL_0");
	TexturesManager::Get().Delete("SOL_1");
	TexturesManager::Get().Delete("SOL_2");
	TexturesManager::Get().Delete("SOL_3");
	TexturesManager::Get().Delete("SOL_4");
	TexturesManager::Get().Delete("MUSIC_RECT_ON");
	TexturesManager::Get().Delete("MUSIC_RECT_OFF");

	for (auto& elem : notesArr)
	{
		delete elem;
		elem = nullptr;
	}

	DeleteSounds();
}

void MusicMiniGame::InitMiniGame(std::shared_ptr<Player>& _player, std::shared_ptr<Enemy>& _enemy, FightingState* _fightingState)
{
	TexturesManager& texM = TexturesManager::Get();
	ControlsManager& ctrlM = ControlsManager::Get();

	test = false;
	keyInRectangle = false;
	isFirstKeyPressed = false;
	isSecondKeyPressed = false;
	success = false;
	endGame = false;
	selectSecondTouch = false;
	over = false;
	failedNote = false;


	indexFirstKeySelection = 0;
	successNote = 0;
	currentRow = 0;
	firstKey = -1;
	secondKey = -1;
	nbTurn = -1;

	note1 = 0;
	note2 = 0;

	effectType = enemy == nullptr ? EffectType::BUFF : EffectType::DEBUFF;

	noteFinalPosBuff = sf::Vector2f(player->getPosition().x, player->getPosition().y - player->GetGlobalAABB().height - 150.f);

	validationRect.setTexture(texM["MUSIC_RECT_OFF"]);
	validationRect.setPosition(player->getPosition().x + player->GetGlobalAABB().width / 2 + 10, player->getPosition().y);

	deltaTimeBetweenInput.SetNewTimerDuration(0.1f);
	deltaTimeBetweenInput.Restart();
	deltaTimeBetweenInput.Stop();

	throwingNote.SetNewTimerDuration(THROWING_NOTE_DURATION);
	throwingNote.Restart();
	throwingNote.Stop();

	waitingForEnd.SetNewTimerDuration(2.f);
	waitingForEnd.Restart();
	waitingForEnd.Stop();

	nbTurn = 0;

	for (auto& elem : notesArr)
	{
		elem->setPosition(sf::Vector2f(-100, -100));
		elem->setOrigin(elem->GetSprite().getGlobalBounds().width / 2.f, elem->GetSprite().getGlobalBounds().height / 2.f);
	}

	note = notesArr[EMPTY_NOTE];
	// JSON READING
	const Json::Value& json = JsonManager::Get()["CONTROLS"];
	const int jsonSize = int(json.size()); // ICI ET PAS AUTRE PART
	selectSecondTouch = false;

	for (int i = 0; i < rythmKeys.size(); i++)
	{
		CreateLineOfRythm(i);
		selectSecondTouch = false;
		for (int j = 0; j < rythmKeys[i].size(); j++)
		{
			if (validTouch[i][j])
			{
				int k = rand() % jsonSize;
				bool jsonLoop = false;
				bool isSameKey = false;

				if (selectSecondTouch)
				{
					if (ctrlM.GetControllerType() == ControllerType::CONTROLLER)
					{
						isSameKey = (json[k]["ControllerButton"].asString() == json[indexFirstKeySelection]["ControllerButton"].asString());
					}
					else
					{
						isSameKey = (json[k]["Key"].asString() == json[indexFirstKeySelection]["Key"].asString());
					}
				}
				// Si c'est une manette on vérifie que la touche n'est pas keyboard only
				if (ctrlM.GetControllerType() == ControllerType::CONTROLLER)
				{
					while ((json[k]["KeyboardOnly"].asBool() || json[k]["Name"].asString() == "PAUSE" || json[k]["Name"].asString() == "FIGHT_INFO") || isSameKey)
					{
						k++;

						if (k == jsonSize && !jsonLoop)
						{
							k = 0;
							jsonLoop = true;
						}
						else if (k == jsonSize && jsonLoop)
						{
							throw;
						}

						if (selectSecondTouch)
						{
							isSameKey = (json[k]["ControllerButton"].asString() == json[indexFirstKeySelection]["ControllerButton"].asString());
						}
					}
				}
				else
				{
					while (json[k]["Key"].asString() == "Escape" || json[k]["Name"].asString() == "FIGHT_INFO" || isSameKey)
					{
						k++;

						if (k == jsonSize && !jsonLoop)
						{
							k = 0;
							jsonLoop = true;
						}
						else if (k == jsonSize && jsonLoop)
						{
							throw;
						}

						if (selectSecondTouch)
						{
							isSameKey = (json[k]["Key"].asString() == json[indexFirstKeySelection]["Key"].asString());
						}
					}
				}

				if (!selectSecondTouch)
				{
					indexFirstKeySelection = k;
					selectSecondTouch = true;
				}



				if (ctrlM.GetControllerType() == ControllerType::CONTROLLER)
				{
					if (sf::Joystick::getIdentification(ctrlM.GetControllerID()).name == "Wireless Controller" || sf::Joystick::getIdentification(ctrlM.GetControllerID()).name == "DualSense Wireless Controller")
					{
						rythmKeys[i][j].setTexture(texM[std::string(PLAYSTATION_TEXTURE_KEY) + json[k]["ControllerButton"].asString()]);
					}
					else
					{
						rythmKeys[i][j].setTexture(texM[std::string(XBOX_TEXTURE_KEY) + json[k]["ControllerButton"].asString()]);
					}
				}
				else
				{
					rythmKeys[i][j].setTexture(texM["KEY_" + json[k]["Key"].asString()]);
				}

				keyToPress[i][j] = json[k]["Name"].asString();
				rythmKeys[i][j].setOrigin(rythmKeys[i][j].getGlobalBounds().width / 2, rythmKeys[i][j].getGlobalBounds().height / 2);
				rythmKeys[i][j].setPosition(player->getPosition().x + player->GetGlobalAABB().width / 2 + 60 + 150 * j, -rythmKeys[i][j].getGlobalBounds().height / 2 - 300 * i);
			}
			else
			{
				keyToPress[i][j] = "null";
			}
		}
	}
	state = static_cast<StateFunc>(&MusicMiniGame::Start);
}

void MusicMiniGame::UpdateMiniGame(FightingState* _fightingState)
{
	if (deltaTimeBetweenInput.GetState() == Timer::State::RUNNING)
	{
		deltaTimeBetweenInput.Update();
		if (deltaTimeBetweenInput.IsOver())
		{
			if (!success)
			{
				successNote--;
			}
			isFirstKeyPressed = false;
			isSecondKeyPressed = false;
			success = false;
		}
	}


	throwingNote.Update();
	waitingForEnd.Update();
	note->Update();
	if (this->state) (this->*state)(_fightingState);
}

void MusicMiniGame::PollEventMiniGame(sf::Event& _event, FightingState* _fightingState)
{
	if (this->state == &MusicMiniGame::On)
	{
		// Ici on check les touches sur lesquels on va devoir appuyer
		for (int i = 0; i < keyToPress[currentRow].size(); i++)
		{
			if (firstKey == -1 && validTouch[currentRow][i])
			{
				firstKey = i;
			}
			else if (validTouch[currentRow][i])
			{
				secondKey = i;
			}
		}

		// Ici c'est la première partie de lorsqu'il y a deux touches sur le même ligne
		if (firstKey != -1 && secondKey != -1 && !isFirstKeyPressed && !isSecondKeyPressed)
		{
			if (ControlsManager::Get().IsPressedInput(_event, keyToPress[currentRow][firstKey]) && keyInRectangle)
			{
				isFirstKeyPressed = true;
				deltaTimeBetweenInput.Restart();
				deltaTimeBetweenInput.Run();
			}
			else if (ControlsManager::Get().IsPressedInput(_event, keyToPress[currentRow][secondKey]) && keyInRectangle)
			{
				isSecondKeyPressed = true;
				deltaTimeBetweenInput.Restart();
				deltaTimeBetweenInput.Run();
			}
			else if (_event.type == sf::Event::EventType::KeyPressed || _event.type == sf::Event::EventType::JoystickButtonPressed)
			{
				firstKey = -1;
				secondKey = -1;
				isFirstKeyPressed = false;
				isSecondKeyPressed = false;
				failedNote = true;
				successNote--;
			}
			else
			{
				firstKey = -1;
				secondKey = -1;
				isFirstKeyPressed = false;
				isSecondKeyPressed = false;
			}

		}
		// La seconde partie de la vérification pour deux touches
		else if (isFirstKeyPressed && secondKey != -1)
		{
			if (ControlsManager::Get().IsPressedInput(_event, keyToPress[currentRow][secondKey]))
			{

				note1 = ChooseNote();

				do
				{
					note2 = ChooseNote();
				} while (note2 == note1);

				PlayNote1();
				PlayNote2();

				successNote++;
				success = true;
				validTouch[currentRow][firstKey] = false;
				validTouch[currentRow][secondKey] = false;
				test = false;
				firstKey = -1;
				secondKey = -1;
				isFirstKeyPressed = false;
				isSecondKeyPressed = false;
				if (currentRow < keyToPress.size() - 1)
				{
					currentRow++;
				}
				else
				{
					endGame = true;
				}
			}
			else if (_event.type == sf::Event::EventType::KeyPressed || _event.type == sf::Event::EventType::JoystickButtonPressed)
			{
				firstKey = -1;
				secondKey = -1;
				isFirstKeyPressed = false;
				isSecondKeyPressed = false;
				failedNote = true;
				successNote--;
			}

		}
		else if (isSecondKeyPressed && firstKey != -1)
		{
			if (ControlsManager::Get().IsPressedInput(_event, keyToPress[currentRow][firstKey]))
			{
				// Les deux notes sont justes
				// Rand to select notes
				note1 = ChooseNote();

				do
				{
					note2 = ChooseNote();
				} while (note2 == note1);

				PlayNote1();
				PlayNote2();

				successNote++;
				success = true;
				validTouch[currentRow][firstKey] = false;
				validTouch[currentRow][secondKey] = false;
				test = false;
				firstKey = -1;
				secondKey = -1;
				isFirstKeyPressed = false;
				isSecondKeyPressed = false;
				if (currentRow < keyToPress.size() - 1)
				{
					currentRow++;
				}
				else
				{
					endGame = true;
				}
			}
			else if (_event.type == sf::Event::EventType::KeyPressed || _event.type == sf::Event::EventType::JoystickButtonPressed)
			{
				// Une des deux notes est juste
				// Rand to select notes
				note1 = ChooseNote();

				do
				{
					note2 = ChooseNote();
				} while (note2 == note1);

				PlayNote1();
				PlayNote2(true);

				firstKey = -1;
				secondKey = -1;
				isFirstKeyPressed = false;
				isSecondKeyPressed = false;
				failedNote = true;
				successNote--;
			}

		}

		// S'il n'y a qu'une touche
		else if (firstKey != -1 && secondKey == -1)
		{
			if (ControlsManager::Get().IsPressedInput(_event, keyToPress[currentRow][firstKey]) && keyInRectangle)
			{
				// La note est juste
				// Rand to select note
				note1 = ChooseNote();

				PlayNote1();

				successNote++;
				validTouch[currentRow][firstKey] = false;
				test = false;
				isFirstKeyPressed = false;
				isSecondKeyPressed = false;
				firstKey = -1;
				secondKey = -1;
				if (currentRow < keyToPress.size() - 1)
				{
					currentRow++;
				}
				else
				{
					endGame = true;
				}
			}
			else if (_event.type == sf::Event::EventType::KeyPressed || _event.type == sf::Event::EventType::JoystickButtonPressed)
			{
				// La note est fausse
				// Rand to select note
				note1 = ChooseNote();

				PlayNote1(true);
				firstKey = -1;
				secondKey = -1;
				isFirstKeyPressed = false;
				isSecondKeyPressed = false;
				failedNote = true;
				successNote--;
			}
			else
			{
				firstKey = -1;
				secondKey = -1;
				isFirstKeyPressed = false;
				isSecondKeyPressed = false;
			}

		}
	}
}

void MusicMiniGame::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	if (!endGame && this->state == &MusicMiniGame::On)
	{
		target.draw(validationRect, states);
		for (int i = 0; i < rythmKeys.size(); i++)
		{
			for (int j = 0; j < rythmKeys[i].size(); j++)
			{
				if (validTouch[i][j])
				{
					target.draw(rythmKeys[i][j], states);
				}
			}

		}
	}
	else if (endGame && this->state == &MusicMiniGame::On)
	{
		target.draw(*note, states);
	}

	target.draw(textResult, states);
}

void MusicMiniGame::Start(FightingState* _fightingState)
{
	if (totalTime >= START_TIME_CANDLE) StartOver(_fightingState);
}

void MusicMiniGame::StartOver(FightingState* _fightingState)
{
	totalTime = 0;
	state = static_cast<StateFunc>(&MusicMiniGame::On);

	stateEvent = StateEvent::PLAYER_INTERACTION;
	player->SetState(PlayerState::CHARGING_ATTACK);
}

void MusicMiniGame::On(FightingState* _fightingState)
{
	float dt = GameClock::Get().GetDeltaTime();
	TexturesManager& texM = TexturesManager::Get();
	if (endGame)
	{
		if (throwingNote.IsOver())
		{
			state = static_cast<StateFunc>(&MusicMiniGame::OnOver);
			return;
		}

		if (throwingNote.GetState() != Timer::State::RUNNING)
		{
			player->SetState(PlayerState::ATTACK);

			//TODO: Peut changer de son selon la réussite
			audioMng.PlaySound("MUSICIAN_ATTACK");
			if (successNote == 10)
			{
				note = notesArr[SOL];
				nbTurn = 5;
			}
			else if (successNote > 7)
			{
				note = notesArr[TRIPLE_NOTE];
				nbTurn = 4;
			}
			else if (successNote > 4)
			{
				note = notesArr[DOUBLE_NOTE];
				nbTurn = 3;
			}
			else if (successNote > 2)
			{
				note = notesArr[SINGLE_NOTE];
				nbTurn = 2;
			}
			else
			{
				note = notesArr[EMPTY_NOTE];
				nbTurn = 0;
			}


			note->setPosition(player->getPosition().x, player->getPosition().y - player->GetGlobalAABB().height / 2);
			noteInitPos = note->getPosition();
			throwingNote.Restart();
			throwingNote.Run();
			totalTime = 0;
		}
		else
		{
			// Car on peut se viser tout seul, et dans ce cas l'ennemi est nullptr
			if (enemy != nullptr)
			{
				// Calculate the factor for the ease-in ease-out animation
				float factor = Maths::Clamp(totalTime / THROWING_NOTE_DURATION, 0.f, 1.f);
				float deltaPos1 = sin(2 * PI * totalTime) * 50;
				// Set the new position for the player
				sf::Vector2f newPosition = noteInitPos + Maths::CreateVector(noteInitPos, enemyInitPos - sf::Vector2f(0, enemy->GetGlobalAABB().height / 2)) * factor;
				newPosition.y += deltaPos1;
				// Apply new player position
				note->setPosition(newPosition);
			}
			else
			{
				// Calculate the factor for the ease-in ease-out animation
				float factor = Easings::Square(Maths::Clamp(totalTime / THROWING_NOTE_DURATION, 0.f, 1.f));
				// Set the new position for the player
				sf::Vector2f newPosition = noteInitPos + Maths::CreateVector(noteInitPos, noteFinalPosBuff) * factor;
				// Apply new player position
				note->setPosition(newPosition);
			}
		}
	}
	else
	{
		for (int i = 0; i < rythmKeys.size(); i++)
		{
			for (int j = 0; j < rythmKeys[i].size(); j++)
			{
				if (validTouch[i][j])
				{
					rythmKeys[i][j].setPosition(rythmKeys[i][j].getPosition().x, rythmKeys[i][j].getPosition().y + 300 * dt);
				}
			}
		}
		for (int i = 0; i < rythmKeys[currentRow].size(); i++)
		{
			if (validTouch[currentRow][i])
			{
				keyInRectangle = rythmKeys[currentRow][i].getGlobalBounds().intersects(validationRect.getGlobalBounds());
				if (keyInRectangle)
				{
					validationRect.setTexture(texM["MUSIC_RECT_ON"]);
					test = true;
				}
				else if (test)
				{
					// Aucune touche n'est appuyée
					// TODO: Mettre le son si ça représente un intérêt
					if (!failedNote)
					{
						audioMng.ChangeBuffer("NOTE_1", "PIANO_FAILED");
						audioMng.PlaySound("NOTE_1", 50.f);
					}


					validationRect.setTexture(texM["MUSIC_RECT_OFF"]);
					if (currentRow < keyToPress.size() - 1)
					{
						currentRow++;
					}
					else
					{
						endGame = true;
					}
					firstKey = -1;
					secondKey = -1;
					test = false;
					failedNote = false;
				}
				else
				{
					validationRect.setTexture(texM["MUSIC_RECT_OFF"]);
				}
			}

		}
	}


}

void MusicMiniGame::OnOver(FightingState* _fightingState)
{
	state = static_cast<StateFunc>(&MusicMiniGame::End);
}

void MusicMiniGame::End(FightingState* _fightingState)
{
	player->SetState(PlayerState::IDLE);
	if (effectType == EffectType::BUFF)
	{
		CallResultBuff((int)_fightingState->GetSelectedPartition());
		audioMng.PlaySound("USE_POWER_POTION");
	}
	else
	{
		CallResultDebuff((int)_fightingState->GetSelectedPartition());
		audioMng.PlaySound("MUSICIAN_DEBUFF");
	}
	waitingForEnd.Restart();
	waitingForEnd.Run();
	state = static_cast<StateFunc>(&MusicMiniGame::EndOver);
}

void MusicMiniGame::EndOver(FightingState* _fightingState)
{
	if (waitingForEnd.IsOver())
	{
		FinishMiniGame();
	}
}

int MusicMiniGame::GetDamage()
{
	return 0;
}

void MusicMiniGame::CreateLineOfRythm(const int& i)
{
	bool noTouch = true;
	bool twoTouch = false;
	for (auto& it : validTouch[i])
	{
		if (!twoTouch)
		{
			bool validIt = rand() % 4 == 0;
			it = validIt;
			if (validIt && noTouch)
			{
				noTouch = false;
			}
			else if (validIt && !noTouch)
			{
				twoTouch = true;
			}
		}
		else
		{
			it = false;
		}

	}
	if (noTouch)
	{
		validTouch[i][rand() % 4] = true;
	}
}

void MusicMiniGame::CallResultBuff(const int& _status)
{

	Status* tempStatus = player->GetApplicableEffects()[_status].status->Clone();
	std::shared_ptr<Entity> playerEntity = player;

	bool applicable = true;

	for (const auto& it : playerEntity->GetStatutes())
	{
		if (it->effectName == player->GetApplicableEffects()[_status].status->effectName)
		{
			it->AddTurns((int)(nbTurn / 2.f));
			if (player->GetCurTie()->GetLevel() > 1 && nbTurn == 5)
			{
				player->ApplyParticularEffect(player, Entity::Effects::IMMUNITY, player->GetCurTie()->GetLevel() - 1);
			}
			applicable = false;
		}
	}

	if (successNote > 2 && applicable && !player->DoesItImmuned(player->GetApplicableEffects()[_status].status->id))
	{
		tempStatus->Init(nullptr, playerEntity, nbTurn);
		player->AddStatus(tempStatus);
		if (player->GetCurTie()->GetLevel() > 1 && nbTurn == 5)
		{
			player->ApplyParticularEffect(playerEntity, Entity::Effects::IMMUNITY, player->GetCurTie()->GetLevel() - 1);
		}
	}

}

void MusicMiniGame::CallResultDebuff(const int& _status)
{
	Status* tempStatus = player->GetApplicableEffects()[_status].status->Clone();
	std::shared_ptr<Entity> enemyEntity = enemy;
	std::shared_ptr<Entity> playerEntity = player;

	bool applicable = true;

	for (const auto& it : enemyEntity->GetStatutes())
	{
		if (it->effectName == player->GetApplicableEffects()[_status].status->effectName)
		{
			it->AddTurns((int)(nbTurn / 2.f));
			if (player->GetCurTie()->GetLevel() > 1 && nbTurn == 5)
			{
				player->ApplyParticularEffect(playerEntity, Entity::Effects::IMMUNITY, player->GetCurTie()->GetLevel() - 1);
			}
			applicable = false;
		}
	}

	if (successNote > 2 && applicable && !enemy->DoesItImmuned(player->GetApplicableEffects()[_status].status->id))
	{
		tempStatus->Init(nullptr, enemyEntity, nbTurn);
		enemy->AddStatus(tempStatus);
		if (player->GetCurTie()->GetLevel() > 1 && nbTurn == 5)
		{
			player->ApplyParticularEffect(playerEntity, Entity::Effects::IMMUNITY, player->GetCurTie()->GetLevel() - 1);
		}
	}
}

void MusicMiniGame::InitSounds(void)
{
	audioMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Player/MusicianDebuff.wav", "MUSICIAN_DEBUFF", 80.f);
	audioMng.AddBuffer("Contents/Assets/Audio/Sounds/Player/Piano1.wav", "PIANO_1");
	audioMng.AddBuffer("Contents/Assets/Audio/Sounds/Player/Piano2.wav", "PIANO_2");
	audioMng.AddBuffer("Contents/Assets/Audio/Sounds/Player/Piano3.wav", "PIANO_3");
	audioMng.AddBuffer("Contents/Assets/Audio/Sounds/Player/Piano4.wav", "PIANO_4");
	audioMng.AddBuffer("Contents/Assets/Audio/Sounds/Player/Piano5.wav", "PIANO_5");
	audioMng.AddBuffer("Contents/Assets/Audio/Sounds/Player/PianoFailed.wav", "PIANO_FAILED");
	audioMng.AddSoundWithBuffer("PIANO_1", "NOTE_1");
	audioMng.AddSoundWithBuffer("PIANO_2", "NOTE_2", 65.f);
}

void MusicMiniGame::DeleteSounds(void)
{
	audioMng.DeleteSoundAndBuffer("MUSICIAN_DEBUFF");
	audioMng.DeleteBuffer("PIANO_1");
	audioMng.DeleteBuffer("PIANO_2");
	audioMng.DeleteBuffer("PIANO_3");
	audioMng.DeleteBuffer("PIANO_4");
	audioMng.DeleteBuffer("PIANO_5");
	audioMng.DeleteBuffer("PIANO_FAILED");
	audioMng.DeleteSound("NOTE_1");
	audioMng.DeleteSound("NOTE_2");
}

int MusicMiniGame::ChooseNote(void)
{
	int note = rand() % 5 + 1;
	return note;
}

void MusicMiniGame::PlayNote1(const bool& _false)
{
	if (_false)
	{
		float randPitch = (rand() % 11) / 100.f + 1.15f;
		audioMng.SetSoundPitch("NOTE_1", randPitch);
	}
	else
	{
		audioMng.SetSoundPitch("NOTE_1", 1.f);
	}
	audioMng.ChangeBuffer("NOTE_1", "PIANO_" + std::to_string(note1));
	audioMng.PlaySound("NOTE_1", 65.f);
}

void MusicMiniGame::PlayNote2(const bool& _false)
{
	if (_false)
	{
		float randPitch = (rand() % 11) / 100.f + 1.15f;
		audioMng.SetSoundPitch("NOTE_2", randPitch);
	}
	else
	{
		audioMng.SetSoundPitch("NOTE_2", 1.f);
	}
	audioMng.ChangeBuffer("NOTE_2", "PIANO_" + std::to_string(note2));
	audioMng.PlaySound("NOTE_2");
}