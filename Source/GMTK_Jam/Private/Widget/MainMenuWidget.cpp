#include "Widget/MainMenuWidget.h"


#include "Components/Button.h"
#include "GMTK_Jam/Public/Widget/ConfirmationWidget.h"
#include "GMTK_Jam/Public/Widget/OptionsMenuWIdget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ContinueButton)
    {
        ContinueButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnContinueClicked);
        UpdateContinueButtonState();
    }
    if (NewGameButton) NewGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnNewGameClicked);
    if (OptionsButton) OptionsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnOptionsClicked);
    if (CreditsButton) CreditsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnCreditsClicked);
    if (QuitButton) QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitClicked);

    if (ConfirmationWidget)
    {
        ConfirmationWidget->SetVisibility(ESlateVisibility::Collapsed);
    }

    if (OptionsMenuWidget)
    {
        OptionsMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
        OptionsMenuWidget->OnBackRequested.AddDynamic(this, &UMainMenuWidget::HandleOptionsBack);
    }
    
    PendingAction = EPendingAction::None;
    SetFocus();
}

FReply UMainMenuWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    if (OptionsMenuWidget && OptionsMenuWidget->IsVisible())
    {
        return FReply::Unhandled(); 
    }

    if (InKeyEvent.GetKey() == EKeys::Escape)
    {
        OnQuitClicked();
        return FReply::Handled();
    }

    return FReply::Unhandled();
}

void UMainMenuWidget::UpdateContinueButtonState()
{
    if (ContinueButton)
    {
        const bool bSaveExists = DoesSaveGameExist();
        ContinueButton->SetIsEnabled(bSaveExists);
        ContinueButton->SetColorAndOpacity(bSaveExists ? FLinearColor::White : FLinearColor(0.5f, 0.5f, 0.5f));
    }
}

bool UMainMenuWidget::DoesSaveGameExist() const
{
    return UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0);
}

void UMainMenuWidget::OnContinueClicked()
{
    if (DoesSaveGameExist())
    {
        UGameplayStatics::OpenLevel(this, FName("YourGameLevelName"));
    }
}

void UMainMenuWidget::OnNewGameClicked()
{
    if (ConfirmationWidget)
    {
        PendingAction = EPendingAction::NewGame;
        ConfirmationWidget->OnConfirmationResult.AddDynamic(this, &UMainMenuWidget::HandleConfirmation);
        ConfirmationWidget->Show(FText::FromString(TEXT("Start a new game? Any unsaved progress will be lost.")));
    }
}

void UMainMenuWidget::OnOptionsClicked()
{
    if (OptionsMenuWidget)
    {
        OptionsMenuWidget->Show();
    }
}

void UMainMenuWidget::OnCreditsClicked()
{
    // Logic for showing credits
}

void UMainMenuWidget::OnQuitClicked()
{
    if (ConfirmationWidget)
    {
        PendingAction = EPendingAction::Quit;
        ConfirmationWidget->OnConfirmationResult.AddDynamic(this, &UMainMenuWidget::HandleConfirmation);
        ConfirmationWidget->Show(FText::FromString(TEXT("Are you sure you want to quit?")));
    }
}

void UMainMenuWidget::HandleConfirmation(bool bConfirmed)
{
    if (bConfirmed)
    {
        switch (PendingAction)
        {
        case EPendingAction::NewGame:
            UGameplayStatics::OpenLevelBySoftObjectPtr(this, MainGameLevel);
            break;
        case EPendingAction::Quit:
            UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
            break;
        }
    }
    PendingAction = EPendingAction::None;
}

void UMainMenuWidget::HandleOptionsBack()
{
    SetFocus();
}
