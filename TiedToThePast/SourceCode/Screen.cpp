#include "Definitions.hpp"
#include "Screen.hpp"
#include "DrawManager.hpp"
#include "ControlsManager.hpp"

Screen::~Screen()
{
}

Screen& Screen::Get(void)
{
	static Screen myInstance;
	return myInstance;
}

void Screen::Init(sf::RenderWindow& _window)
{
	workingAspectRatio = ((float)WORKING_WIDTH / (float)WORKING_HEIGHT);
	Restaure();
	Reload(_window);
}

void Screen::SetIcon(sf::RenderWindow& _window, const std::string& _filePath)
{
	sf::Image tempImage = sf::Image(); // temporary image for loading the file and putting it into window icon
	if (!tempImage.loadFromFile(_filePath))
	{
		Debug.warning("Failed to set window icon. Reason : Unable to get load icon image");
		return;
	}
	sf::Vector2u tempImageSize = tempImage.getSize();
	_window.setIcon(tempImageSize.x, tempImageSize.y, tempImage.getPixelsPtr());
}

void Screen::ChangeAspectRatio(void)
{
	float curWindowWidth = preferences["video"]["videoMode"]["width"].asFloat();
	float curWindowHeight = preferences["video"]["videoMode"]["height"].asFloat();

	aspectRatio = curWindowWidth / curWindowHeight;

	if (aspectRatio != workingAspectRatio)
	{
		scaleFactor = std::min(curWindowWidth / WORKING_WIDTH, curWindowHeight / WORKING_HEIGHT * aspectRatio);
	}
	else
	{
		scaleFactor = std::min(curWindowWidth / WORKING_WIDTH, curWindowHeight / WORKING_HEIGHT);
	}
}

sf::Vector2f Screen::Scale(const sf::Vector2f& _scale)
{
	return sf::Vector2f
	(
		scaleFactor * _scale.x,
		scaleFactor * _scale.y
	);
}

sf::Vector2f Screen::Scale(const float& _scaleX, const float& _scaleY)
{
	return sf::Vector2f
	(
		scaleFactor * _scaleX,
		scaleFactor * _scaleY
	);
}

sf::Vector2f Screen::Pos(const sf::Vector2f& _pos)
{
	float curWindowWidth = preferences["video"]["videoMode"]["width"].asFloat();
	float curWindowHeight = preferences["video"]["videoMode"]["height"].asFloat();

	return sf::Vector2f
	(
		(_pos.x / WORKING_WIDTH) * curWindowWidth,
		(_pos.y / WORKING_HEIGHT) * curWindowHeight
	);
}

sf::Vector2f Screen::Pos(const float& _x, const float& _y)
{
	float curWindowWidth = preferences["video"]["videoMode"]["width"].asFloat();
	float curWindowHeight = preferences["video"]["videoMode"]["height"].asFloat();

	return sf::Vector2f
	(
		(_x / WORKING_WIDTH) * curWindowWidth,
		(_y / WORKING_HEIGHT) * curWindowHeight
	);
}

float Screen::PosX(const float& _x)
{
	float curWindowWidth = preferences["video"]["videoMode"]["width"].asFloat();

	return (_x / WORKING_WIDTH) * curWindowWidth;
}

float Screen::PosY(const float& _y)
{
	float curWindowHeight = preferences["video"]["videoMode"]["height"].asFloat();

	return (_y / WORKING_HEIGHT) * curWindowHeight;
}

sf::Sprite& Screen::GetWinShaderSprite(void)
{
	return winShaderSprite;
}

const sf::Shader* Screen::GetWinShader(void) const
{
	return winShader;
}

sf::Texture& Screen::GetWinShaderTexture(void)
{
	return winShaderTexture;
}

sf::Image& Screen::GetWindowImage(void)
{
	windowImage = winShaderTexture.copyToImage();
	return windowImage;
}

void Screen::SetWinShader(sf::Shader* _shader)
{
	winShader = _shader;
}

void Screen::Apply(void)
{
	preferences = preferencesSave;
	ChangeAspectRatio();

	// Save in file
	std::ofstream file;
	Json::StreamWriterBuilder writer;

	file.open(PREFERENCES_PATH, std::ios::trunc);
	file << Json::writeString(writer, preferences);
	file.close();
}

