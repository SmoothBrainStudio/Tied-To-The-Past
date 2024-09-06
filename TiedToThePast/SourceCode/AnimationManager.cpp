#include "Definitions.hpp"
#include "AnimationManager.hpp"

AnimationManager& AnimationManager::Get(void)
{
	static AnimationManager myInstance;
	return myInstance;
}

const bool AnimationManager::Add(const std::string& _fileName, const std::string& _key)
{
	const auto it = std::find_if(animationMap.begin(), animationMap.end(),
		[&](const std::pair<const std::string, std::unique_ptr<AnimationContainer>>& _fontPair)
		{
			return _fontPair.second->fileName == ANIM_PATH + _fileName;
		});

	if (it != animationMap.end())
	{
		return false;
	}
	else
	{
		std::unique_ptr<AnimationContainer> tempAnim = std::make_unique<AnimationContainer>();
		tempAnim->fileName = ANIM_PATH + _fileName;
		tempAnim->animation = LoadAnimation(ANIM_PATH + _fileName);
		animationMap[_key] = std::move(tempAnim);
		return true;
	}
}

void AnimationManager::Delete(const std::string& _key)
{
	const auto it = animationMap.find(_key);

	if (it != animationMap.end())
	{
		animationMap.erase(_key);
	}
}

Animation AnimationManager::operator[](const std::string& _key)
{
	const auto it = animationMap.find(_key);

	if (it != animationMap.end())
	{
		return animationMap[_key]->animation;
	}
	else
	{
		throw;
	}
}