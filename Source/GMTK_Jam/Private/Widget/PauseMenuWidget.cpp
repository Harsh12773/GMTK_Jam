#include "Widget/PauseMenuWidget.h"

#include "Components/Button.h"
#include "GMTK_Jam/Public/Widget/ConfirmationWidget.h"
#include "GMTK_Jam/Public/Widget/OptionsMenuWIdget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UPauseMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ResumeButton) ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnResumeClicked);
    if (OptionsButton) OptionsButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnOptionsClicked);
    if (QuitToMenuButton) QuitToMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnQuitToMenuClicked);
    if (QuitGameButton) QuitGameButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnQuitGameClicked);

    if (ConfirmationWidget)
    {
        ConfirmationWidget->SetVisibility(ESlateVisibility::Collapsed);
    }

    if (OptionsMenuWidget)
    {
        OptionsMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
        OptionsMenuWidget->OnBackRequested.AddDynamic(this, &UPauseMenuWidget::HandleOptionsBack);
    }

    PendingAction = EPausePendingAction::None;
}

FReply UPauseMenuWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    if (InKeyEvent.GetKey() == EKeys::Escape)
    {
        if (OptionsMenuWidget && !OptionsMenuWidget->IsVisible())
        {
            TogglePauseMenu();
            return FReply::Handled();
        }
    }
    return FReply::Unhandled();
}

void UPauseMenuWidget::TogglePauseMenu()
{
    const bool bIsVisible = IsVisible();
    SetGamePaused(!bIsVisible);

    if (!bIsVisible)
    {
        SetVisibility(ESlateVisibility::Visible);
        SetFocus();
    }
    else
    {
        SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UPauseMenuWidget::SetGamePaused(bool bIsPaused)
{
    APlayerController* PC = GetOwningPlayer();
    if (!PC) return;

    UGameplayStatics::SetGamePaused(this, bIsPaused);
    PC->bShowMouseCursor = bIsPaused;

    if (bIsPaused)
    {
        FInputModeUIOnly InputModeData;
        InputModeData.SetWidgetToFocus(TakeWidget());
        InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        PC->SetInputMode(InputModeData);
    }
    else
    {
        FInputModeGameOnly InputModeData;
        PC->SetInputMode(InputModeData);
    }
}

void UPauseMenuWidget::OnResumeClicked()
{
    TogglePauseMenu();
}

void UPauseMenuWidget::OnOptionsClicked()
{
    if (OptionsMenuWidget)
    {
        OptionsMenuWidget->Show();
    }
}

void UPauseMenuWidget::OnQuitToMenuClicked()
{
    if (ConfirmationWidget)
    {
        PendingAction = EPausePendingAction::QuitToMainMenu;
        ConfirmationWidget->OnConfirmationResult.AddDynamic(this, &UPauseMenuWidget::HandleConfirmation);
        ConfirmationWidget->Show(FText::FromString(TEXT("Quit to Main Menu? Any unsaved progress will be lost.")));
    }
}

void UPauseMenuWidget::OnQuitGameClicked()
{
    if (ConfirmationWidget)
    {
        PendingAction = EPausePendingAction::QuitGame;
        ConfirmationWidget->OnConfirmationResult.AddDynamic(this, &UPauseMenuWidget::HandleConfirmation);
        ConfirmationWidget->Show(FText::FromString(TEXT("Are you sure you want to quit the game?")));
    }
}

void UPauseMenuWidget::HandleConfirmation(bool bConfirmed)
{
    if (bConfirmed)
    {
        switch (PendingAction)
        {
        case EPausePendingAction::QuitToMainMenu:
            UGameplayStatics::SetGamePaused(this, false);
            UGameplayStatics::OpenLevelBySoftObjectPtr(this, MainMenuLevel);
            break;
        case EPausePendingAction::QuitGame:
            UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
            break;
        }
    }
    PendingAction = EPausePendingAction::None;
}

void UPauseMenuWidget::HandleOptionsBack()
{
    // Return focus to the main pause menu when backing out of options
    SetFocus();
}