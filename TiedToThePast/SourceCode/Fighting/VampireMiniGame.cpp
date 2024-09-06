#include "../Definitions.hpp"
#include "VampireMiniGame.hpp"
#include "../Enemy.hpp"

VampireMiniGame::VampireMiniGame(void)
{
	notesArr[SINGLE_NOTE] = new AnimatedSprite("Contents/Data/Animations/Other/Notes/SingleNote.json");
	notesArr[DOUBLE_NOTE] = new AnimatedSprite("Contents/Data/Animations/Other/Notes/DoubleNote.json");
	notesArr[TRIPLE_NOTE] = new AnimatedSprite("Contents/Data/Animations/Other/Notes/TripleNote.json");
	notesArr[SOL] = new AnimatedSprite("Contents/Data/Animations/Other/Notes/Sol.json");

	bats[0] = new AnimatedSprite("Contents/Data/Animations/Other/bat.json");

	for (unsigned int i = 1; i < bats.size(); i++)
	{
		bats[i] = new AnimatedSprite(*bats[0]);
	}

	InitSound();
}

VampireMiniGame::~VampireMiniGame(void)
{
	for (auto& elem : notesArr)
	{
		delete elem;
		elem = nullptr;
	}

	for (auto& bat : bats)
	{
		delete bat;
		bat = nullptr;
	}

	DeleteSound();
}

void VampireMiniGame::InitMiniGame(std::shared_ptr<Player>& _player, std::shared_ptr<Enemy>& _enemy, FightingState* _fightingState)
{
	SetActiveDef();

	playerTakeDamage = false;
	totalTime = 0.f;

	batDescendingStopPos = { enemy->getPosition() - sf::Vector2f(0.f, enemy->GetGlobalAABB().height + 100.f) };

	if (player->IsAlive())
	{
		enemy->SetAnimationState(Enemy::AnimationState::IDLE);
	}

	if (--nbTurnsSpecial == 0)
	{
		InitSpecialAtk();
	}
	else
	{
		InitNormalAtk();
	}

	if (enemy->IsStun())
	{
		End();
	}
}

void VampireMiniGame::UpdateMiniGame(FightingState* _fightingState)
{
	const float& dt = GameClock::Get().GetDeltaTime();

	if (note != nullptr)
	{
		note->Update();
	}

	for (auto& bat : bats)
	{
		if (bat != nullptr)
		{
			bat->Update();
		}
	}

	// State update
	switch (state)
	{
	case VampireMiniGame::PREPARING_NORMAL_ATK:
		UpdatePreparingNormalAttack(_fightingState, dt);
		break;
	case VampireMiniGame::PREPARING_SPECIAL_ATK:
		UpdatePreparingSpecialAttack(_fightingState, dt);
		break;
	case VampireMiniGame::NORMAL_ATK:
		UpdateNormalAttack(_fightingState, dt);
		break;
	case VampireMiniGame::SPECIAL_ATK:
		UpdateSpecialAttack(_fightingState, dt);
		break;
	case VampireMiniGame::END_NORMAL_ATK:
		UpdateEndNormalAttack(_fightingState, dt);
		break;
	case VampireMiniGame::END_SPECIAL_ATK:
		UpdateEndSpecialAttack(_fightingState, dt);
		break;
	default:
		break;
	}
}

void VampireMiniGame::PollEventMiniGame(sf::Event& _event, FightingState* _fightingState)
{
}

void VampireMiniGame::InitSpecialAtk(void)
{
	batStartingPos = { enemy->getPosition().x, -100.f };

	for (auto& bat : bats)
	{
		bat->setOrigin(bat->GetSprite().getGlobalBounds().width / 2.f, bat->GetSprite().getGlobalBounds().height / 2.f);
	}

	bats[0]->setPosition(batStartingPos);
	bats[0]->setRotation(-25.f);
	bats[0]->SetCurrentFrame(0);
	bats[1]->setPosition(batStartingPos + BAT_OFFSET_1);
	bats[1]->setRotation(-35.f);
	bats[1]->SetCurrentFrame(1);
	bats[2]->setPosition(batStartingPos + BAT_OFFSET_2);
	bats[2]->setRotation(-15.f);
	bats[2]->SetCurrentFrame(2);

	nbTurnsSpecial = 3;
	enemy->SetAnimationState(Enemy::AnimationState::SPECIAL);
	enemy->GetCurAnim()->Reset();
	enemy->GetCurAnim()->Play();

	state = PREPARING_SPECIAL_ATK;
	totalTime = 0.f;

	// Play bats' arrival sound
	audioMng.PlaySound("VAMPIRE_BAT_ARRIVAL");
}

