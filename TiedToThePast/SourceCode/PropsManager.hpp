#ifndef PROPS_MANAGER__HPP
#define PROPS_MANAGER__HPP

#include "Common.hpp"
#include "Tiled.hpp"
#include "Player.hpp"

struct Prop
{
	Prop(const Json::Value& _propData, const std::string& _section)
	{
		section = _section;

		sprite = Tiled::LoadSprite(_propData);
		DrawManager::Get().AddObject(section, sprite);

		if (!_propData["sound"].empty())
		{
			soundKey = Tiled::LoadSoundFX(_propData);
		}
	}
	~Prop(void)
	{
		if (!soundKey.empty())
		{
			Audio::Get().DeleteSound(soundKey);
		}
		DrawManager::Get().DeleteObject(section, &sprite);
	}

	void Update(void)
	{
		if (!soundKey.empty())
		{
			Audio::Get().SetRelativeToListner(soundKey, false, 500.f, 1.3f);
			Audio::Get().SetSoundPosition(soundKey, sf::Vector2f(sprite.getPosition().x, sprite.getPosition().y));
			Audio::Get().PlaySound(soundKey, 90.f);
		}
	}

	std::string soundKey;
	sf::Sprite sprite;
	std::string section;
};
struct PropAnimated
{
	PropAnimated(const Json::Value& _propData, const std::string& _section)
	{
		section = _section;

		animation = Tiled::LoadAnimatedSprite(_propData);
		DrawManager::Get().AddObject(section, animation);

		if (!_propData["sound"].empty())
		{
			propsSoundKey = Tiled::LoadSoundFX(_propData);
		}
	}
	~PropAnimated(void)
	{
		if (!propsSoundKey.empty())
		{
			Audio::Get().DeleteSound(propsSoundKey);
		}
		DrawManager::Get().DeleteObject(section, &animation);
	}

	void Update(void)
	{
		animation.Update();

		if (!propsSoundKey.empty())
		{
			auto& AudioMng = Audio::Get();
			AudioMng.SetRelativeToListner(propsSoundKey, false, 500.f, 1.3f);
			AudioMng.SetSoundPosition(propsSoundKey, sf::Vector2f(animation.getPosition().x, animation.getPosition().y));
			AudioMng.PlaySound(propsSoundKey, 90.f);
		}
	}

	std::string propsSoundKey;
	AnimatedSprite animation;
	std::string section;
};

class PropsManager
{
public:
	PropsManager(void)
	{
		Init();
	}
	~PropsManager(void) = default;

	void Init(void);
	void Add(const Json::Value& _propData, const std::string& _section, const bool& _isBackground = false);
	void Clear(void);
	void Update(const Player& _player);
	void DisplayTransparentShader(sf::RenderTarget& _window, Player& _player);

	const std::vector<std::unique_ptr<Prop>>& GetPropsVector();
	const std::vector<std::unique_ptr<PropAnimated>>& GetPropsAnimatedVector(void);

private:
	std::vector<std::unique_ptr<Prop>> bgProps;
	std::vector<std::unique_ptr<PropAnimated>> bgPropsAnimated;

	std::vector<std::unique_ptr<Prop>> props;
	std::vector<std::unique_ptr<PropAnimated>> propsAnimated;

	// shaders variables
	sf::RenderTexture transparentBackground;
	sf::RenderTexture playerTransparentBackground;
	sf::Shader transparentShader;
};
#endif // !PROPS_MANAGER__HPP