void Screen::Reload(sf::RenderWindow& _window)
{
	// Window
	_window.close();

	// Mode
	sf::VideoMode mode;
	if (preferences["video"]["style"].asInt() == sf::Style::Fullscreen)
	{
		mode.width = sf::VideoMode::getFullscreenModes()[0].width;
		mode.height = sf::VideoMode::getFullscreenModes()[0].height;

		preferences["video"]["videoMode"]["width"] = mode.width;
		preferences["video"]["videoMode"]["height"] = mode.height;
		//SetVideoMode(mode.width, mode.height);
	}
	else
	{
		mode.width = std::min(preferences["video"]["videoMode"]["width"].asUInt(), sf::VideoMode::getFullscreenModes()[0].width);
		mode.height = std::min(preferences["video"]["videoMode"]["height"].asUInt(), sf::VideoMode::getFullscreenModes()[0].height);

		preferences["video"]["videoMode"]["width"] = mode.width;
		preferences["video"]["videoMode"]["height"] = mode.height;
		//SetVideoMode(mode.width, mode.height);
	}

	mode.bitsPerPixel = preferences["video"]["videoMode"]["bitsPerPixel"].asInt();
	// Title
	std::string title = preferences["video"]["title"].asString();
	// Style
	sf::Uint32 style = preferences["video"]["style"].asInt();
	// Settings
	sf::ContextSettings settings;
	settings.depthBits = preferences["video"]["depth"].asInt();
	settings.stencilBits = preferences["video"]["stencil"].asInt();
	settings.antialiasingLevel = preferences["video"]["antialiasing"].asInt();
	settings.majorVersion = preferences["video"]["major"].asInt();
	settings.minorVersion = preferences["video"]["minor"].asInt();
	settings.attributeFlags = preferences["video"]["attributes"].asInt();
	settings.sRgbCapable = preferences["video"]["sRgb"].asBool();

	_window.create(mode, title, style, settings);
	_window.clear();
	_window.display();

	_window.setFramerateLimit(preferences["video"]["framerateLimit"].asInt());
	_window.setVerticalSyncEnabled(preferences["video"]["verticalSync"].asBool());
	_window.setJoystickThreshold(JOYSTICK_MIN_STEP);
	_window.setKeyRepeatEnabled(false);
	SetIcon(_window, preferences["other"]["iconPath"].asString());

	// Ratio
	ChangeAspectRatio();

	// Shader
	unsigned int curWindowWidth = preferences["video"]["videoMode"]["width"].asInt();
	unsigned int curWindowHeight = preferences["video"]["videoMode"]["height"].asInt();

	winShaderTexture.create(curWindowWidth, curWindowHeight);
	winShaderSprite.setTexture(winShaderTexture);

	for (auto& rt : DrawManager::Get().GetRenderTextureMap())
	{
		rt.second.reset();
		rt.second = std::make_unique<DrawManager::RendererContainer>();
		rt.second->RT.create(curWindowWidth, curWindowHeight);
		rt.second->RTSprite.setTexture(rt.second->RT.getTexture());
	}
}

void Screen::Restaure()
{
	preferences = JsonManager::Get().LoadFromFile(PREFERENCES_PATH);
	preferencesSave = preferences;
}

float Screen::GetAspectRatio()
{
	return aspectRatio;
}

float Screen::GetScaleFactor()
{
	return scaleFactor;
}

sf::Vector2f Screen::GetMousePosition(void)
{
	float curWindowWidth = preferences["video"]["videoMode"]["width"].asFloat();
	float curWindowHeight = preferences["video"]["videoMode"]["height"].asFloat();
	sf::Vector2f mousePosition(sf::Mouse::getPosition());

	return sf::Vector2f((mousePosition.x / curWindowWidth) * WORKING_WIDTH, (mousePosition.y / curWindowHeight) * WORKING_HEIGHT);
}

sf::Vector2f Screen::GetMousePosition(sf::RenderWindow& _window)
{
	float curWindowWidth = preferences["video"]["videoMode"]["width"].asFloat();
	float curWindowHeight = preferences["video"]["videoMode"]["height"].asFloat();
	sf::Vector2f mousePosition(sf::Mouse::getPosition(_window));

	return sf::Vector2f((mousePosition.x / curWindowWidth) * WORKING_WIDTH, (mousePosition.y / curWindowHeight) * WORKING_HEIGHT);
}