void VampireMiniGame::InitNormalAtk(void)
{
	for (auto& elem : notesArr)
	{
		elem->setPosition(sf::Vector2f(-100, -100));
		elem->setOrigin(elem->GetSprite().getGlobalBounds().width / 2.f, elem->GetSprite().getGlobalBounds().height / 2.f);
	}

	throwingNoteTimer = THROWING_NOTE_DURATION;
	state = PREPARING_NORMAL_ATK;
}

void VampireMiniGame::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
{
	_states.transform *= getTransform();

	_target.draw(textResult, _states);

	if (state == NORMAL_ATK)
	{
		_target.draw(*note, _states);
	}
	else if (state == PREPARING_SPECIAL_ATK || state == SPECIAL_ATK)
	{
		for (auto& bat : bats)
		{
			_target.draw(*bat, _states);
		}
	}
}

void VampireMiniGame::UpdatePreparingNormalAttack(FightingState* _fightingState, const float& _dt)
{
	if (enemy->GetAnimationState() != Enemy::AnimationState::ATTACKING)
	{
		chosenNote = rand() % NB_NOTES;
		note = notesArr[chosenNote];
		note->setPosition(enemy->getPosition().x, enemy->getPosition().y - enemy->GetGlobalAABB().height / 2);
		noteInitPos = note->getPosition();

		totalTime = 0;
		enemy->SetAnimationState(Enemy::AnimationState::ATTACKING);
		enemy->GetCurAnim()->Reset();
		enemy->GetCurAnim()->Play();
	}

	if (enemy->GetAnimationState() == Enemy::AnimationState::ATTACKING &&
		enemy->GetCurAnim()->GetCurrentFrame() == enemy->GetCurAnim()->GetFrameCount() - 1)
	{
		totalTime = 0.f;
		state = NORMAL_ATK;
		audioMng.PlaySound("MUSICIAN_ATTACK");
	}
}

void VampireMiniGame::UpdatePreparingSpecialAttack(FightingState* _fightingState, const float& _dt)
{
	if (enemy->GetCurAnim()->GetCurrentFrame() == 5)
	{
		enemy->GetCurAnim()->Pause();
	}

	// Decrease timer
	batDescendingTimer += _dt;
	// Calculate the factor for animation
	float factor = Maths::Clamp(batDescendingTimer / BAT_DESCENDING_DURATION, 0.f, 1.f);
	// Set the new position for the player
	sf::Vector2f newPosition = batStartingPos + Maths::CreateVector(batStartingPos, batDescendingStopPos) * factor;
	// Apply new bats position
	bats[0]->setPosition(newPosition);
	bats[1]->setPosition(newPosition + BAT_OFFSET_1);
	bats[2]->setPosition(newPosition + BAT_OFFSET_2);

	if (totalTime >= 1.15f)
	{
		enemy->GetCurAnim()->Play();
	}

	if (enemy->GetCurAnim()->GetCurrentFrame() == enemy->GetCurAnim()->GetFrameCount() - 2)
	{
		totalTime = 0.f;
		batDescendingTimer = 0.f;
		state = SPECIAL_ATK;
		// Attack sound
		audioMng.PlaySound("MUSICIAN_ATTACK");
		// Bats sound
		audioMng.PlaySound("VAMPIRE_BAT_FLY");
	}
}

void VampireMiniGame::UpdateNormalAttack(FightingState* _fightingState, const float& _dt)
{
	throwingNoteTimer -= _dt;

	// Calculate the factor for the ease-in ease-out animation
	float factor = Maths::Clamp(totalTime / THROWING_NOTE_DURATION, 0.f, 1.f);
	float deltaPos1 = sin(2 * PI * totalTime) * 50;
	// Set the new position for the player
	sf::Vector2f newPosition = noteInitPos + Maths::CreateVector(noteInitPos, playerInitPos - sf::Vector2f(800.f, player->GetGlobalAABB().height / 2)) * factor;
	newPosition.y += deltaPos1;
	// Apply new player position
	note->setPosition(newPosition);

	// Active exclamation mark
	if (newPosition.x - player->getPosition().x > 100.f &&
		newPosition.x - player->getPosition().x < 250.f)
	{
		warning.setPosition(player->getPosition() - sf::Vector2f(0.f, 330.f));
		activeWarning = true;
	}
	else
	{
		activeWarning = false;
	}

	// Resolve collision note player
	if (std::abs(player->getPosition().x - newPosition.x) < 70.f)
	{
		if (player->GetState() != PlayerState::DODGE && !playerTakeDamage)
		{
			state = END_NORMAL_ATK;
		}
	}
	else if (throwingNoteTimer < 0.f)
	{
		End();
	}
}

