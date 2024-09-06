#include "Definitions.hpp"
#include "ItemOnGround.hpp"

ItemOnGround::ItemOnGround(const sf::Vector2f& _position, const std::string& _itemId) :
	itemId(_itemId), time(0.f)
{
	this->setPosition(_position);

	const Item& item = *Item::prefabs.find(_itemId)->second;
	sf::Vector2f itemSize = sf::Vector2f(item.GetTexture()->getSize());
	this->sprite.setTexture(*item.GetTexture(), true);
	this->sprite.setOrigin(itemSize * 0.5f);

	this->shadow.setFillColor(sf::Color(0, 0, 0, 80));
	this->shadow.setRadius(itemSize.x * 0.5f);
	this->shadow.setScale(1.f, 0.5f);
	this->shadow.setOrigin(itemSize.x * 0.5f, 0.f);
	this->shadow.setPosition(0.f, itemSize.x * 0.75f);
}

ItemOnGround::~ItemOnGround()
{
	DrawManager::Get().DeleteObject("Exploration", this);
}

const std::string& ItemOnGround::GetItemId() const
{
	return this->itemId;
}

sf::FloatRect ItemOnGround::GetRect() const
{
	return this->sprite.getGlobalBounds();
}

bool ItemOnGround::IsClose(const sf::Vector2f& _position)
{
	float squaredDistance = Maths::GetSquaredLength(this->getPosition(), _position);
	float distanceToMove = DISTANCE_TO_MOVE + this->shadow.getRadius();
	if (squaredDistance <= distanceToMove * distanceToMove)
	{
		if (squaredDistance <= this->shadow.getRadius() * this->shadow.getRadius())
		{
			return true;
		}
		else
		{
			sf::Vector2f diff = _position - this->getPosition();
			this->move(diff * SPEED / Maths::GetSquaredLength(diff) * glMng.GetDeltaTime());
		}
	}
	return false;
}

void ItemOnGround::Update()
{
	this->time += glMng.GetDeltaTime();
	float factor = (sinf(this->time * 1.5f) + 1.f) * 0.5f;
	this->sprite.setPosition(0.f, factor * 20.f);
	this->shadow.setFillColor(sf::Color(0, 0, 0, 30 + sf::Uint8(factor * 40.f)));
}

void ItemOnGround::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
{
	_states.transform *= this->getTransform();
	_target.draw(this->shadow, _states);
	_target.draw(this->sprite, _states);
}