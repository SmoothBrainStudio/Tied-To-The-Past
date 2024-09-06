#include "Shop.hpp"
#include "HUD.hpp"

Shop::Shop()
{
	BoolManager::Get()["SHOP_TUTORIAL_0_DONE"];
	BoolManager::Get()["SHOP_TUTORIAL_1_DONE"];

	TexturesManager& textureMng = TexturesManager::Get();

	textureMng.Add("Contents/Assets/Sprites/Dummies/Boxer.png", "DUMMY_BOXER");
	textureMng.Add("Contents/Assets/Sprites/Dummies/Candle.png", "DUMMY_CANDLE");
	textureMng.Add("Contents/Assets/Sprites/Dummies/Princess.png", "DUMMY_PRINCESS");
	textureMng.Add("Contents/Assets/Sprites/Dummies/Musician.png", "DUMMY_MUSICIAN");
	textureMng.Add("Contents/Assets/Sprites/Dummies/Mystery.png", "DUMMY_MYSTERY");
	textureMng.Add("Contents/Assets/Sprites/UI/Backgrounds/DialogueBox.png", "SHOP_DIALOGUE_BOX");
	textureMng.Add("Contents/Assets/Sprites/UI/MenuItems/BuyButton.png", "BUY_BUTTON");
	textureMng.Add("Contents/Assets/Sprites/UI/MenuItems/BuyButtonHovered.png", "BUY_BUTTON_HOVER");
	textureMng.Add("Contents/Assets/Sprites/UI/MenuItems/SellButton.png", "SELL_BUTTON");
	textureMng.Add("Contents/Assets/Sprites/UI/MenuItems/SellButtonHovered.png", "SELL_BUTTON_HOVER");
	textureMng.Add("Contents/Assets/Sprites/UI/MenuItems/UpgradeButton.png", "UPGRADE_BUTTON");
	textureMng.Add("Contents/Assets/Sprites/UI/MenuItems/UpgradeButtonHovered.png", "UPGRADE_BUTTON_HOVER");
	textureMng.Add("Contents/Assets/Sprites/UI/Inventory/Background.png", "INVENTORY_BACKGROUND");
	textureMng.Add("Contents/Assets/Sprites/UI/Shop/arrow_right_over.png", "SHOP_ARROW");
	textureMng.Add("Contents/Assets/Sprites/UI/Shop/arrow_right.png", "SHOP_ARROW_HOVERED");

	Audio::Get().AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Items/ItemPurchase.wav", "ITEM_PURCHASE");
	Audio::Get().AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Items/ItemSell.wav", "ITEM_SELL");
	Audio::Get().AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Items/CannotPurchase.wav", "CANNOT_PURCHASE", 80.f);
	Audio::Get().AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Items/ImproveTie.wav", "IMPROVE_TIE");
	Audio::Get().AddSoundAndBuffer("Contents/Assets/Audio/Sounds/Items/ShopSelect.wav", "SHOP_SELECT", 30.f);

	this->background.setTexture(TexturesManager::Get()["SHOP_BG"]);

	this->dialogueBoxBackground.setTexture(TexturesManager::Get()["SHOP_DIALOGUE_BOX"]);
	this->dialogueBoxBackground.setPosition(0.f, WORKING_HEIGHT - this->dialogueBoxBackground.getGlobalBounds().height);

	this->seller[(sf::Uint8)Animation::IDLE].LoadFromFile("Contents/Data/Animations/Entities/NPCs/Alejandro/Idle.json");
	this->seller[(sf::Uint8)Animation::BUY].LoadFromFile("Contents/Data/Animations/Entities/NPCs/Alejandro/Buy.json");
	this->seller[(sf::Uint8)Animation::TALK].LoadFromFile("Contents/Data/Animations/Entities/NPCs/Alejandro/Talk.json");
	this->seller[(sf::Uint8)Animation::SWIPE_LEFT].LoadFromFile("Contents/Data/Animations/Entities/NPCs/Alejandro/SwipeLeft.json");
	this->seller[(sf::Uint8)Animation::SWIPE_RIGHT].LoadFromFile("Contents/Data/Animations/Entities/NPCs/Alejandro/SwipeRight.json");
	for (auto& animation : this->seller)
	{
		sf::FloatRect rect = animation.GetSprite().getGlobalBounds();
		animation.setOrigin(rect.width * 0.5f, rect.height);
	}
	this->seller[(sf::Uint8)Animation::IDLE].setPosition(WORKING_WIDTH * 0.5f, this->dialogueBoxBackground.getPosition().y + 25.f);
	this->seller[(sf::Uint8)Animation::BUY].setPosition(WORKING_WIDTH * 0.5f, this->dialogueBoxBackground.getPosition().y + 25.f);
	this->seller[(sf::Uint8)Animation::TALK].setPosition(WORKING_WIDTH * 0.5f, this->dialogueBoxBackground.getPosition().y + 35.f);
	this->seller[(sf::Uint8)Animation::SWIPE_LEFT].setPosition(WORKING_WIDTH * 0.5f, this->dialogueBoxBackground.getPosition().y + 25.f);
	this->seller[(sf::Uint8)Animation::SWIPE_RIGHT].setPosition(WORKING_WIDTH * 0.5f, this->dialogueBoxBackground.getPosition().y + 25.f);

	DrawManager::Get().AddSection("SHOP_BACKGROUND");
	DrawManager::Get().AddSection("SHOP");
	DrawManager::Get().AddSection("SHOP_FOREGROUND");
	DrawManager::Get().AddSection("SHOP_SELLER");

	DrawManager::Get().AddObject("SHOP_BACKGROUND", this->background);
	//DrawManager::Get().AddObject("SHOP", this->backButton);
	DrawManager::Get().AddObject("SHOP", this->dialogueBoxBackground);
}