void VampireMiniGame::UpdateSpecialAttack(FightingState* _fightingState, const float& _dt)
{
	if (enemy->GetCurAnim()->GetCurrentFrame() == enemy->GetCurAnim()->GetFrameCount() - 1)
	{
		enemy->GetCurAnim()->Pause();
	}

	//// bats rotation
	//rotateAngle += ROTATE_SPEED * _dt;
	//enemy->GetCurAnim()->setRotation(rotateAngle);

	// Calcul position by time between 2 positions
	const float easingFactor = Easings::InCirc(Maths::Clamp(1 - totalTime / BAT_ATTACK_DURATION, 0, 1));
	const float timerFactor = Maths::Clamp(1 - totalTime / BAT_ATTACK_DURATION, 0, 1);
	sf::Vector2f newPosition = Easings::LerpY(sf::Vector2f(-OUT_SCREEN_X, player->getPosition().y - player->GetGlobalAABB().height / 2.f), batDescendingStopPos, timerFactor, easingFactor);

	// Apply new bats position
	bats[0]->setPosition(newPosition);
	bats[1]->setPosition(newPosition + BAT_OFFSET_1);
	bats[2]->setPosition(newPosition + BAT_OFFSET_2);

	// Active exclamation mark
	if (newPosition.x - player->getPosition().x > 100.f &&
		newPosition.x - player->getPosition().x < 250.f)
	{
		warning.setPosition(player->getPosition() - sf::Vector2f(0.f, 330.f));
		activeWarning = true;
	}
	else
	{
		activeWarning = false;
	}

	// Resolve collision bats player
	if (std::abs(player->getPosition().x - newPosition.x) < 70.f)
	{
		if (player->GetState() != PlayerState::DODGE && !playerTakeDamage)
		{
			state = END_SPECIAL_ATK;
		}
	}
	else if (newPosition.x <= -OUT_SCREEN_X)
	{
		audioMng.StopSound("VAMPIRE_BAT_FLY");
		End();
	}
}

void VampireMiniGame::UpdateEndNormalAttack(FightingState* _fightingState, const float& _dt)
{
	//TODO: SON
	audioMng.ChangeBuffer("ENEMY_HIT", "VAMPIRE_NORMAL_HIT");
	audioMng.PlaySound("ENEMY_HIT", 50.f);
	player->TakeDamage(short((float)enemy->GetDamage() * multipliers[chosenNote]), enemy->GetAttackType());
	End();
}

void VampireMiniGame::UpdateEndSpecialAttack(FightingState* _fightingState, const float& _dt)
{
	//TODO: SON
	audioMng.StopSound("VAMPIRE_BAT_FLY");
	audioMng.ChangeBuffer("ENEMY_HIT", "VAMPIRE_SPECIAL_HIT");
	audioMng.PlaySound("ENEMY_HIT", 100.f);
	enemy->ApplyEffect(player);
	player->TakeDamage(enemy->GetDamage(), enemy->GetAttackType());
	End();
}

void VampireMiniGame::InitSound(void)
{
	audioMng.AddBuffer("Contents/Assets/Audio/Sounds/Enemies/VampireNormalHit.wav", "VAMPIRE_NORMAL_HIT");
	audioMng.AddBuffer("Contents/Assets/Audio/Sounds/Enemies/VampireSpecialHit.wav", "VAMPIRE_SPECIAL_HIT");
	audioMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Enemies/VampireBatFly.wav", "VAMPIRE_BAT_FLY", 90.f);
	audioMng.AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Enemies/VampireBatArrival.wav", "VAMPIRE_BAT_ARRIVAL", 50.f);
}

void VampireMiniGame::DeleteSound(void)
{
	audioMng.DeleteBuffer("VAMPIRE_NORMAL_HIT");
	audioMng.DeleteBuffer("VAMPIRE_SPECIAL_HIT");
	audioMng.DeleteSoundAndBuffer("VAMPIRE_BAT_FLY");
	audioMng.DeleteSoundAndBuffer("VAMPIRE_BAT_ARRIVAL");
}

void VampireMiniGame::End()
{
	totalTime = 0.f;

	playerTakeDamage = true;

	// Undraw exclamation mark
	activeWarning = false;

	// Dodge text
	if (!playerTakeDamage)
	{
		CallResult(ResultDodge::DODGE);
	}

	enemy->setPosition(enemyInitPos);
	enemy->SetAnimationState(Enemy::AnimationState::IDLE);
	FinishMiniGame();
}
