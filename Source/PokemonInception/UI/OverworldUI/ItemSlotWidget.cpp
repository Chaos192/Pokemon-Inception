// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSlotWidget.h"

void UItemSlotWidget::OnItemClicked()
{
	ItemClicked.Broadcast(ItemID);
}

void UItemSlotWidget::SetItemName(FText Name)
{
	ItemName->SetText(Name);
}

void UItemSlotWidget::SetItemImage(UTexture2D* Image)
{
	ItemImage->SetBrushFromTexture(Image);
}

void UItemSlotWidget::SetItemCount(int Count)
{
	ItemCount->SetText(FText::FromString(FString::FromInt(Count)));
}

void UItemSlotWidget::SetItemID(int InID)
{
	ItemID = InID;
}

void UItemSlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ItemButton->OnClicked.AddDynamic(this, &UItemSlotWidget::OnItemClicked);
}
