// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInfoWidget.h"

void UItemInfoWidget::OnUseClicked()
{
	UseClicked.Broadcast(ItemID);
}

void UItemInfoWidget::SetDescription(FText Description)
{
	DescriptionText->SetText(Description);
}

void UItemInfoWidget::SetID(int InItemID)
{
	ItemID = InItemID;
}

void UItemInfoWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Use->OnClicked.AddDynamic(this, &UItemInfoWidget::OnUseClicked);
}
