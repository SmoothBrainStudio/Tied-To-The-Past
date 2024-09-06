#ifndef WARP__HPP
#define WARP__HPP

#include "Warp.hpp"

class WarpManager
{
public:
	using WarpArr = std::vector<Warp>;

	WarpManager(void) = default;
	~WarpManager(void) = default;
	Warp& operator[](const int& _key);

	void Clear(void);
	void PollEvent(sf::Event& _event, Player& _player);
	void Update(Player& _player);

	void AddWarp(const Warp& _warp);

	Warp* GetWarpCollide(void);

private:
	WarpArr warps;
};

#endif // !WARP__HPP
