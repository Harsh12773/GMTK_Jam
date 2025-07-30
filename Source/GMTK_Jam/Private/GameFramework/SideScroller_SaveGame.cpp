#include "GameFramework/SideScroller_SaveGame.h"


USideScroller_SaveGame::USideScroller_SaveGame()
{
	LevelName = TEXT("");
	PlayerLocation = FVector::ZeroVector;
	PlayerHealth = 100.0f;
}