#include <SFML/System.hpp>

class NewTimer
{
public:
	NewTimer(void);
	~NewTimer(void);

private:
	sf::Clock clock;
};

NewTimer::NewTimer()
{
}

NewTimer::~NewTimer()
{
}