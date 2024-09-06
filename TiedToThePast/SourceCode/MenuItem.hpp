#ifndef MENUITEM_HPP
#define MENUITEM_HPP

#include "Common.hpp"
#include "GameData.hpp"
#include "Eventable.hpp"

class MenuItem : public sf::Drawable, public sf::Transformable, public Eventable
{
public:
	MenuItem() = default;
	virtual ~MenuItem() override;

	virtual void Update(GameData& _gameData);
	virtual void PollEvent(GameData& _gameData);

	void SetEnable(bool _bool);
	void SetVisible(bool _bool);
	void SetID(const unsigned int& _id);

	bool GetEnable();
	bool GetVisible();
	const unsigned int GetID();
	virtual sf::FloatRect GetRect() const;
	const bool& GetWasOver(void) const;

	unsigned int id = 0;
	std::string className;

	bool enable = true;
	bool visible = true;
	bool wasOver = false;
};

#endif // !MENUITEM_HPP
