#include "Definitions.hpp"
#include "GameManager.hpp"

int main(void)
{
	#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif // _DEBUG

	std::unique_ptr<GameData> gameData = std::make_unique<GameData>(); // create gameData in the heap
	GameManager stateManager;

	stateManager.Init(*gameData);
	stateManager.Run(*gameData);

	return 0;
}