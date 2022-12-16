// Fill out your copyright notice in the Description page of Project Settings.


#include "BagWidget.h"

void UBagWidget::OnBackClicked()
{
	BackClicked.Broadcast();
}

void UBagWidget::AddToWrapBox(UItemSlotWidget* ItemWidget)
{
	WrapBox->AddChildToWrapBox(ItemWidget);
}

void UBagWidget::ClearWrapBox()
{
	if (WrapBox->HasAnyChildren()) {
		WrapBox->ClearChildren();
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

void UBagWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Back->OnClicked.AddDynamic(this, &UBagWidget::OnBackClicked);
}
