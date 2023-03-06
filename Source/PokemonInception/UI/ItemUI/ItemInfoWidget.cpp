// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInfoWidget.h"

void UItemInfoWidget::OnUseClicked()
{
	UseClicked.Broadcast(ItemID);
}

void UItemInfoWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Use->OnClicked.AddDynamic(this, &UItemInfoWidget::OnUseClicked);
}

void UItemInfoWidget::SetDescription(FText Description)
{
	DescriptionText->SetText(Description);
}

void UItemInfoWidget::SetID(int InItemID)
{
	ItemID = InItemID;
}

void UItemInfoWidget::SetUseButton(bool bUsable)
{
	if (bUsable) {
		Use->SetColorAndOpacity(FLinearColor(0, 0, 0, 1));
		Use->SetBackgroundColor(FLinearColor(0.5, 0.5, 0.5, 1));
	}
	else {
		Use->SetColorAndOpacity(FLinearColor(0, 0, 0, 0));
		Use->SetBackgroundColor(FLinearColor(0, 0, 0, 0));
	}
}
