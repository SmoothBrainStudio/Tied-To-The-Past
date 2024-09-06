#ifndef ANIMATION_MANAGER__HPP
#define ANIMATION_MANAGER__HPP

#include "Definitions.hpp"
#include "Animation.hpp"

static constexpr auto ANIM_PATH = "Contents/Data/Animations/";

class AnimationManager
{
private:
	AnimationManager() = default;

	struct AnimationContainer
	{
		std::string fileName;
		Animation animation;
	};

	std::unordered_map<std::string, std::unique_ptr<AnimationContainer>> animationMap;

public:
	~AnimationManager() = default;
	AnimationManager(const AnimationManager&) = delete;
	AnimationManager(AnimationManager&&) = delete;
	AnimationManager& operator=(const AnimationManager&) = delete;
	AnimationManager& operator=(AnimationManager&&) = delete;

	static AnimationManager& Get(void);

	void Delete(const std::string& _key);
	const bool Add(const std::string& _fileName, const std::string& _key);

	Animation operator[](const std::string& _key);

};

#endif // !ANIMATION_MANAGER__HPP
