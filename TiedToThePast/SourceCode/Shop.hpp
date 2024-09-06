#ifndef SHOP__HPP
#define SHOP__HPP

#include "State.hpp"
#include "ShopButton.hpp"
#include "VerticalScrollMenu.hpp"
#include "DialogBox.hpp"
#include "SellMenu.hpp"

class HUD;

class Shop : public State
{
public:
	Shop();
	~Shop() override;
	void Load(const std::string& _filePath, std::shared_ptr<Player> _player, HUD& _hud);
	void AddItem(const std::string& _id, sf::Uint16 _count = 1);
	void Init(GameData& _gameData) override;
	void Update(GameData& _gameData) override;
	void PollEvent(GameData& _gameData) override;
	void Display(sf::RenderWindow& _window) override;

	// Operator overloading for saving
	friend std::ostream& operator << (std::ostream& _buffer, Shop& _shop);
	friend std::istream& operator >> (std::istream& _buffer, Shop& _shop);

private:
	constexpr static float SCROLL_DURATION = 0.5f;
	constexpr static float SCROLL_TRIGGER_DISTANCE = 20.f;
	constexpr static float SUITCASE_SCROLL_DURATION = 0.5f;
	constexpr static float SUITCASE_SHOWN_PART_HEIGHT = 50.f;
	constexpr static float FIRST_SHELF_POS_X = 105.f;
	constexpr static float FIRST_SHELF_POS_Y = 194.f;
	constexpr static float SECOND_SHELF_POS_X = 105.f;
	constexpr static float SECOND_SHELF_POS_Y = 505.f;
	constexpr static float THIRD_SHELF_POS_X = 1324.f;
	constexpr static float THIRD_SHELF_POS_Y = 357.f;
	constexpr static float SHELF_WIDTH = 484.f;
	constexpr static sf::Uint32 NB_ITEMS_BY_SHELF = 3;
	constexpr static float DISTANCE_BETWEEN_BUTTONS = (SHELF_WIDTH - NB_ITEMS_BY_SHELF * ShopButton::WIDTH) / (NB_ITEMS_BY_SHELF + 1);
	constexpr static float DIALOGUE_BOX_RIGHT_PART_LEFT = 1164.f;
	constexpr static float DIALOGUE_BOX_RIGHT_PART_CENTER_X = 1542.f;
	constexpr static float PRICES_TEXT_POS_Y = 850.f;
	constexpr static float BUY_UPGRADE_BUTTON_POS_Y = PRICES_TEXT_POS_Y + 100.f;

	enum class Animation : sf::Uint8
	{
		IDLE,
		TALK,
		BUY,
		SWIPE_LEFT,
		SWIPE_RIGHT,
		COUNT
	};

	struct Dummy
	{
		enum Type : sf::Uint8
		{
			BOXER,
			CANDLE,
			MUSICIAN,
			PRINCESS,
			COUNT
		};

		Type type = COUNT;
		Button* button = nullptr;
		Tie* tie = nullptr;
		bool isUnlocked = false;
	};

	void InitItems();
	void InitBuyMenu();
	void InitUpgradeMenu();
	bool PollSellMenuEvent(GameData& _gameData);
	void DisplayTextOnBuyMenu(const Json::Value& _json);
	void DisplayTextOnUpgradeMenu(const Json::Value& _json);
	void ShowItem(ShopButton* _button);
	void BuyItem();
	void SwitchSellMode();
	void SwitchMenu();
	void ShowDummy(Dummy::Type _dummy, bool _force = false, bool _isUpgrade = false);
	void UpgradeTie();
	void UpdateDummies();
	void MoveView(float _velocity);
	void SetCurrentAnim(Animation _anim);
	int GetDamages(int _succesLevel, bool _nextLevel = false) const;

	GameData* gameData = nullptr;
	bool isBuyMenuActive = true;
	bool isSwitchingMenu = false;
	sf::Sprite background;
	std::string filePath;
	float scrollTime = 0.f;
	ShopButton* selectedShopButton = nullptr;
	sf::Sprite dialogueBoxBackground;
	float screenScaleFactor = 0.f;

	// Buy menu
	VerticalScrollMenu scrollMenu;
	ImageButton buyButton;
	sf::Text itemTitle;
	sf::Text itemCostText;
	sf::Sprite itemCostCandySprite;
	Paragraph itemDescription;
	std::array<AnimatedSprite, (sf::Uint8)Animation::COUNT> seller;
	Animation currentAnim = Animation::IDLE;
	SellMenu sellMenu;
	float sellMenuScrollTime = 0.f;
	bool isSellMenuMoving = false;
	bool isSelling = false;

	sf::Uint32 nbItems = 0;
	std::vector<sf::Uint32> itemIndices;
	std::shared_ptr<Player> player;
	HUD* hud = nullptr;
	
	// Upgrade menu
	ImageButton upgradeButton;
	Dummy::Type selectedDummy = Dummy::COUNT;
	sf::Text tieNameText;
	sf::Text tieLevelText;
	sf::Text tiePriceText;
	sf::Sprite candyUpgradeSprite;
	Paragraph nextLevelDescription;
	sf::Sprite tiePreview;
	VerticalScrollMenu dummiesScrollMenu;
	std::array<Dummy, Dummy::COUNT> dummies;
};

#endif //!SHOP__HPP