Shop::~Shop()
{
	TexturesManager& textureMng = TexturesManager::Get();

	DrawManager::Get().DeleteSection("SHOP_BACKGROUND");
	DrawManager::Get().DeleteSection("SHOP");
	DrawManager::Get().DeleteSection("SHOP_FOREGROUND");
	DrawManager::Get().DeleteSection("SHOP_SELLER");
	DrawManager::Get().DeleteSection("SHOP_SELECT");
	DrawManager::Get().DeleteSection("OPEN_SELLING");
	DrawManager::Get().DeleteSection("CLOSE_SELLING");

	textureMng.Delete("DUMMY_BOXER");
	textureMng.Delete("DUMMY_CANDLE");
	textureMng.Delete("DUMMY_PRINCESS");
	textureMng.Delete("DUMMY_MUSICIAN");
	textureMng.Delete("DUMMY_MYSTERY");
	textureMng.Delete("SHOP_DIALOGUE_BOX");
	textureMng.Delete("BUY_BUTTON");
	textureMng.Delete("BUY_BUTTON_HOVER");
	textureMng.Delete("SELL_BUTTON");
	textureMng.Delete("SELL_BUTTON_HOVER");
	textureMng.Delete("UPGRADE_BUTTON");
	textureMng.Delete("UPGRADE_BUTTON_HOVER");
	textureMng.Delete("INVENTORY_BACKGROUND");
	textureMng.Delete("SHOP_ARROW");
	textureMng.Delete("SHOP_ARROW_HOVERED");

	Audio::Get().DeleteSoundAndBuffer("ITEM_PURCHASE");
	Audio::Get().DeleteSoundAndBuffer("ITEM_SELL");
	Audio::Get().DeleteSoundAndBuffer("CANNOT_PURCHASE");
	Audio::Get().DeleteSoundAndBuffer("IMPROVE_TIE");
}

void Shop::Load(const std::string& _filePath, std::shared_ptr<Player> _player, HUD& _hud)
{
	this->filePath = _filePath;
	this->player = _player;
	this->hud = &_hud;
	this->InitItems();
}

void Shop::AddItem(const std::string& _id, sf::Uint16 _count)
{
	sf::Uint32 index = 0;
	do
	{
		index = rand() % 9;
	} while (std::find(this->itemIndices.begin(), this->itemIndices.end(), index) != this->itemIndices.end());
	this->itemIndices.push_back(index);

	sf::Vector2f pos;
	if (index < 3)
	{
		pos.x = FIRST_SHELF_POS_X;
		pos.y = FIRST_SHELF_POS_Y;
	}
	else if (index < 6)
	{
		pos.x = SECOND_SHELF_POS_X;
		pos.y = SECOND_SHELF_POS_Y;
	}
	else
	{
		pos.x = THIRD_SHELF_POS_X;
		pos.y = THIRD_SHELF_POS_Y;
	}
	std::shared_ptr<Item> item;
	if (Item::prefabs[_id]->GetType() == Item::CONSUMABLE)
	{
		item = std::make_shared<Consumable>(_id, _count);
		item->SetTexture(((Consumable*)item.get())->GetBigTexture());
		pos.y -= item->GetRect().height * WORKING_HEIGHT / Screen::Get().GetHeight();
	}
	else
	{
		item = std::make_shared<Item>(_id);
		pos.y -= item->GetRect().height;
	}
	pos.x += DISTANCE_BETWEEN_BUTTONS + (index % 3) * (DISTANCE_BETWEEN_BUTTONS + ShopButton::WIDTH);

	ShopButton* button = new ShopButton(*this->player, pos, item);
	button->SetShopIndex(index);
	button->SetOnClickFct(std::bind(&Shop::ShowItem, this, button));
	this->scrollMenu.AddItem(std::move(button));
	++this->nbItems;
}

void Shop::Init(GameData& _gameData)
{
	this->gameData = &_gameData;
	this->InitBuyMenu();
	this->InitUpgradeMenu();
}

