// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopWidget.h"

void UShopWidget::OnExitClicked()
{
	ExitClicked.Broadcast();
}

void UShopWidget::OnBuyModeClicked()
{
	BuyModeClicked.Broadcast();
}

void UShopWidget::OnSellModeClicked()
{
	SellModeClicked.Broadcast();
}

void UShopWidget::DisplayInShop(UItemSlotWidget* ItemWidget)
{
	ItemBox->AddChild(ItemWidget);
}

void UShopWidget::ShowShopInfo(UItemShopInfoWidget* InfoWidget)
{
	InfoBox->AddChildToWrapBox(InfoWidget);
}

void UShopWidget::ShowMoney(FString Message)
{
	Money->SetText(FText::FromString(Message));
}

void UShopWidget::ClearShop()
{
	if (ItemBox->HasAnyChildren()) {
		ItemBox->ClearChildren();
	}
}

void UShopWidget::ClearShopInfo()
{
	if (InfoBox->HasAnyChildren()) {
		InfoBox->ClearChildren();
	}
}

void UShopWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Exit->OnClicked.AddDynamic(this, &UShopWidget::OnExitClicked);
	BuyMode->OnClicked.AddDynamic(this, &UShopWidget::OnBuyModeClicked);
	SellMode->OnClicked.AddDynamic(this, &UShopWidget::OnSellModeClicked);
}
