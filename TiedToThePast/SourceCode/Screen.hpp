#ifndef SCREEN__HPP
#define SCREEN__HPP

#include "Definitions.hpp"
#include "JsonManager.hpp"

static constexpr unsigned int WORKING_WIDTH = 1920;
static constexpr unsigned int WORKING_HEIGHT = 1080;

class Screen
{
public:
	~Screen();
	Screen(const Screen&) = delete;
	Screen(Screen&&) = delete;
	Screen& operator=(const Screen&) = delete;
	Screen& operator=(Screen&&) = delete;

	static Screen& Get(void);

	void Init(sf::RenderWindow& _window);

	sf::Vector2f Scale(const sf::Vector2f& _scale);
	sf::Vector2f Scale(const float& _scaleX, const float& _scaleY);

	sf::Vector2f Pos(const sf::Vector2f& _pos);
	sf::Vector2f Pos(const float& _x, const float& _y);

	float PosX(const float& _x);
	float PosY(const float& _y);

	// Shader
	sf::Sprite& GetWinShaderSprite(void);
	const sf::Shader* GetWinShader(void) const;
	sf::Texture& GetWinShaderTexture(void);
	sf::Image& GetWindowImage(void);
	void SetWinShader(sf::Shader* _shader);

	void Apply(void);
	void Reload(sf::RenderWindow& _window);
	void Restaure();

	float GetAspectRatio();
	float GetScaleFactor();
	sf::Vector2f GetMousePosition(void);
	sf::Vector2f GetMousePosition(sf::RenderWindow& _window);

	// Video function
	void SetVideoMode(sf::VideoMode _videoMode);
	void SetVideoMode(const unsigned int& _width, const unsigned int& _height);
	void SetVideoMode(const unsigned int& _width, const unsigned int& _height, const unsigned int& bitsPerPixel);
	void SetWidth(const unsigned int& _width);
	void SetHeight(const unsigned int& _height);
	void SetBitsPerPixel(const unsigned int& _bitsPerPixel);
	void SetStyle(const sf::Uint32& _style);
	void SetFramerateLimit(const unsigned int& _fps);
	void SetVerticalSync(const bool& _sync);
	void SetAntiAliasing(const unsigned int& _antiAliasing);

	void SetFullScreen();
	void SetBorderless();
	void SetDefault();

	const sf::VideoMode GetVideoMode(void) const;
	const float GetWidth(void) const;
	const float GetHeight(void) const;
	const unsigned int GetBitsPerPixel(void);
	const sf::Uint32 GetStyle(void) const;
	const unsigned int GetFramerateLimit(void);
	const bool GetVerticalSync(void);
	const unsigned int GetAntiAliasing(void);

	void DrawWinShader(sf::RenderWindow& _window);

private:
	Screen() = default;

	void SetIcon(sf::RenderWindow& _window, const std::string& _filePath);
	void ChangeAspectRatio(void);

	Json::Value preferences;
	Json::Value preferencesSave;

	float aspectRatio = 0.f;
	float workingAspectRatio = 0.f;
	float scaleFactor = 0.f;
	sf::Image lightImage;
	sf::Image windowImage;
	sf::Texture winShaderTexture;
	sf::Sprite winShaderSprite;
	sf::Shader* winShader = nullptr;

};
#endif // !SCREEN_H