void Shop::Update(GameData& _gameData)
{
	if (this->screenScaleFactor != Screen::Get().GetScaleFactor())
	{
		this->screenScaleFactor = Screen::Get().GetScaleFactor();
		this->scrollMenu.Clear();
		this->nbItems = 0;
		this->InitItems();
	}

	this->hud->Update(*this->player);

	this->seller[(sf::Uint8)this->currentAnim].Update();
	if ((this->currentAnim == Animation::TALK && ((this->isBuyMenuActive && this->itemDescription.IsAnimationEnd()) || (!this->isBuyMenuActive && this->nextLevelDescription.IsAnimationEnd())))
		|| (this->currentAnim == Animation::BUY && this->seller[(sf::Uint8)Animation::BUY].GetStatus() == AnimatedSprite::Status::STOPPED)
		|| (this->currentAnim == Animation::SWIPE_LEFT && this->seller[(sf::Uint8)Animation::SWIPE_LEFT].GetStatus() == AnimatedSprite::Status::STOPPED)
		|| (this->currentAnim == Animation::SWIPE_RIGHT && this->seller[(sf::Uint8)Animation::SWIPE_RIGHT].GetStatus() == AnimatedSprite::Status::STOPPED))
	{
		this->SetCurrentAnim(Animation::IDLE);
	}

	if (this->isSwitchingMenu)
	{
		float lastPos = Easings::OutQuint(this->scrollTime / SCROLL_DURATION) * WORKING_WIDTH;
		this->scrollTime += GameClock::Get().GetDeltaTime();
		if (this->scrollTime >= SCROLL_DURATION)
		{
			this->isSwitchingMenu = false;
			this->scrollTime = SCROLL_DURATION;
		}

		if (this->isBuyMenuActive)
		{
			this->MoveView(Easings::OutQuint(this->scrollTime / SCROLL_DURATION) * WORKING_WIDTH - lastPos);
		}
		else
		{
			this->MoveView(-(Easings::OutQuint(this->scrollTime / SCROLL_DURATION) * WORKING_WIDTH - lastPos));
		}
	}

	if (this->isSellMenuMoving)
	{
		this->sellMenuScrollTime += GameClock::Get().GetDeltaTime();
		if (this->sellMenuScrollTime >= SUITCASE_SCROLL_DURATION)
		{
			this->isSellMenuMoving = false;
			this->sellMenuScrollTime = SUITCASE_SCROLL_DURATION;
		}

		float offset = (this->sellMenu.GetGlobalBounds().height - SUITCASE_SHOWN_PART_HEIGHT) * Easings::OutQuint(this->sellMenuScrollTime / SUITCASE_SCROLL_DURATION);
		if (this->isSelling)
		{
			this->sellMenu.setPosition(this->sellMenu.getPosition().x, WORKING_HEIGHT - SUITCASE_SHOWN_PART_HEIGHT - offset);
		}
		else
		{
			this->sellMenu.setPosition(this->sellMenu.getPosition().x, WORKING_HEIGHT - this->sellMenu.GetGlobalBounds().height + offset);
		}
	}

	if (this->isBuyMenuActive)
	{
		if (this->isSelling || this->isSellMenuMoving)
		{
			this->sellMenu.Update(_gameData);
		}
		else
		{
			if (!this->buyButton.IsHovered() && !this->isSelling)
			{
				this->scrollMenu.Update(_gameData);
			}
			if (!this->itemDescription.IsAnimationEnd())
			{
				this->itemDescription.AddTime(GameClock::Get().GetDeltaTime());
				this->itemDescription.Update();
			}
		}
	}
	else
	{
		if (!this->nextLevelDescription.IsAnimationEnd())
		{
			this->nextLevelDescription.AddTime(GameClock::Get().GetDeltaTime());
			this->nextLevelDescription.Update();
		}
		if (!this->upgradeButton.IsHovered())
		{
			this->dummiesScrollMenu.Update(_gameData);
		}
	}

	if (!BoolManager::Get()["SHOP_TUTORIAL_1_DONE"])
	{
		if (BoolManager::Get()["SHOP_TUTORIAL_0_DONE"])
		{
			BoolManager::Get()["SHOP_TUTORIAL_1_DONE"] = true;
			Json::Value json = JsonManager::LoadFromFile("Contents/Data/Texts/ShopTutorial1.json")["text"];
			this->DisplayTextOnUpgradeMenu(json);
		}
		else
		{
			Json::Value json = JsonManager::LoadFromFile("Contents/Data/Texts/ShopTutorial0.json")["text"];
			this->DisplayTextOnBuyMenu(json);
			BoolManager::Get()["SHOP_TUTORIAL_0_DONE"] = true;
		}
	}
}

void Shop::PollEvent(GameData& _gameData)
{
	if (PollSellMenuEvent(_gameData))
	{
		this->isDelete = false;
		return;
	}
	
	ControlsManager& controlsMng = ControlsManager::Get();
	if (controlsMng.IsPressedInput(_gameData.gameEvent, "CANCEL")
		|| ((_gameData.gameEvent.type == sf::Event::KeyPressed && _gameData.gameEvent.key.code == sf::Keyboard::Escape)))
	{
		if (this->isSelling)
		{
			this->isDelete = false;
			this->SwitchSellMode();
		}
		else if (this->buyButton.IsHovered())
		{
			this->isDelete = false;
			this->buyButton.CallOnOut();
		}
		else if (this->upgradeButton.IsHovered())
		{
			this->isDelete = false;
			this->upgradeButton.CallOnOut();
		}
		else if (!this->isBuyMenuActive)
		{
			this->isDelete = false;
			this->SwitchMenu();
		}
		else if (!this->isSwitchingMenu)
		{
			this->isDelete = true;
		}
	}
	else if (this->isBuyMenuActive)
	{
		if (this->isSelling
			&& _gameData.gameEvent.type == sf::Event::MouseButtonPressed
			&& _gameData.gameEvent.mouseButton.button == sf::Mouse::Left
			&& !this->sellMenu.GetGlobalBounds().contains(Screen::Get().GetMousePosition(_gameData.window)) && controlsMng.GetControllerType() == KEYBOARD)
		{
			this->SwitchSellMode();
		}
		else if (!this->isSelling)
		{
			this->buyButton.PollEvent(_gameData);
			if (!this->buyButton.IsHovered())
			{
				this->scrollMenu.PollEvent(_gameData);
			}
			if (_gameData.gameEvent.type == sf::Event::MouseMoved
				&& Screen::Get().GetMousePosition(_gameData.window).x > WORKING_WIDTH - SCROLL_TRIGGER_DISTANCE && controlsMng.GetControllerType() == KEYBOARD)
			{
				this->SwitchMenu();
			}
		}
	}
	else
	{
		this->upgradeButton.PollEvent(_gameData);
		if (!this->upgradeButton.IsHovered())
		{
			this->dummiesScrollMenu.PollEvent(_gameData);
		}
		if (_gameData.gameEvent.type == sf::Event::MouseMoved
			&& Screen::Get().GetMousePosition(_gameData.window).x < SCROLL_TRIGGER_DISTANCE && controlsMng.GetControllerType() == KEYBOARD)
		{
			this->SwitchMenu();
		}
	}
}

void Shop::Display(sf::RenderWindow& _window)
{
	DrawManager::Get().Draw(_window, "SHOP_BACKGROUND");
	DrawManager::Get().Draw(_window, "SHOP_SELLER");
	DrawManager::Get().Draw(_window, "SHOP");
	DrawManager::Get().Draw(_window, "SHOP_FOREGROUND");
	DrawManager::Get().Draw(_window, "HUD_CANDIES");
}

std::ostream& operator << (std::ostream& _buffer, Shop& _shop)
{
	const std::vector<MenuItem*>& items = _shop.scrollMenu.GetItems();
	for (size_t i = 0; i < items.size(); ++i)
	{
		_buffer << ((ShopButton*)items[i])->GetCount();
		if (i < items.size() - 1)
		{
			_buffer << ' ';
		}
	}
	return _buffer;
}

