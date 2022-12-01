// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSlotWidget.h"

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
