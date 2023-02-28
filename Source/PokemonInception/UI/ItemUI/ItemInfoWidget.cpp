// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInfoWidget.h"

void UItemInfoWidget::OnUseClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 4, FColor::Green, TEXT("Use Clicked!"));
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