std::istream& operator >> (std::istream& _buffer, Shop& _shop)
{
	const std::vector<MenuItem*>& items = _shop.scrollMenu.GetItems();
	if (items.size() != 0)
	{
		for (size_t i = 0; i < items.size(); ++i)
		{
			sf::Uint16 count;
			_buffer >> count;
			((ShopButton*)items[i])->SetCount(count);
		}
	}

	return _buffer;
}

void Shop::InitItems()
{
	this->itemIndices.clear();
	Json::Value json = JsonManager::LoadFromFile(this->filePath);
	for (sf::Uint32 i = 0; i < json.size(); ++i)
	{
		auto it = Item::prefabs.find(json[i]["id"].asString());
		if (it != Item::prefabs.end())
		{
			this->AddItem(it->second->GetId(), json[i]["nb"].asInt());
		}
	}

	std::sort(this->scrollMenu.GetItems().begin(), this->scrollMenu.GetItems().end(), [](const MenuItem* _itemA, const MenuItem* _itemB)
		{
			return ((ShopButton*)_itemA)->GetShopIndex() < ((ShopButton*)_itemB)->GetShopIndex();
		});

	ImageButton* arrowButton = new ImageButton();
	arrowButton->SetTexture(TexturesManager::Get()["SHOP_ARROW"]);
	arrowButton->setOrigin(arrowButton->GetRect().width * 0.5f, arrowButton->GetRect().height * 0.5f);
	arrowButton->setScale(2.f, 2.f);
	arrowButton->setPosition(WORKING_WIDTH - arrowButton->GetRect().width * 1.5f, WORKING_HEIGHT * 0.5f);
	arrowButton->SetOnClickFct([this, arrowButton]
		{
			if (arrowButton->IsHovered())
			{
				this->SwitchMenu();
			}
		});
	arrowButton->SetOnOverFct(std::bind(&ImageButton::SetTexture, arrowButton, TexturesManager::Get()["SHOP_ARROW_HOVERED"]));
	arrowButton->SetOnOutFct(std::bind(&ImageButton::SetTexture, arrowButton, TexturesManager::Get()["SHOP_ARROW"]));
	this->scrollMenu.AddItem(std::move(arrowButton));
}

void Shop::InitBuyMenu()
{
	this->scrollMenu.SetSize({ WORKING_WIDTH * WORKING_WIDTH / Screen::Get().GetWidth(), WORKING_HEIGHT * WORKING_HEIGHT / Screen::Get().GetHeight() });
	this->scrollMenu.SetVerticalMode(false);

	this->itemTitle.setFont(FontsManager::Get()["SPOOKY"]);
	this->itemTitle.setOutlineThickness(2.f);
	this->itemTitle.setOutlineColor(sf::Color::Black);
	this->itemTitle.setStyle(sf::Text::Bold);
	this->itemTitle.setCharacterSize(40);
	this->itemTitle.setPosition(40.f, this->dialogueBoxBackground.getPosition().y + 20.f);

	this->itemCostText.setFont(FontsManager::Get()["SPOOKY"]);
	this->itemCostText.setCharacterSize(50);
	this->itemCostText.setFillColor(sf::Color::Yellow);
	this->itemCostText.setOutlineThickness(2.f);
	this->itemCostText.setOutlineColor(sf::Color::Black);

	this->itemCostCandySprite.setTexture(TexturesManager::Get()["CANDY"]);
	this->itemCostCandySprite.setColor(sf::Color::Transparent);
	this->itemCostCandySprite.setOrigin(this->itemCostCandySprite.getGlobalBounds().width * 0.5f, this->itemCostCandySprite.getGlobalBounds().height * 0.5f);
	this->itemCostCandySprite.setScale(0.6f, 0.6f);

	this->buyButton.SetTexture(TexturesManager::Get()["BUY_BUTTON"]);
	this->buyButton.SetColor(sf::Color(255, 255, 255, 200));
	this->buyButton.SetHoveredColor(sf::Color::White);
	this->buyButton.setPosition(DIALOGUE_BOX_RIGHT_PART_CENTER_X - this->buyButton.GetRect().width * 0.5f, BUY_UPGRADE_BUTTON_POS_Y);
	this->buyButton.SetOnOverFct(std::bind(&ImageButton::SetTexture, &this->buyButton, TexturesManager::Get()["BUY_BUTTON_HOVER"]));
	this->buyButton.SetOnOutFct(std::bind(&ImageButton::SetTexture, &this->buyButton, TexturesManager::Get()["BUY_BUTTON"]));
	this->buyButton.SetOnClickFct(std::bind(&Shop::BuyItem, this));

	this->sellMenu.Init(*this->gameData, this->player);
	this->sellMenu.setPosition((WORKING_WIDTH - this->sellMenu.GetGlobalBounds().width) * 0.5f, WORKING_HEIGHT - SUITCASE_SHOWN_PART_HEIGHT);

	DrawManager::Get().AddObject("SHOP_BACKGROUND", this->scrollMenu);
	DrawManager::Get().AddObject("SHOP_FOREGROUND", this->buyButton);
	DrawManager::Get().AddObject("SHOP_FOREGROUND", this->itemTitle);
	DrawManager::Get().AddObject("SHOP_FOREGROUND", this->itemCostText);
	DrawManager::Get().AddObject("SHOP_FOREGROUND", this->itemCostCandySprite);
	DrawManager::Get().AddObject("SHOP_FOREGROUND", this->itemDescription);
	DrawManager::Get().AddObject("SHOP_FOREGROUND", this->nextLevelDescription);
	DrawManager::Get().AddObject("SHOP_FOREGROUND", this->sellMenu);
	DrawManager::Get().AddObject("SHOP_SELLER", this->seller[(sf::Uint8)Animation::IDLE]);
	DrawManager::Get().AddObject("SHOP_SELLER", this->seller[(sf::Uint8)Animation::BUY]);
	DrawManager::Get().AddObject("SHOP_SELLER", this->seller[(sf::Uint8)Animation::TALK]);
	DrawManager::Get().AddObject("SHOP_SELLER", this->seller[(sf::Uint8)Animation::SWIPE_LEFT]);
	DrawManager::Get().AddObject("SHOP_SELLER", this->seller[(sf::Uint8)Animation::SWIPE_RIGHT]);
	DrawManager::Get().SetObjectVisible("SHOP_SELLER", &this->seller[(sf::Uint8)Animation::BUY], false);
	DrawManager::Get().SetObjectVisible("SHOP_SELLER", &this->seller[(sf::Uint8)Animation::TALK], false);
	DrawManager::Get().SetObjectVisible("SHOP_SELLER", &this->seller[(sf::Uint8)Animation::SWIPE_LEFT], false);
	DrawManager::Get().SetObjectVisible("SHOP_SELLER", &this->seller[(sf::Uint8)Animation::SWIPE_RIGHT], false);
}

