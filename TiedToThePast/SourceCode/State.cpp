#include "Definitions.hpp"
#include "State.hpp"

State::~State()
{
	if (fade != nullptr)
	{
		delete fade;
		fade = nullptr;
	}
}
