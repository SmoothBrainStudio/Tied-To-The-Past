#include "Definitions.hpp"
#include "FightingHUD.hpp"

FightingHUD::FightingHUD(void)
{
}
FightingHUD::~FightingHUD(void)
{
	DrawManager::Get().DeleteObject("FIGHT_HUD", &lifeHUD.bar);
	DrawManager::Get().DeleteObject("FIGHT_HUD", &lifeHUD.heart);
	DrawManager::Get().DeleteObject("FIGHT_HUD", &lifeHUD.point);

	DrawManager::Get().DeleteObject("FIGHT_BG", &background);
}

void FightingHUD::InitBackground(void)
{
	background.setTexture(TexturesManager::Get()["BACKGROUND_FIGHTING"]);
	background.setPosition(-100.f, -90.f);
	DrawManager::Get().AddObject("FIGHT_BG", background, false);
}

void FightingHUD::InitLifeHUD(void)
{
	sf::Vector2f healthBarTextureSize = sf::Vector2f(TexturesManager::Get()["HUD_FIGHTING"].getSize());
	lifeHUD.bar.setTexture(TexturesManager::Get()["HUD_FIGHTING"], true);
	lifeHUD.bar.setOrigin(healthBarTextureSize.x / 2.f, healthBarTextureSize.y);
	lifeHUD.bar.setPosition(healthBarTextureSize.x / 2.f + 64.f, WORKING_HEIGHT);

	lifeHUD.heart.setTexture(TexturesManager::Get()["HEARTH"], true);
	sf::FloatRect rectHeart = lifeHUD.heart.getGlobalBounds();
	lifeHUD.heart.setOrigin(rectHeart.width / 2.f, rectHeart.height / 2.f);
	lifeHUD.heart.setPosition(lifeHUD.bar.getPosition() - sf::Vector2f(healthBarTextureSize.x / 2.f - 24.f, 48.f));

	lifeHUD.point.setFont(FontsManager::Get()["FIGHTING"]);
	lifeHUD.point.setFillColor(sf::Color(208, 241, 231));
	lifeHUD.point.setCharacterSize(100);
	lifeHUD.point.setOutlineThickness(2.f);
	lifeHUD.point.setOutlineColor(sf::Color(52, 15, 13));
	lifeHUD.point.setPosition(lifeHUD.bar.getPosition() - sf::Vector2f(0.f, 50.f));

	DrawManager::Get().AddObject("FIGHT_HUD", lifeHUD.bar);
	DrawManager::Get().AddObject("FIGHT_HUD", lifeHUD.heart);
	DrawManager::Get().AddObject("FIGHT_HUD", lifeHUD.point);
}

void FightingHUD::Update(Player& _player, const float& _timerEncounter)
{
	DrawManager::Get().SetObjectVisible("FIGHT_BG", &background, _timerEncounter < 0.5f);
	UpdateLifeHUD(_player);
}

void FightingHUD::UpdateLifeHUD(Player& _player)
{
	lifeHUD.time += GameClock::Get().GetDeltaTime();

	lifeHUD.point.setString(std::to_string(_player.GetHP()));

	const sf::FloatRect lifeTextRect = lifeHUD.point.getLocalBounds();
	lifeHUD.point.setOrigin(lifeTextRect.left + lifeTextRect.width / 2.f, lifeTextRect.top + lifeTextRect.height / 2.f);
	lifeHUD.point.setPosition(lifeHUD.bar.getPosition() - sf::Vector2f(0.f, sin(lifeHUD.time * 4.f) * 5.f + 50.f));

	const float deltaHearth = sin(lifeHUD.time * (2.f + 20.f * (_player.GetHPMax() - _player.GetHP()) / _player.GetHPMax())) * 0.1f;
	lifeHUD.heart.setScale(1.f + deltaHearth, 1.f + deltaHearth);
}