void Shop::InitUpgradeMenu()
{
	sf::Font& defaultFont = FontsManager::Get()["SPOOKY"];

	this->tieNameText.setFont(defaultFont);
	this->tieNameText.setOutlineThickness(2.f);
	this->tieNameText.setOutlineColor(sf::Color::Black);
	this->tieNameText.setCharacterSize(40);
	this->tieNameText.setPosition(WORKING_WIDTH + 40.f, this->dialogueBoxBackground.getPosition().y + 20.f);

	this->tieLevelText.setFont(defaultFont);
	this->tieLevelText.setStyle(sf::Text::Bold);
	this->tieLevelText.setOutlineThickness(2.f);
	this->tieLevelText.setOutlineColor(sf::Color::Black);
	this->tieLevelText.setCharacterSize(40);
	this->tieLevelText.setPosition(WORKING_WIDTH + 40.f, this->dialogueBoxBackground.getPosition().y + 20.f);

	this->tiePriceText.setFont(defaultFont);
	this->tiePriceText.setCharacterSize(50);
	this->tiePriceText.setFillColor(sf::Color::Yellow);
	this->tiePriceText.setOutlineThickness(2.f);
	this->tiePriceText.setOutlineColor(sf::Color::Black);

	this->candyUpgradeSprite.setTexture(TexturesManager::Get()["CANDY"]);
	this->candyUpgradeSprite.setColor(sf::Color::Transparent);
	this->candyUpgradeSprite.setOrigin(this->candyUpgradeSprite.getGlobalBounds().width * 0.5f, this->candyUpgradeSprite.getGlobalBounds().height * 0.5f);
	this->candyUpgradeSprite.setScale(0.6f, 0.6f);

	this->dummiesScrollMenu.SetSize({ WORKING_WIDTH * WORKING_WIDTH / Screen::Get().GetWidth(), WORKING_HEIGHT * WORKING_HEIGHT / Screen::Get().GetHeight() });
	this->dummiesScrollMenu.setPosition(WORKING_WIDTH, 0.f);
	this->dummiesScrollMenu.SetVerticalMode(false);

	for (sf::Uint8 i = 0; i < this->dummies.size(); ++i)
	{
		this->dummies[i].type = Dummy::Type(i);
		this->dummies[i].button = new Button();
		this->dummies[i].button->setOrigin(TexturesManager::Get()["DUMMY_BOXER"].getSize().x * 0.5f, 0.f);
		this->dummies[i].button->SetOnClickFct(std::bind(&Shop::ShowDummy, this, Dummy::Type(i), true, false));
		this->dummies[i].button->SetOnOverFct(std::bind(&Button::SetColor, this->dummies[i].button, sf::Color(255, 255, 255, 200)));
		this->dummies[i].button->SetOnOutFct(std::bind(&Button::SetColor, this->dummies[i].button, sf::Color::White));
		this->dummiesScrollMenu.AddItem(this->dummies[i].button);
	}

	Button* arrowButton = new Button();
	arrowButton->SetTextureSprite(TexturesManager::Get()["SHOP_ARROW"]);
	arrowButton->setOrigin(arrowButton->GetRect().width * 0.5f, arrowButton->GetRect().height * 0.5f);
	arrowButton->setScale(2.f, 2.f);
	arrowButton->setRotation(180.f);
	arrowButton->setPosition(arrowButton->GetRect().width * 1.5f, WORKING_HEIGHT * 0.5f);
	arrowButton->SetOnClickFct(std::bind(&Shop::SwitchMenu, this));
	arrowButton->SetOnOverFct(std::bind(&Button::SetTextureSprite, arrowButton, TexturesManager::Get()["SHOP_ARROW_HOVERED"], false));
	arrowButton->SetOnOutFct(std::bind(&Button::SetTextureSprite, arrowButton, TexturesManager::Get()["SHOP_ARROW"], false));
	this->dummiesScrollMenu.AddItem(std::move(arrowButton));

	constexpr float DUMMIES_DISTANCE_FROM_BORDER = 75.f;
	constexpr float DUMMIES_OFFSET = (WORKING_WIDTH - 500.f) * 0.23f;
	constexpr float DUMMIES_LOW_LEVEL = 300.f;
	constexpr float DUMMIES_HIGH_LEVEL = 250.f;
	this->dummies[(sf::Uint8)Dummy::BOXER].button->setPosition(-DUMMIES_DISTANCE_FROM_BORDER + DUMMIES_OFFSET, DUMMIES_LOW_LEVEL);
	this->dummies[(sf::Uint8)Dummy::BOXER].tie = Tie::prefabs["BOXER"].get();
	this->dummies[(sf::Uint8)Dummy::CANDLE].button->setPosition(-DUMMIES_DISTANCE_FROM_BORDER + DUMMIES_OFFSET * 2.f, DUMMIES_HIGH_LEVEL);
	this->dummies[(sf::Uint8)Dummy::CANDLE].tie = Tie::prefabs["CANDLE"].get();
	this->dummies[(sf::Uint8)Dummy::MUSICIAN].button->setPosition(WORKING_WIDTH + DUMMIES_DISTANCE_FROM_BORDER - DUMMIES_OFFSET * 2.f, DUMMIES_HIGH_LEVEL);
	this->dummies[(sf::Uint8)Dummy::MUSICIAN].tie = Tie::prefabs["MUSIC"].get();
	this->dummies[(sf::Uint8)Dummy::PRINCESS].button->setPosition(WORKING_WIDTH + DUMMIES_DISTANCE_FROM_BORDER - DUMMIES_OFFSET, DUMMIES_LOW_LEVEL);
	this->dummies[(sf::Uint8)Dummy::PRINCESS].tie = Tie::prefabs["PRINCESS"].get();

	this->upgradeButton.SetTexture(TexturesManager::Get()["UPGRADE_BUTTON"]);
	this->upgradeButton.SetColor(sf::Color(255, 255, 255, 200));
	this->upgradeButton.SetHoveredColor(sf::Color::White);
	this->upgradeButton.setPosition(WORKING_WIDTH + DIALOGUE_BOX_RIGHT_PART_CENTER_X - this->upgradeButton.GetRect().width * 0.5f, BUY_UPGRADE_BUTTON_POS_Y);
	this->upgradeButton.SetOnOverFct(std::bind(&ImageButton::SetTexture, &this->upgradeButton, TexturesManager::Get()["UPGRADE_BUTTON_HOVER"]));
	this->upgradeButton.SetOnOutFct(std::bind(&ImageButton::SetTexture, &this->upgradeButton, TexturesManager::Get()["UPGRADE_BUTTON"]));
	this->upgradeButton.SetOnClickFct(std::bind(&Shop::UpgradeTie, this));

	DrawManager::Get().AddObject("SHOP_FOREGROUND", this->upgradeButton);
	DrawManager::Get().AddObject("SHOP_BACKGROUND", this->dummiesScrollMenu);
	DrawManager::Get().AddObject("SHOP_FOREGROUND", this->tieNameText);
	DrawManager::Get().AddObject("SHOP_FOREGROUND", this->tieLevelText);
	DrawManager::Get().AddObject("SHOP_FOREGROUND", this->tiePriceText);
	DrawManager::Get().AddObject("SHOP_FOREGROUND", this->candyUpgradeSprite);
}

