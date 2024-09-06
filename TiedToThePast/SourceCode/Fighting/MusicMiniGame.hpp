#ifndef MUSIC_MINIGAME_HPP
#define MUSIC_MINIGAME_HPP

#include "FightingMiniGames.hpp"
#include "../JsonManager.hpp"

class MusicMiniGame : public MiniGame
{
public:
	MusicMiniGame(void);
	~MusicMiniGame(void) override;

	void InitMiniGame(std::shared_ptr<Player>& _player, std::shared_ptr<Enemy>& _enemy, FightingState* _fightingState) override;
	void UpdateMiniGame(FightingState* _fightingState) override;
	void PollEventMiniGame(sf::Event& _event, FightingState* _fightingState) override;

private:

	static constexpr char NB_RYTHM_KEYS = 10;
	static constexpr float THROWING_NOTE_DURATION = 2.f;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void Start(FightingState* _fightingState);
	void StartOver(FightingState* _fightingState);
	void On(FightingState* _fightingState);
	void OnOver(FightingState* _fightingState);
	void End(FightingState* _fightingState);
	void EndOver(FightingState* _fightingState);

	int GetDamage();
	void CreateLineOfRythm(const int& i);
	void CallResultBuff(const int& _status);
	void CallResultDebuff(const int& _status);

	void InitSounds(void);
	void DeleteSounds(void);
	int ChooseNote(void);
	// Play a bad note if _false = true
	void PlayNote1(const bool& _false = false);
	void PlayNote2(const bool& _false = false);

	Audio& audioMng = Audio::Get();

	typedef void (MusicMiniGame::* StateFunc)(FightingState* _fightingState);
	StateFunc state = static_cast<StateFunc>(&MusicMiniGame::Start);

	enum class StateEvent
	{
		PLAYER_INTERACTION,
		NOT_INTERACTION
	};

	enum class EffectType
	{
		BUFF,
		DEBUFF
	};

	enum Notes
	{
		EMPTY_NOTE,
		SINGLE_NOTE,
		DOUBLE_NOTE,
		TRIPLE_NOTE,
		SOL
	};

	EffectType effectType;

	sf::Vector2f startPosition;
	sf::Vector2f endPosition;

	std::array<std::array<sf::Sprite, 4>, NB_RYTHM_KEYS> rythmKeys;
	std::array<std::array<std::string, 4>, NB_RYTHM_KEYS> keyToPress;
	std::array<std::array<bool, 4>, NB_RYTHM_KEYS> validTouch;

	sf::Sprite validationRect;

	int indexFirstKeySelection;
	int successNote;
	int currentRow;
	int firstKey;
	int secondKey;
	int nbTurn;

	bool test;
	bool keyInRectangle;
	bool isFirstKeyPressed;
	bool isSecondKeyPressed;
	bool success;
	bool endGame;
	bool selectSecondTouch;
	bool failedNote;
	Timer deltaTimeBetweenInput;
	Timer throwingNote;
	Timer waitingForEnd;

	AnimatedSprite* note;

	std::array<AnimatedSprite*, 5> notesArr;

	sf::Vector2f noteInitPos;
	sf::Vector2f noteFinalPosBuff;

	StateEvent stateEvent = StateEvent::NOT_INTERACTION;

	int note1;
	int note2;
};

#endif