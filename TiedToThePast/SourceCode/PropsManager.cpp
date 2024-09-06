#include "Definitions.hpp"
#include "PropsManager.hpp"

void PropsManager::Init(void)
{
	auto& screenMng = Screen::Get();

	transparentShader.loadFromFile("Contents/Assets/Shaders/Transparent.frag", sf::Shader::Fragment);

	transparentBackground.create(unsigned int(screenMng.GetWidth()), unsigned int(screenMng.GetHeight()));
	transparentBackground.clear();

	playerTransparentBackground.create(unsigned int(screenMng.GetWidth()), unsigned int(screenMng.GetHeight()));
	playerTransparentBackground.clear();

	transparentShader.setUniform("mask", transparentBackground.getTexture());
	transparentShader.setUniform("player", playerTransparentBackground.getTexture());
}

void PropsManager::Add(const Json::Value& _propData, const std::string& _section, const bool& _isBackground)
{
	if (_propData["animation"].empty())
	{
		if (_isBackground)
		{
			bgProps.push_back(std::make_unique<Prop>(_propData, _section));
		}
		else
		{
			props.push_back(std::make_unique<Prop>(_propData, _section));
		}
	}
	else
	{
		if (_isBackground)
		{
			bgPropsAnimated.push_back(std::make_unique<PropAnimated>(_propData, _section));
		}
		else
		{
			propsAnimated.push_back(std::make_unique<PropAnimated>(_propData, _section));
		}
	}
}

void PropsManager::Clear(void)
{
	props.clear();
	propsAnimated.clear();

	bgProps.clear();
	bgPropsAnimated.clear();
}

void PropsManager::Update(const Player& _player)
{
	for (auto& animatedProp : propsAnimated)
	{
		animatedProp->Update();
	}

	for (auto& prop : props)
	{
		prop->Update();
	}

	for (auto& bgPropsAnim : bgPropsAnimated)
	{
		bgPropsAnim->Update();
	}

	for (auto& bgProp : bgProps)
	{
		bgProp->Update();
	}
}

void PropsManager::DisplayTransparentShader(sf::RenderTarget& _window, Player& _player)
{
	auto& screenMng = Screen::Get();

	DrawManager::Get().SetRenderTextureShader("Exploration", &transparentShader);

	transparentBackground.clear(sf::Color::Transparent);
	transparentBackground.setView(_window.getView());

	playerTransparentBackground.clear(sf::Color::Transparent);
	playerTransparentBackground.setView(_window.getView());

	for (auto& propAnimated : propsAnimated)
	{
		if (propAnimated->animation.getPosition().y > _player.getPosition().y)
		{
			AnimatedSprite propAnimation = propAnimated->animation;
			propAnimation.setScale(screenMng.Scale(propAnimation.getScale()));
			propAnimation.setPosition(screenMng.Pos(propAnimation.getPosition()));
			transparentBackground.draw(propAnimation);
		}
	}

	for (auto& prop : props)
	{
		if (prop->sprite.getPosition().y > _player.getPosition().y)
		{
			sf::Sprite propSprite = prop->sprite;
			propSprite.setScale(screenMng.Scale(propSprite.getScale()));
			propSprite.setPosition(screenMng.Pos(propSprite.getPosition()));
			transparentBackground.draw(propSprite);
		}
	}

	sf::Sprite trsprtSprite = _player.GetSprite();
	trsprtSprite.setOrigin(_player.getOrigin());
	trsprtSprite.setScale(screenMng.Scale(_player.getScale()));
	trsprtSprite.setPosition(screenMng.Pos(_player.getPosition()));
	playerTransparentBackground.draw(trsprtSprite);

	playerTransparentBackground.display();
	transparentBackground.display();
}

const std::vector<std::unique_ptr<Prop>>& PropsManager::GetPropsVector(void)
{
	return props;
}

const std::vector<std::unique_ptr<PropAnimated>>& PropsManager::GetPropsAnimatedVector(void)
{
	return propsAnimated;
}