#include "Definitions.hpp"
#include "PieChart.hpp"
#include "Maths.hpp"

namespace selbaward
{

	PieChart::Slice::Slice()
		: size{ 0.1f }
		, scale{ 1.f }
		, explode{ 0.f }
		, color(sf::Color::White)
	{
	}

	PieChart::PieChart()
		: slices()
		, m_primitive{ sf::PrimitiveType::Triangles }
		, m_vertices()
		, m_size({ 32.f, 32.f })
	{
	}

	void PieChart::update()
	{
		const sf::Vector2f halfSize{ m_size / 2.f };
		unsigned int totalNumberOfTrianglesRequired{ 0u };
		std::vector<unsigned int> numberOfTrianglesRequiredPerSlice;
		for (auto& slice : slices)
		{
			numberOfTrianglesRequiredPerSlice.emplace_back(static_cast<unsigned int>(std::floor(1.f + slice.size * 50.f)));
			totalNumberOfTrianglesRequired += numberOfTrianglesRequiredPerSlice.back();
		}
		m_vertices.resize(totalNumberOfTrianglesRequired * 3);
		unsigned int currentVertex{ 0u };
		float currentAngle{ 0.f };
		for (unsigned int slice{ 0u }; slice < slices.size(); ++slice)
		{
			const float startAngle{ currentAngle };
			const float halfAngleDifference{ 180.f * slices[slice].size };
			const sf::Vector2f offset{ sf::Vector2f(std::sin(Maths::DegreesToRadians(startAngle + halfAngleDifference)), -std::cos(Maths::DegreesToRadians(startAngle + halfAngleDifference))) * slices[slice].explode };
			for (unsigned int triangle{ 0u }; triangle < numberOfTrianglesRequiredPerSlice[slice]; ++triangle)
			{
				m_vertices[currentVertex + 0].position = halfSize + sf::Vector2f(offset.x * halfSize.x, offset.y * halfSize.y);
				m_vertices[currentVertex + 1].position = halfSize + sf::Vector2f((offset.x + std::sin(Maths::DegreesToRadians(currentAngle))) * halfSize.x, (offset.y - std::cos(Maths::DegreesToRadians(currentAngle))) * halfSize.y) * slices[slice].scale;
				currentAngle += halfAngleDifference * 2.f / numberOfTrianglesRequiredPerSlice[slice];
				m_vertices[currentVertex + 2].position = halfSize + sf::Vector2f((offset.x + std::sin(Maths::DegreesToRadians(currentAngle))) * halfSize.x, (offset.y - std::cos(Maths::DegreesToRadians(currentAngle))) * halfSize.y) * slices[slice].scale;
				m_vertices[currentVertex + 0].color = slices[slice].color;
				m_vertices[currentVertex + 1].color = slices[slice].color;
				m_vertices[currentVertex + 2].color = slices[slice].color;
				currentVertex += 3;
			}
		}
	}

	sf::FloatRect PieChart::getLocalBounds() const
	{
		sf::Vector2f topLeft{ m_vertices[0].position };
		sf::Vector2f bottomRight{ topLeft };
		for (auto& vertex : m_vertices)
		{
			if (vertex.position.x < topLeft.x)
				topLeft.x = vertex.position.x;
			else if (vertex.position.x > bottomRight.x)
				bottomRight.x = vertex.position.x;
			if (vertex.position.y < topLeft.y)
				topLeft.y = vertex.position.y;
			else if (vertex.position.y > bottomRight.y)
				bottomRight.y = vertex.position.y;
		}
		return{ topLeft, bottomRight - topLeft };
	}

	sf::FloatRect PieChart::getGlobalBounds() const
	{
		const sf::Transform transform{ getTransform() };
		sf::Vector2f topLeft{ transform.transformPoint(m_vertices[0].position) };
		sf::Vector2f bottomRight{ topLeft };
		sf::Vector2f current;
		for (auto& vertex : m_vertices)
		{
			current = transform.transformPoint(vertex.position);
			if (current.x < topLeft.x)
				topLeft.x = current.x;
			else if (current.x > bottomRight.x)
				bottomRight.x = current.x;
			if (current.y < topLeft.y)
				topLeft.y = current.y;
			else if (current.y > bottomRight.y)
				bottomRight.y = current.y;
		}
		return{ topLeft, bottomRight - topLeft };
	}



	// PRIVATE

	void PieChart::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();

		if (m_vertices.size() > 0)
			target.draw(&m_vertices.front(), m_vertices.size(), m_primitive, states);
	}

} // namespace selbaward