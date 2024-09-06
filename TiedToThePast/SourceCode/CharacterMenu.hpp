#ifndef CHARACTER_MENU__HPP
#define CHARACTER_MENU__HPP

#include "State.hpp"
#include "Player.hpp"

class Mirror;
class Suitcase;
class TiesBox;
class DialogPlace;

class Mirror : public sf::Transformable, public sf::Drawable
{
private:
	// Manager(s)
	TexturesManager& texMng = TexturesManager::Get();
	FontsManager& fntMng = FontsManager::Get();
	DrawManager& drwMng = DrawManager::Get();
	GameClock& glMng = GameClock::Get();

	// Constantes
	const sf::Vector2f SIZE_BACKGROUND = sf::Vector2f(250.f, 485.f);
	const sf::Vector2f POSITION_HEALTH_TEXT = sf::Vector2f(60.f, 405.f);
	const sf::Vector2f POSITION_CANDY_TEXT = sf::Vector2f(164.f, 400.f);
	const sf::Vector2f POSITION_HEARTH = sf::Vector2f(38.f, 409.f);
	const sf::Vector2f POSITION_CANDY = sf::Vector2f(144.f, 426.f);

	// Member(s)
	sf::RectangleShape background;
	sf::RectangleShape reflection;
	AnimatedSprite player;
	sf::Text healthText;
	sf::Text candyText;
	sf::Sprite hearth;
	sf::Sprite candy;

	// Private function(s)
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
	Mirror() = default;
	~Mirror() = default;

	void Init(GameData& _gameData, std::shared_ptr<Player> _player);
	void Update(std::shared_ptr<Player> _playerData);

	void SetCostume(std::shared_ptr<Player> _player, const CostumeState& _costume);
	const sf::FloatRect GetGlobalBounds();

};

class Suitcase : public sf::Transformable, public sf::Drawable
{
protected:
	// Manager(s)
	TexturesManager& texMng = TexturesManager::Get();
	DrawManager& drwMng = DrawManager::Get();
	Screen& scrMng = Screen::Get();

	// Constantes
	const sf::Vector2f SIZE_BACKGROUND = sf::Vector2f(751.f, 811.f);
	const sf::Vector2f START_ORIGIN = sf::Vector2f(22.f, 346.f);
	const sf::Vector2f SIZE_ONE_BOX = sf::Vector2f(166.f, 168.f);
	const sf::Vector2f SIZE_ALL_BOX = sf::Vector2f(707.f, 349.f);
	static constexpr float SPACE_BETWEEN_BOXS = 14.f;
	static constexpr float OFFSET_OVER_MOUSE = 48.f;

	// Member(s)
	sf::RectangleShape background;
	sf::RectangleShape front1;
	sf::RectangleShape front2;
	std::array<sf::FloatRect, 8> mouseCollision;
	std::array<sf::Sprite, 8> objectBoxs;
	std::vector<const Consumable*> items;
	unsigned short scrollValue = 0;

	// Private function(s)
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
	Suitcase() = default;
	~Suitcase() = default;

	void Init(GameData& _gameData, std::shared_ptr<Player> _player);
	void Update(GameData& _gameData, std::shared_ptr<Player> _playerData, DialogPlace& _dialog);
	void PollEvent(sf::Event& _event);
	const sf::FloatRect GetGlobalBounds();

};

class TiesBox : public sf::Transformable, public sf::Drawable
{
private:
	// Manager(s)
	TexturesManager& texMng = TexturesManager::Get();
	DrawManager& drwMng = DrawManager::Get();
	Screen& scrMng = Screen::Get();

	// Constantes
	const sf::Vector2f BACKGROUND_SIZE = sf::Vector2f(482.f, 234.f);
	const sf::Vector2f START_ORIGIN = sf::Vector2f(9.f, 9.f);
	const sf::Vector2f SIZE_BOX = sf::Vector2f(156.f, 156.f);
	const sf::Vector2f SIZE_BOX_NORMAL = sf::Vector2f(110.f, 147.f);
	static constexpr float SPACE_BETWEEN_BOXS = 8.f;
	static constexpr float OFFSET_OVER_MOUSE = 48.f;

	// Member(s)
	sf::RectangleShape background;
	sf::RectangleShape front;
	std::array<sf::Sprite, 4> tieBoxs;
	std::array<sf::FloatRect, 4> mouseCollision;

