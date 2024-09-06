#ifndef SELBAWARD_PIECHART_HPP
#define SELBAWARD_PIECHART_HPP

#include "Definitions.hpp"

namespace selbaward
{
	class PieChart : public sf::Drawable, public sf::Transformable
	{
	public:
		struct Slice
		{
			float size;
			float scale;
			float explode;
			sf::Color color;
			Slice();
		};
		std::vector<Slice> slices;

		PieChart();
		void update();
		void setSize(sf::Vector2f size);
		sf::Vector2f getSize() const;
		void setRadius(float radius);
		void setDiameter(float diameter);

		sf::FloatRect getLocalBounds() const;
		sf::FloatRect getGlobalBounds() const;

	private:
		sf::PrimitiveType m_primitive;
		std::vector<sf::Vertex> m_vertices;
		sf::Vector2f m_size;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	};

	inline void PieChart::setSize(sf::Vector2f size)
	{
		m_size = size;
	}

	inline sf::Vector2f PieChart::getSize() const
	{
		return m_size;
	}

	inline void PieChart::setRadius(float radius)
	{
		setDiameter(radius * 2.f);
	}

	inline void PieChart::setDiameter(float diameter)
	{
		m_size = { diameter, diameter };
	}

}
#endif // SELBAWARD_PIECHART_HPP

