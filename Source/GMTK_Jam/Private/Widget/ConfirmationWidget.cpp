#include "Widget/ConfirmationWidget.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

void UConfirmationWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (YesButton)
	{
		YesButton->OnClicked.AddDynamic(this, &UConfirmationWidget::OnYesClicked);
	}

	if (NoButton)
	{
		NoButton->OnClicked.AddDynamic(this, &UConfirmationWidget::OnNoClicked);
	}
}

void UConfirmationWidget::Show(const FText& Message)
{
	if (ConfirmationText)
	{
		ConfirmationText->SetText(Message);
	}
	SetVisibility(ESlateVisibility::Visible);
	SetFocus();
}

void UConfirmationWidget::Hide()
{
	SetVisibility(ESlateVisibility::Collapsed);
	if (OnConfirmationResult.IsBound())
	{
		OnConfirmationResult.Clear();
	}
}

void UConfirmationWidget::OnYesClicked()
{
	OnConfirmationResult.Broadcast(true);
	Hide();
}

void UConfirmationWidget::OnNoClicked()
{
	OnConfirmationResult.Broadcast(false);
	Hide();
}
