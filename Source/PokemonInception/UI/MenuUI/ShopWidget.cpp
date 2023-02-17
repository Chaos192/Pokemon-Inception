// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopWidget.h"

void UShopWidget::OnExitClicked()
{
	ExitClicked.Broadcast();
}

void UShopWidget::DisplayInShop(UItemShopSlotWidget* ItemWidget)
{
	WrapBox->AddChildToWrapBox(ItemWidget);
}

void UShopWidget::ShowText(FString Message)
{
	TextBox->SetText(FText::FromString(Message));
}

void UShopWidget::ClearShop()
{
	if (WrapBox->HasAnyChildren()) {
		WrapBox->ClearChildren();
	}
}

void UShopWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Exit->OnClicked.AddDynamic(this, &UShopWidget::OnExitClicked);
}