bool Shop::PollSellMenuEvent(GameData& _gameData)
{
	if (this->isSelling)
	{
		if (this->sellMenu.PollEvent(_gameData))
		{
			return true;
		}
	}

	if (this->isBuyMenuActive && ControlsManager::Get().IsPressedInput(_gameData.gameEvent, "INVENTORY"))
	{
		this->SwitchSellMode();
		return true;
	}
	else if (this->isBuyMenuActive && _gameData.gameEvent.type == sf::Event::MouseButtonPressed && !this->isSelling)
	{
		if (_gameData.gameEvent.mouseButton.button == sf::Mouse::Left
			&& this->sellMenu.GetGlobalBounds().contains(Screen::Get().GetMousePosition(_gameData.window)) && ControlsManager::Get().GetControllerType() == KEYBOARD)
		{
			this->SwitchSellMode();
			return true;
		}
	}
	else if (_gameData.gameEvent.type == sf::Event::MouseMoved
		|| _gameData.gameEvent.type == sf::Event::MouseButtonPressed)
	{
		if (this->isBuyMenuActive && ControlsManager::Get().GetControllerType() == KEYBOARD)
		{
			if (this->sellMenu.GetGlobalBounds().contains(Screen::Get().GetMousePosition(_gameData.window)))
			{
				if (!this->isSelling)
				{
					this->sellMenu.SetColor(sf::Color(255, 255, 255, 200));
				}
				return true;
			}
			else
			{
				this->sellMenu.SetColor(sf::Color::White);
			}
		}
	}
	return false;
}

void Shop::DisplayTextOnBuyMenu(const Json::Value& _json)
{
	this->itemDescription = Paragraph(_json);
	this->itemDescription.setPosition(this->itemTitle.getPosition().x, this->itemTitle.getPosition().y + this->itemTitle.getGlobalBounds().height + 20.f);
	this->itemDescription.ResetAnimation();
	this->SetCurrentAnim(Animation::TALK);
}

void Shop::DisplayTextOnUpgradeMenu(const Json::Value& _json)
{
	this->nextLevelDescription = Paragraph(_json);
	this->nextLevelDescription.setPosition(this->tieNameText.getPosition().x, this->tieNameText.getPosition().y + this->tieNameText.getGlobalBounds().height + 20.f);
	this->nextLevelDescription.ResetAnimation();
	this->SetCurrentAnim(Animation::TALK);
}

void Shop::ShowItem(ShopButton* _button)
{
	const Item* item = _button->GetItem();
	if (this->selectedShopButton != _button)
	{
		this->selectedShopButton = _button;

		this->itemTitle.setString(item->GetName());
		this->itemCostText.setString(std::to_string(item->GetPrice()));
		SetCenteredText(this->itemCostText);
		float offset = (this->itemCostText.getGlobalBounds().width + this->itemCostCandySprite.getGlobalBounds().width) * 0.25f + 10.f;
		this->itemCostText.setPosition(DIALOGUE_BOX_RIGHT_PART_CENTER_X - offset, PRICES_TEXT_POS_Y);
		this->itemCostCandySprite.setPosition(DIALOGUE_BOX_RIGHT_PART_CENTER_X + offset, PRICES_TEXT_POS_Y);
		this->itemCostCandySprite.setColor(sf::Color::White);

		Json::Value json = JsonManager::LoadFromFile("Contents/Data/Texts/ShopItemDescription.json")["text"];
		json["content"][0]["value"] = item->GetDescription();
		this->DisplayTextOnBuyMenu(json);

		if (!_button->IsEmpty()
			&& item->GetPrice() <= this->player->GetPossessedSweets()
			&& ControlsManager::Get().GetControllerType() == CONTROLLER)
		{
			this->buyButton.CallOnOver();
		}

		Audio::Get().PlaySoundNoSecurity("SHOP_SELECT");
	}
}

