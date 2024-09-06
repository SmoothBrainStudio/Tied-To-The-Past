#ifndef LOADINGSCREEN_HPP
#define LOADINGSCREEN_HPP

#include "State.hpp"
#include "Text.hpp"

class LoadingScreen
{
public:
	virtual ~LoadingScreen(void) = default;

	LoadingScreen(const LoadingScreen&) = delete;
	LoadingScreen(LoadingScreen&&) = delete;
	LoadingScreen& operator=(const LoadingScreen&) = delete;
	LoadingScreen& operator=(LoadingScreen&&) = delete;
	
	static LoadingScreen& Get(void);

	void Init(void);
	void Display(void);

	void SetWindow(sf::RenderWindow& _window);
	void SetLoading(const bool& _loading);

private:
	TexturesManager& texMng = TexturesManager::Get();
	static constexpr float MOVE_MAX = 150.f;

	LoadingScreen(void) = default;
	void InitTextures(void);
	void InitShaders(void);
	void InitAnimations(void);

	bool loading = false;
	float timer = 0.f;

	sf::Sprite loadingScreen;
	sf::Shader shader;
	sf::RenderWindow* window = nullptr;
	std::thread displayThread;
	sf::Mutex mutex;

	enum PlayerState
	{
		WALKING,
		DAMAGE,

		SIZE_STATE
	};
	
	int alpha = 0;
	float moveX = MOVE_MAX;
	sf::Vector2f playerPos;
	PlayerState playerState = WALKING;
	AnimatedSprite player[SIZE_STATE];
	sf::Vector2f initialScale = sf::Vector2f(-1.f, 1.f);
	sf::Vector2f initialScaleLoading = sf::Vector2f(1.f, 1.f);
};
#endif // !LOADINGSCREEN_HPP