#include "Widget/OptionsMenuWIdget.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/ComboBoxString.h"
#include "Components/CheckBox.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"

void UOptionsMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (SaveButton) SaveButton->OnClicked.AddDynamic(this, &UOptionsMenuWidget::OnSaveClicked);
    if (BackButton) BackButton->OnClicked.AddDynamic(this, &UOptionsMenuWidget::OnBackClicked);

    if (ScalabilityComboBox)
    {
        ScalabilityComboBox->ClearOptions();
        ScalabilityComboBox->AddOption(TEXT("Low"));
        ScalabilityComboBox->AddOption(TEXT("Medium"));
        ScalabilityComboBox->AddOption(TEXT("High"));
        ScalabilityComboBox->AddOption(TEXT("Epic"));
        ScalabilityComboBox->AddOption(TEXT("Cinematic"));
    }
    
    PopulateResolutions();
}

FReply UOptionsMenuWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    if (InKeyEvent.GetKey() == EKeys::Escape)
    {
        OnBackClicked();
        return FReply::Handled();
    }
    return FReply::Unhandled();
}

void UOptionsMenuWidget::Show()
{
    LoadSettings();
    SetVisibility(ESlateVisibility::Visible);
    SetFocus();
}

void UOptionsMenuWidget::Hide()
{
    SetVisibility(ESlateVisibility::Collapsed);
}

void UOptionsMenuWidget::OnSaveClicked()
{
    ApplySettings();
}

void UOptionsMenuWidget::OnBackClicked()
{
    Hide();
    OnBackRequested.Broadcast();
}

void UOptionsMenuWidget::PopulateResolutions()
{
    if (!ResolutionComboBox) return;

    ResolutionComboBox->ClearOptions();
    TArray<FIntPoint> Resolutions;
    if (UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolutions))
    {
        for (const FIntPoint& Res : Resolutions)
        {
            ResolutionComboBox->AddOption(FString::Printf(TEXT("%d x %d"), Res.X, Res.Y));
        }
    }
}

void UOptionsMenuWidget::LoadSettings()
{
    UGameUserSettings* Settings = GEngine ? GEngine->GetGameUserSettings() : nullptr;
    if (!Settings) return;

    if (ScalabilityComboBox) ScalabilityComboBox->SetSelectedIndex(Settings->GetOverallScalabilityLevel());
    if (VSyncCheckBox) VSyncCheckBox->SetIsChecked(Settings->IsVSyncEnabled());
    
    if (ResolutionComboBox)
    {
        FIntPoint CurrentRes = Settings->GetScreenResolution();
        FString ResString = FString::Printf(TEXT("%d x %d"), CurrentRes.X, CurrentRes.Y);
        ResolutionComboBox->SetSelectedOption(ResString);
    }
    
    // Load volume settings here if implemented
}

void UOptionsMenuWidget::ApplySettings()
{
    UGameUserSettings* Settings = GEngine ? GEngine->GetGameUserSettings() : nullptr;
    if (!Settings) return;

    if (ScalabilityComboBox) Settings->SetOverallScalabilityLevel(ScalabilityComboBox->GetSelectedIndex());
    if (VSyncCheckBox) Settings->SetVSyncEnabled(VSyncCheckBox->IsChecked());

    if (ResolutionComboBox)
    {
        FString SelectedRes = ResolutionComboBox->GetSelectedOption();
        int32 X, Y;
        FString Left, Right;
        SelectedRes.Split(TEXT(" x "), &Left, &Right);
        X = FCString::Atoi(*Left);
        Y = FCString::Atoi(*Right);
        Settings->SetScreenResolution(FIntPoint(X, Y));
    }
    
    // Apply volume settings here
    
    Settings->ApplySettings(false);
}
