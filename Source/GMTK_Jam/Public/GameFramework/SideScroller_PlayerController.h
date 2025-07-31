#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Sidescroller_PlayerController.generated.h"

class UInputAction;
class UMobileControlWidget;
class UPauseMenuWidget;

UCLASS()
class GMTK_JAM_API ASidescroller_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASidescroller_PlayerController();
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> PauseAction;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UMobileControlWidget> MobileWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UPauseMenuWidget> PauseMenuWidgetClass;


private:
	UPROPERTY()
	TObjectPtr<UMobileControlWidget> MobileWidgetInstance;
	UPROPERTY()
	TObjectPtr<UPauseMenuWidget> PauseMenuInstance;
};
