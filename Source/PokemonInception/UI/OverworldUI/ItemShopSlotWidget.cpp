// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemShopSlotWidget.h"

void UItemShopSlotWidget::OnBuyClicked()
{
	BuyClicked.Broadcast(Item);
	RefreshDelegate.Broadcast(this);
}

void UItemShopSlotWidget::OnSellClicked()
{
	SellClicked.Broadcast(Item);
	RefreshDelegate.Broadcast(this);
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

void UItemShopSlotWidget::SetBuyState(bool enabled)
{
	Buy->OnClicked.RemoveDynamic(this, &UItemShopSlotWidget::OnBuyClicked);

	if (enabled) {
		Buy->OnClicked.AddDynamic(this, &UItemShopSlotWidget::OnBuyClicked);
		return;
	}
}

void UItemShopSlotWidget::SetSellState(bool enabled)
{
	Sell->OnClicked.RemoveDynamic(this, &UItemShopSlotWidget::OnSellClicked);

	if (enabled) {
		Sell->OnClicked.AddDynamic(this, &UItemShopSlotWidget::OnSellClicked);
		return;
	}
}

void UItemShopSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Buy->OnClicked.AddDynamic(this, &UItemShopSlotWidget::OnBuyClicked);
	Sell->OnClicked.AddDynamic(this, &UItemShopSlotWidget::OnSellClicked);
}

void UItemShopSlotWidget::SetItem(FItemBaseStruct ItemStruct)
{
	Item = ItemStruct;
}

FItemBaseStruct UItemShopSlotWidget::GetItem()
{
	return Item;
}
