#include "GameFramework/SideScroller_GameMode.h"

#include "GameFramework/SideScroller_PlayerCharacter.h"
#include "GameFramework/SideScroller_PlayerController.h"

ASidescroller_GameMode::ASidescroller_GameMode()
{
	DefaultPawnClass = ASidescroller_PlayerCharacter::StaticClass();
	PlayerControllerClass = ASidescroller_PlayerController::StaticClass();
}
