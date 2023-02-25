// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemShopInfoWidget.h"

void UItemShopInfoWidget::OnOperationClicked()
{
	OperationClicked.Broadcast(ItemID);
}

void UItemShopInfoWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Operation->OnClicked.AddDynamic(this, &UItemShopInfoWidget::OnOperationClicked);
}

void UItemShopInfoWidget::SetOperation(FString OperationName)
{
	OperationText->SetText(FText::FromString(OperationName));
}

void UItemShopInfoWidget::SetDescription(FText Description)
{
	DescriptionText->SetText(Description);
}

void UItemShopInfoWidget::SetPrice(int Price)
{
	PriceText->SetText(FText::FromString("Price: " + FString::FromInt(Price) + "$"));
}

void UItemShopInfoWidget::SetID(int InItemID)
{
	ItemID = InItemID;
}
