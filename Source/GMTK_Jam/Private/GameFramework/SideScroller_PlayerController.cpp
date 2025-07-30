#include "GameFramework/SideScroller_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/SideScroller_CameraManager.h"
#include "Widget/MobileControlWidget.h"
#include "Widget/PauseMenuWidget.h"

ASidescroller_PlayerController::ASidescroller_PlayerController()
{
	PlayerCameraManagerClass = ASidescroller_CameraManager::StaticClass();

}

void ASidescroller_PlayerController::BeginPlay()
{
	Super::BeginPlay();

#if PLATFORM_ANDROID
	if (MobileWidgetClass)
	{
		MobileWidgetInstance = CreateWidget<UMobileControlWidget>(this, MobileWidgetClass);
		if (MobileWidgetInstance)
		{
			MobileWidgetInstance->AddToViewport();

			bShowMouseCursor = false;
		}
	}
#endif
	SetInputMode(FInputModeGameOnly());

	if(PauseMenuWidgetClass)
		PauseMenuInstance = CreateWidget<UPauseMenuWidget>(this, PauseMenuWidgetClass);
}
