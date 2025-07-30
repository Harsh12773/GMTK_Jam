#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

class UButton;
class UConfirmationWidget;
class UOptionsMenuWidget;

UENUM(BlueprintType)
enum class EPausePendingAction : uint8
{
	None,
	QuitToMainMenu,
	QuitGame
};

UCLASS(Blueprintable, BlueprintType)
class GMTK_JAM_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void TogglePauseMenu();

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> ResumeButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> OptionsButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> QuitToMenuButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> QuitGameButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UOptionsMenuWidget> OptionsMenuWidget;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UConfirmationWidget> ConfirmationWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pause Menu")
	TSoftObjectPtr<UWorld> MainMenuLevel;

private:
	UFUNCTION()
	void OnResumeClicked();

	UFUNCTION()
	void OnOptionsClicked();

	UFUNCTION()
	void OnQuitToMenuClicked();

	UFUNCTION()
	void OnQuitGameClicked();

	UFUNCTION()
	void HandleConfirmation(bool bConfirmed);

	UFUNCTION()
	void HandleOptionsBack();

	void SetGamePaused(bool bIsPaused);

	EPausePendingAction PendingAction;
};
