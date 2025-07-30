#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UButton;
class UConfirmationWidget;
class UOptionsMenuWidget;

UENUM(BlueprintType)
enum class EPendingAction : uint8
{
	None,
	NewGame,
	Quit
};

UCLASS(Blueprintable, BlueprintType)
class GMTK_JAM_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> ContinueButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> NewGameButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> OptionsButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> CreditsButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> QuitButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UConfirmationWidget> ConfirmationWidget;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UOptionsMenuWidget> OptionsMenuWidget;
	
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> MainGameLevel;

private:
	UFUNCTION()
	void OnContinueClicked();

	UFUNCTION()
	void OnNewGameClicked();

	UFUNCTION()
	void OnOptionsClicked();

	UFUNCTION()
	void OnCreditsClicked();

	UFUNCTION()
	void OnQuitClicked();

	UFUNCTION()
	void HandleConfirmation(bool bConfirmed);

	UFUNCTION()
	void HandleOptionsBack();

	void UpdateContinueButtonState();
	bool DoesSaveGameExist() const;

	EPendingAction PendingAction;
	FString SaveSlotName = TEXT("DefaultSaveSlot");
};
