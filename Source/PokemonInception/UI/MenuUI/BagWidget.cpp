// Fill out your copyright notice in the Description page of Project Settings.


#include "BagWidget.h"

void UBagWidget::OnBackClicked()
{
	BackClicked.Broadcast();
}

void UBagWidget::AddToItemBox(UItemSlotWidget* ItemWidget)
{
	ItemBox->AddChild(ItemWidget);
}

void UBagWidget::ClearItemBox()
{
	if (ItemBox->HasAnyChildren()) {
		ItemBox->ClearChildren();
	}
}

void UBagWidget::ShowInfo(UItemInfoWidget* ItemInfoWidget)
{
	ClearInfoBox();
	InfoBox->AddChildToWrapBox(ItemInfoWidget);
}

void UBagWidget::ClearInfoBox()
{
	if (InfoBox->HasAnyChildren()) {
		InfoBox->ClearChildren();
	}
}

void UBagWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Back->OnClicked.AddDynamic(this, &UBagWidget::OnBackClicked);
}
