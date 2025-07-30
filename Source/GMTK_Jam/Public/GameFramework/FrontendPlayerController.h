#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FrontendPlayerController.generated.h"

class UMainMenuWidget;

UCLASS()
class GMTK_JAM_API AFrontendPlayerController : public APlayerController
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

    // The Widget Blueprint class to use for the main menu.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;

private:
    // The instance of the main menu widget.
    UPROPERTY()
    TObjectPtr<UMainMenuWidget> MainMenuWidgetInstance;
};