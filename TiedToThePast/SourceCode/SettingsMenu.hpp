#ifndef SETTINGSMENU_HPP
#define SETTINGSMENU_HPP

#include "State.hpp"
#include "AnimatedBackground.hpp"
#include "VerticalScrollMenu.hpp"

static constexpr int ITEMS_LEFT_NB = 4;

class SettingsMenu : public State
{
public:
	enum class SubMenuState
	{
		NONE,
		VIDEO,
		AUDIO,
		CONTROLS
	};

	SettingsMenu();
	~SettingsMenu() override;

	const SubMenuState& GetSubMenuState(void) const { return currentSubState; };

	void Init(GameData& _gameData) override;
	void Update(GameData& _gameData) override;
	void PollEvent(GameData& _gameData) override;
	void Display(sf::RenderWindow& _window) override;

private:
	// Enum(s) & struct(s)

	// Managers
	DrawManager& drwMng = DrawManager::Get();
	TexturesManager& texMng = TexturesManager::Get();
	FontsManager& fntMng = FontsManager::Get();
	Screen& scrMng = Screen::Get();
	Audio& audMng = Audio::Get();
	ControlsManager& ctrlMng = ControlsManager::Get();

	// Privates function
	void InitTextures(GameData& _gameData);
	void InitCommonSettings(GameData& _gameData);
	void InitVideoSettings(GameData& _gameData);
	void InitAudioSettings(GameData& _gameData);
	void InitControlsSettings(GameData& _gameData);

	void UpdateCommon(GameData& _gameData);
	void UpdateVideo(GameData& _gameData);
	void UpdateAudio(GameData& _gameData);
	void UpdateControls(GameData& _gameData);

	void PollEventCommon(GameData& _gameData);
	void PollEventVideo(GameData& _gameData);
	void PollEventAudio(GameData& _gameData);
	void PollEventControls(GameData& _gameData);

	void EnterInNewSubState(const SubMenuState& _subState);

	void ReloadInput(GameData& _gameData);
	bool IsNewInput(GameData& _gameData);

	void Back(GameData& _gameData);

	// COMMON
	typedef void (SettingsMenu::*PtrSubState)(GameData&);
	PtrSubState currentUpdate;
	PtrSubState currentPollEvent;

	SubMenuState currentSubState;
	int currentId;

	AnimatedBackground background;
	std::unique_ptr<sf::Sprite> titleBackground;
	std::unique_ptr<sf::Text> title;

	// LEFT ITEMS

	std::vector<std::unique_ptr<MenuItem>> commonItems;

	// VIDEO ITEMS
	std::unique_ptr<sf::Text> styleText;
	std::unique_ptr<sf::Text> fpsText;
	std::unique_ptr<sf::Text> antialiasingText;
	std::unique_ptr<sf::Text> resolutionText;

	std::vector<std::unique_ptr<MenuItem>> videoItems;

	// AUDIO ITEMS
	std::unique_ptr<sf::Text> soundText;
	std::unique_ptr<sf::Text> musicText;

	std::vector<std::unique_ptr<MenuItem>> audioItems;

	// CONTROLS ITEMS

	std::vector<std::unique_ptr<sf::Text>> controlsItems;
	std::unique_ptr<VerticalScrollMenu> controlsButtons;

	std::string waitedInputName;

	bool videoModified = false;

};

#endif // !SETTINGSMENU_HPP
