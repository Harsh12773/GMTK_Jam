#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MobileControlWidget.generated.h"

// Forward declarations
class UButton;
class UUserWidget;

UCLASS()
class GMTK_JAM_API UMobileControlWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;


	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> LeftArrowButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> RightArrowButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> JumpButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> PauseButton;
	
	UPROPERTY(EditAnywhere, Category = "Pause Menu")
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;


	UFUNCTION()
	void OnLeftArrowPressed();
	UFUNCTION()
	void OnRightArrowPressed();
	void ApplyDirectionalInput(float Scale);


	UFUNCTION()
	void OnJumpButtonPressed();
	UFUNCTION()
	void OnJumpButtonReleased();

	UFUNCTION()
	void OnPauseButtonPressed();

	ACharacter* GetOwningPlayerCharacter() const;
};