void Shop::BuyItem()
{
	if (this->selectedShopButton)
	{
		this->SetCurrentAnim(Animation::BUY);
		this->selectedShopButton->Buy();

		if (this->selectedShopButton->IsEmpty() || this->selectedShopButton->GetItem()->GetPrice() > this->player->GetPossessedSweets())
		{
			this->buyButton.CallOnOut();
		}
	}
}

void Shop::SwitchSellMode()
{
	this->sellMenu.SetColor(sf::Color::White);
	this->isSellMenuMoving = true;
	this->isSelling = !this->isSelling;
	this->sellMenuScrollTime = 0.f;
	this->buyButton.CallOnOut();
	this->scrollMenu[this->scrollMenu.GetNbItems() - 1]->CallOnOut();
	if (this->isSelling)
	{
		this->sellMenu.Init(*this->gameData, this->player);
		Audio::Get().PlaySoundNoSecurity("OPEN_SELLING");
	}
	else
	{
		Audio::Get().PlaySoundNoSecurity("CLOSE_SELLING");
	}
}

void Shop::SwitchMenu()
{
	if (this->isSwitchingMenu)
		return;

	this->isSwitchingMenu = true;
	this->isBuyMenuActive = !this->isBuyMenuActive;
	this->scrollTime = 0.f;
	this->UpdateDummies();
	if (this->isBuyMenuActive)
	{
		this->SetCurrentAnim(Animation::SWIPE_RIGHT);
	}
	else
	{
		this->SetCurrentAnim(Animation::SWIPE_LEFT);
	}
	Audio::Get().PlaySoundNoSecurity("BUTTON_PRESSED");
}

void Shop::ShowDummy(Dummy::Type _dummy, bool _force, bool _isUpgrade)
{
	if (!this->dummies[_dummy].isUnlocked)
		return;

	Tie* tie = this->dummies[_dummy].tie;
	if (this->selectedDummy != _dummy || _force)
	{
		if (this->selectedDummy != Dummy::COUNT)
		{
			this->dummies[this->selectedDummy].button->SetColor(sf::Color::White);
		}
		this->dummies[_dummy].button->SetColor(sf::Color(255, 255, 255, 200));

		this->selectedDummy = _dummy;

		this->candyUpgradeSprite.setColor(sf::Color::White);

		switch (_dummy)
		{
		case Dummy::BOXER:
			this->tieNameText.setFillColor(sf::Color(38, 34, 250));
			break;
		case Dummy::CANDLE:
			this->tieNameText.setFillColor(sf::Color(250, 170, 39));
			break;
		case Dummy::MUSICIAN:
			this->tieNameText.setFillColor(sf::Color(138, 86, 186));
			break;
		case Dummy::PRINCESS:
			this->tieNameText.setFillColor(sf::Color(249, 165, 198));
			break;
		}

		if (tie->IsLevelMax())
		{
			this->tieNameText.setString(tie->GetName());
			this->tieLevelText.setString(" - Max Level");
			this->tiePriceText.setString("");
			this->candyUpgradeSprite.setColor(sf::Color::Transparent);
			this->upgradeButton.CallOnOut();
		}
		else
		{
			this->tieNameText.setString(tie->GetName());
			this->tieLevelText.setString(" - Level " + std::to_string(tie->GetLevel()));
			this->tiePriceText.setString(std::to_string(tie->GetNextLevelPrice()));
			if (tie->GetNextLevelPrice() <= this->player->GetPossessedSweets()
				&& ControlsManager::Get().GetControllerType() == CONTROLLER
				&& !tie->IsLevelMax())
			{
				this->upgradeButton.CallOnOver();
			}
		}

		SetCenteredText(this->tiePriceText);
		float offset = (this->tiePriceText.getGlobalBounds().width + this->candyUpgradeSprite.getGlobalBounds().width) * 0.25f + 10.f;
		this->tieLevelText.setPosition(this->tieNameText.getPosition().x + this->tieNameText.getGlobalBounds().width, this->tieNameText.getPosition().y);
		this->tiePriceText.setPosition(DIALOGUE_BOX_RIGHT_PART_CENTER_X - offset, PRICES_TEXT_POS_Y);
		this->candyUpgradeSprite.setPosition(DIALOGUE_BOX_RIGHT_PART_CENTER_X + offset, PRICES_TEXT_POS_Y);

		Json::Value json;
		if (tie->GetId() == "MUSIC")
		{
			json = JsonManager::LoadFromFile("Contents/Data/Texts/ShopMusicTieDescription.json")["text"];
			json["content"][0]["value"] = tie->GetNextLevelDescription();
			json["content"][1]["value"] = "\nNumber of turns applying effect :\n - ";
			json["content"][3]["value"] = " : 2";
			json["content"][7]["value"] = " : 3";
			json["content"][11]["value"] = " : 4";
			json["content"][21]["value"] = " : 5";
		}
		else
		{
			json = JsonManager::LoadFromFile("Contents/Data/Texts/ShopTieDescription.json")["text"];
			if (tie->IsLevelMax())
			{
				json["content"][3]["value"] = " : " + std::to_string(this->GetDamages(0, false));
				json["content"][7]["value"] = " : " + std::to_string(this->GetDamages(1, false));
				json["content"][11]["value"] = " : " + std::to_string(this->GetDamages(2, false));
			}
			else
			{
				json["content"][0]["value"] = tie->GetNextLevelDescription();

				json["content"][3]["value"] = " : " + std::to_string(this->GetDamages(0, false)) + " -> ";
				json["content"][4]["value"] = std::to_string(this->GetDamages(0, true));

				json["content"][7]["value"] = " : " + std::to_string(this->GetDamages(1, false)) + " -> ";
				json["content"][8]["value"] = std::to_string(this->GetDamages(1, true));

				json["content"][11]["value"] = " : " + std::to_string(this->GetDamages(2, false)) + " -> ";
				json["content"][12]["value"] = std::to_string(this->GetDamages(2, true));
			}
		}
		this->DisplayTextOnUpgradeMenu(json);

		this->tiePreview.setTexture(*tie->GetTexture());
		this->tiePreview.setOrigin(this->tiePreview.getGlobalBounds().width * 0.5f, this->tiePreview.getGlobalBounds().height * 0.5f);
		this->tiePreview.setPosition(DIALOGUE_BOX_RIGHT_PART_CENTER_X - 75.f, this->dialogueBoxBackground.getPosition().y + this->dialogueBoxBackground.getGlobalBounds().height * 0.25f);
	
		if (!_isUpgrade)
		{
			Audio::Get().PlaySoundNoSecurity("SHOP_SELECT");
		}
	}
}

