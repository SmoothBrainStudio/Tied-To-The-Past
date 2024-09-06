#include "Definitions.hpp"
#include "Color.hpp"

sf::Color HexToRgba(const std::string& hex)
{
	if (hex.empty()) return sf::Color();
	if (hex[0] == '#') {
		std::stringstream ss;
		ss << std::hex << hex.substr(1);
		unsigned int value;
		ss >> value;
		sf::Color color(value >> 16 & 0xFF, value >> 8 & 0xFF, value & 0xFF);
		if (hex.size() == 9) {
			color.a = value & 0xFF;
		}
		return color;
	}
	return sf::Color();
}