	// Private function(s)
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
	TiesBox() = default;
	~TiesBox() = default;

	void Init(GameData& _gameData, std::shared_ptr<Player> _player);
	void Update(GameData& _gameData, std::shared_ptr<Player> _playerData, DialogPlace& _dialog);
	void PollEvent(GameData& _gameData, std::shared_ptr<Player> _player, Mirror& _mirror);

	const sf::FloatRect GetGlobalBounds();

};

class DialogPlace : public sf::Transformable, public sf::Drawable
{
private:
	// Manager(s)
	TexturesManager& texMng = TexturesManager::Get();
	FontsManager& fntMng = FontsManager::Get();
	DrawManager& drwMng = DrawManager::Get();
	Screen& scrMng = Screen::Get();

	// Constantes
	const sf::Vector2f POSITION_TITLE = sf::Vector2f(32.f, 32.f);
	const sf::Vector2f POSITION_DESCRIPTION = sf::Vector2f(48.f, 96.f);

	// Member(s)
	sf::RectangleShape background;
	sf::Text title;
	sf::Text description;

	// Private function(s)
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
	DialogPlace() = default;
	~DialogPlace() = default;

	void Init(GameData& _gameData, std::shared_ptr<Player> _player);
	void Update();
	const sf::FloatRect GetGlobalBounds();
	void SetTitle(const sf::String& _val);
	void SetDescription(const sf::String& _val);

};

class CharacterMenu : public State
{
private:
	// Manager(s)
	TexturesManager& texMng = TexturesManager::Get();
	ShadersManager& sdrMng = ShadersManager::Get();
	DrawManager& drwMng = DrawManager::Get();
	GameClock& glMng = GameClock::Get();
	Screen& scrMng = Screen::Get();

	// Constantes
	static constexpr float TIME_LILBOO_MOVEMENT = 0.001f;
	static constexpr float TIME_SHADER = 0.2f;
	static constexpr float BLUR_MAX = 6.f;
	const sf::Vector2f POSITION_MIROR = sf::Vector2f(227.f, 305.f);
	const sf::Vector2f POSITION_SUITCASE = sf::Vector2f(543.f, 5.f);
	const sf::Vector2f POSITION_TIES = sf::Vector2f(1374.f, 556.f);
	const sf::Vector2f POSITION_DIALOG = sf::Vector2f(543.f, 829.f);
	const sf::Vector2f POSITION_LILBOO_MIROR = sf::Vector2f(471.f, 444.f);
	const sf::Vector2f POSITION_LILBOO_SUITCASE = sf::Vector2f(1274.f, 771.f);
	const sf::Vector2f POSITION_LILBOO_TIES = sf::Vector2f(1861.f, 544.f);
	const sf::Vector2f POSITION_LILBOO_DIALOG = sf::Vector2f(1783.f, 950.f);

	// Struct(s) & Enum(s)
	enum class CurrentEvent
	{
		NONE,
		SUITCASE,
		TIES
	};

	// Private function(s)
	void LilbooGoto(const sf::Vector2f& _postition);

	// Member(s)
	sf::RectangleShape blackBackground;

	Mirror* miror = nullptr;
	Suitcase* suitcase = nullptr;
	TiesBox* tiesBox = nullptr;
	DialogPlace* dialogPlace = nullptr;
	std::shared_ptr<Player> player = nullptr;

	AnimatedSprite lilBoo;
	sf::Vector2f lilBooStartPosition;
	sf::Vector2f lilBooFinalPosition;
	float lilbooTotalTime = 0.f;
	float lilbooDistance = 0.f;
	float valueShader = 0.f;
	CurrentEvent currentEvent = CurrentEvent::SUITCASE;

public:
	CharacterMenu();
	~CharacterMenu() override;

	void Init(GameData& _gameData) override;
	void PollEvent(GameData& _gameData) override;
	void Update(GameData& _gameData) override;
	void Display(sf::RenderWindow& _window) override;

	void InitOnOpen(GameData& _gameData, std::shared_ptr<Player> _player);
	void SetPlayer(std::shared_ptr<Player> _player);
	void DisableShader();

};

#endif // !CHARACTER_MENU__HPP
