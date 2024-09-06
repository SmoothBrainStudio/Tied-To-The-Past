#ifndef LILBOO_BOX__HPP
#define LILBOO_BOX__HPP

#include "Entity.hpp"
#include "DialogMap.hpp"

class LilBooBox : public Entity
{
public:

	static constexpr float DIALOG_BOX_OFFSET = 5.f;

	LilBooBox();

	~LilBooBox(void) override;

	void AddBox(DialogMap& _dialogMap);

	void Update(Player& _player);

	void Clear(void);

private:
	std::vector<DialogMap*> dialogsMap;
	AnimatedSprite animation;
};

#endif // !LILBOO_BOX__HPP