void Shop::UpgradeTie()
{
	if (this->selectedDummy == Dummy::COUNT)
		return;

	Tie* tie = this->dummies[this->selectedDummy].tie;
	sf::Int16 price = tie->GetNextLevelPrice();
	if (this->player->GetPossessedSweets() - price >= 0 && !tie->IsLevelMax())
	{
		std::string boolean = tie->GetName() + "_" + std::to_string(tie->GetLevel() + 1);
		std::transform(boolean.begin(), boolean.end(), boolean.begin(), ::toupper);
		BoolManager::Get()[boolean] = true; // set true level up boolean
		tie->IncreaseLevel();
		this->player->AddSweets(-price);
		this->ShowDummy(this->selectedDummy, true, true);
		this->SetCurrentAnim(Animation::BUY);
		Audio::Get().PlaySound("IMPROVE_TIE");
		if (tie->IsLevelMax() || tie->GetNextLevelPrice() > this->player->GetPossessedSweets())
		{
			this->upgradeButton.CallOnOut();
		}
	}
	else
	{
		Audio::Get().PlaySound("CANNOT_PURCHASE");
	}
}

void Shop::UpdateDummies()
{
	for (auto& dummy : this->dummies)
	{
		dummy.isUnlocked = false;
		dummy.button->SetTextureSprite(TexturesManager::Get()["DUMMY_MYSTERY"]);
	}
	auto ties = this->player->GetTies();
	for (auto& tie : ties)
	{
		if (tie->GetId() == "BOXER")
		{
			this->dummies[(sf::Uint8)Dummy::BOXER].isUnlocked = true;
			this->dummies[(sf::Uint8)Dummy::BOXER].button->SetTextureSprite(TexturesManager::Get()["DUMMY_BOXER"]);
		}
		else if (tie->GetId() == "CANDLE")
		{
			this->dummies[(sf::Uint8)Dummy::CANDLE].isUnlocked = true;
			this->dummies[(sf::Uint8)Dummy::CANDLE].button->SetTextureSprite(TexturesManager::Get()["DUMMY_CANDLE"]);
		}
		else if (tie->GetId() == "MUSIC")
		{
			this->dummies[(sf::Uint8)Dummy::MUSICIAN].isUnlocked = true;
			this->dummies[(sf::Uint8)Dummy::MUSICIAN].button->SetTextureSprite(TexturesManager::Get()["DUMMY_MUSICIAN"]);
		}
		else if (tie->GetId() == "PRINCESS")
		{
			this->dummies[(sf::Uint8)Dummy::PRINCESS].isUnlocked = true;
			this->dummies[(sf::Uint8)Dummy::PRINCESS].button->SetTextureSprite(TexturesManager::Get()["DUMMY_PRINCESS"]);
		}
	}
}

void Shop::MoveView(float _velocity)
{
	this->background.move(_velocity, 0.f);
	this->dialogueBoxBackground.move(_velocity, 0.f);
	this->scrollMenu.move(_velocity, 0.f);
	this->buyButton.move(_velocity, 0.f);
	this->itemTitle.move(_velocity, 0.f);
	this->itemCostText.move(_velocity, 0.f);
	this->itemCostCandySprite.move(_velocity, 0.f);
	this->itemDescription.move(_velocity, 0.f);
	this->sellMenu.move(_velocity, 0.f);
	this->upgradeButton.move(_velocity, 0.f);
	this->dummiesScrollMenu.move(_velocity, 0.f);
	this->tieNameText.move(_velocity, 0.f);
	this->tieLevelText.move(_velocity, 0.f);
	this->tiePriceText.move(_velocity, 0.f);
	this->candyUpgradeSprite.move(_velocity, 0.f);
	this->nextLevelDescription.move(_velocity, 0.f);
	this->tiePreview.move(_velocity, 0.f);
}

void Shop::SetCurrentAnim(Animation _anim)
{
	DrawManager::Get().SetObjectVisible("SHOP_SELLER", &this->seller[(sf::Uint8)this->currentAnim], false);
	DrawManager::Get().SetObjectVisible("SHOP_SELLER", &this->seller[(sf::Uint8)_anim]);
	this->seller[(sf::Uint8)_anim].Reset();
	this->seller[(sf::Uint8)_anim].Play();
	this->currentAnim = _anim;
}

int Shop::GetDamages(int _succesLevel, bool _nextLevel) const
{
	Tie* tie = this->dummies[(sf::Uint8)this->selectedDummy].tie;
	float levelCoef = _nextLevel ? tie->GetNextLevelCoef() : tie->GetLevelCoef();
	float multiplicator;
	switch (this->selectedDummy)
	{
	case Dummy::BOXER:
		if (_succesLevel == 0) // OK
		{
			multiplicator = BOXER_OK_MULTIPLICATOR;
		}
		else if (_succesLevel == 1) // GREAT
		{
			multiplicator = BOXER_GREAT_MULTIPLICATOR;
		}
		else // EXCELLENT
		{
			multiplicator = BOXER_PERFECT_EXCELLENT_MULTIPLICATOR;
		}
		break;

	default:
		if (_succesLevel == 0) // OK
		{
			multiplicator = CANDLE_OK_MULTIPLICATOR;
		}
		else if (_succesLevel == 1) // GREAT
		{
			multiplicator = CANDLE_GREAT_MULTIPLICATOR;
		}
		else // EXCELLENT
		{
			multiplicator = CANDLE_PERFECT_EXCELLENT_MULTIPLICATOR;
		}
	}
	return int(this->player->GetDamage() * multiplicator * levelCoef);
}