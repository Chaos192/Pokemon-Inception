// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemShopSlotWidget.h"

void UItemShopSlotWidget::OnBuyClicked()
{
	//BuyClicked.Broadcast();
}

void UItemShopSlotWidget::OnSellClicked()
{
	//SellClicked.Broadcast();
}

void UItemShopSlotWidget::SetItemName(FText Name)
{
	ItemName->SetText(Name);
}

void UItemShopSlotWidget::SetItemImage(UTexture2D* Image)
{
	ItemImage->SetBrushFromTexture(Image);
}

void UItemShopSlotWidget::SetItemCount(int Count)
{
	ItemCount->SetText(FText::FromString(FString::FromInt(Count)));
}

void UItemShopSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Buy->OnClicked.AddDynamic(this, &UItemShopSlotWidget::OnBuyClicked);
	Sell->OnClicked.AddDynamic(this, &UItemShopSlotWidget::OnSellClicked);
}
