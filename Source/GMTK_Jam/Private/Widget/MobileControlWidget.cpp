#include "Widget/MobileControlWidget.h"
#include "Components/Button.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

void UMobileControlWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (LeftArrowButton)
        LeftArrowButton->OnPressed.AddDynamic(this, &UMobileControlWidget::OnLeftArrowPressed);
    
    if (RightArrowButton)
        RightArrowButton->OnPressed.AddDynamic(this, &UMobileControlWidget::OnRightArrowPressed);
    

    if (JumpButton)
    {
        JumpButton->OnPressed.AddDynamic(this, &UMobileControlWidget::OnJumpButtonPressed);
        JumpButton->OnReleased.AddDynamic(this, &UMobileControlWidget::OnJumpButtonReleased);
    }
    
    if (PauseButton)
        PauseButton->OnClicked.AddDynamic(this, &UMobileControlWidget::OnPauseButtonPressed);
    
}

void UMobileControlWidget::OnPauseButtonPressed()
{
    if (PauseMenuWidgetClass)
    {
        UGameplayStatics::SetGamePaused(GetWorld(), true);

        UUserWidget* PauseMenu = CreateWidget<UUserWidget>(GetOwningPlayer(), PauseMenuWidgetClass);
        if (PauseMenu)
        {
            PauseMenu->AddToViewport();
            FInputModeUIOnly InputMode;
            GetOwningPlayer()->SetInputMode(InputMode);
            GetOwningPlayer()->bShowMouseCursor = true;
        }
    }
}

void UMobileControlWidget::OnLeftArrowPressed()
{
    ApplyDirectionalInput(-1.0f);
}

void UMobileControlWidget::OnRightArrowPressed()
{
    ApplyDirectionalInput(1.0f);
}

void UMobileControlWidget::ApplyDirectionalInput(float Scale)
{
    APawn* PlayerPawn = GetOwningPlayerPawn();
    if (!PlayerPawn || !PlayerPawn->Controller)
        return;

    const FRotator ControlRotation = PlayerPawn->Controller->GetControlRotation();
    const FRotator YawRotation(0, ControlRotation.Yaw, 0);

    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

    PlayerPawn->AddMovementInput(ForwardDirection, Scale);
}


void UMobileControlWidget::OnJumpButtonPressed()
{
    if (ACharacter* PlayerCharacter = GetOwningPlayerCharacter())
        PlayerCharacter->Jump();
}

void UMobileControlWidget::OnJumpButtonReleased()
{
    if (ACharacter* PlayerCharacter = GetOwningPlayerCharacter())
        PlayerCharacter->StopJumping();
}

ACharacter* UMobileControlWidget::GetOwningPlayerCharacter() const
{
    if (APlayerController* PC = GetOwningPlayer())
        return Cast<ACharacter>(PC->GetPawn());
    return nullptr;
}