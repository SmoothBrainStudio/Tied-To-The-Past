#ifndef PREMENU_HPP
#define PREMENU_HPP

#include "State.hpp"
#include "SceneManager.hpp"

static constexpr float DURATION_DEFAULT_PREMENU = 6.f;

class PreMenu : public State
{
public:
	PreMenu() = default;
	~PreMenu() override;

	void Init(GameData& _gameData) override;
	void Update(GameData& _gameData) override;
	void PollEvent(GameData& _gameData) override;
	void Display(sf::RenderWindow& _window) override;

private:
	void ChangeTexture(const sf::Texture& _texture);

	enum class State
	{
		NONE,
		CREAJEUX,
		SMOOTHBRAIN
	};

	TexturesManager& texturesManager = TexturesManager::Get();
	Screen& scrMng = Screen::Get();

	sf::Sprite sprite;

	bool skip = false;
	State state = State::CREAJEUX;

	float totalTime = 0.f;
};

#endif // !PREMENU_HPP