// VIDEO FUNCTION
void Screen::SetVideoMode(sf::VideoMode _videoMode)
{
	preferencesSave["video"]["videoMode"]["width"] = _videoMode.width;
	preferencesSave["video"]["videoMode"]["height"] = _videoMode.height;
	preferencesSave["video"]["videoMode"]["bitsPerPixel"] = _videoMode.bitsPerPixel;
}

void Screen::SetVideoMode(const unsigned int& _width, const unsigned int& _height)
{
	preferencesSave["video"]["videoMode"]["width"] = _width;
	preferencesSave["video"]["videoMode"]["height"] = _height;
}

void Screen::SetVideoMode(const unsigned int& _width, const unsigned int& _height, const unsigned int& bitsPerPixel)
{
	preferencesSave["video"]["videoMode"]["width"] = _width;
	preferencesSave["video"]["videoMode"]["height"] = _height;
	preferencesSave["video"]["videoMode"]["bitsPerPixel"] = bitsPerPixel;
}

void Screen::SetWidth(const unsigned int& _width)
{
	preferencesSave["video"]["videoMode"]["width"] = _width;
}

void Screen::SetHeight(const unsigned int& _height)
{
	preferencesSave["video"]["videoMode"]["height"] = _height;
}

void Screen::SetBitsPerPixel(const unsigned int& _bitsPerPixel)
{
	preferencesSave["video"]["videoMode"]["bitsPerPixel"] = _bitsPerPixel;
}

void Screen::SetStyle(const sf::Uint32& _style)
{
	preferencesSave["video"]["style"] = _style;
}

void Screen::SetFramerateLimit(const unsigned int& _fps)
{
	preferencesSave["video"]["framerateLimit"] = _fps;
}

void Screen::SetVerticalSync(const bool& _sync)
{
	preferencesSave["video"]["verticalSync"] = _sync;
}

void Screen::SetAntiAliasing(const unsigned int& _antiAliasing)
{
	preferencesSave["video"]["settings"]["antialiasing"] = _antiAliasing;
}

void Screen::SetFullScreen()
{
	preferencesSave["video"]["style"] = sf::Style::Fullscreen;
}

void Screen::SetBorderless()
{
	preferencesSave["video"]["style"] = sf::Style::None;
}

void Screen::SetDefault()
{
	preferencesSave["video"]["style"] = sf::Style::Titlebar | sf::Style::Close;
}

const sf::VideoMode Screen::GetVideoMode(void) const
{
	sf::VideoMode videoMode;
	videoMode.width = preferences["video"]["videoMode"]["width"].asInt();
	videoMode.height = preferences["video"]["videoMode"]["height"].asInt();
	videoMode.bitsPerPixel = preferences["video"]["videoMode"]["bitsPerPixel"].asInt();

	return videoMode;
}

const float Screen::GetWidth(void) const
{
	return preferences["video"]["videoMode"]["width"].asFloat();
}

const float Screen::GetHeight(void) const
{
	return preferences["video"]["videoMode"]["height"].asFloat();
}

const unsigned int Screen::GetBitsPerPixel(void)
{
	return preferences["video"]["bitsPerPixel"].asInt();
}

const sf::Uint32 Screen::GetStyle(void) const
{
	return preferences["video"]["style"].asInt();
}

const unsigned int Screen::GetFramerateLimit(void)
{
	return preferences["video"]["framerateLimit"].asInt();
}

const bool Screen::GetVerticalSync(void)
{
	return preferences["video"]["verticalSync"].asBool();
}

const unsigned int Screen::GetAntiAliasing(void)
{
	return preferences["video"]["settings"]["antialiasing"].asInt();
}

void Screen::DrawWinShader(sf::RenderWindow& _window)
{
	if (sf::Shader::isAvailable() && winShader != nullptr)
	{
		winShaderTexture.update(_window);
		winShaderSprite.setPosition(_window.mapPixelToCoords(sf::Vector2i(0, 0)));
		_window.draw(winShaderSprite, winShader);
	}
}
