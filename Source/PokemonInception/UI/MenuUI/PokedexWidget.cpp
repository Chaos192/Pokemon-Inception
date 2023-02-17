// Fill out your copyright notice in the Description page of Project Settings.


#include "PokedexWidget.h"

void UPokedexWidget::OnBackClicked()
{
	BackClicked.Broadcast();
}

void UPokedexWidget::AddPokedexSlotToBox(UPokedexSlotWidget* PokedexSlot)
{
	PokedexSlotBox->AddChildToWrapBox(PokedexSlot);
}

void UPokedexWidget::AddPokedexInfoToBox(UPokedexInfoWidget* PokedexInfo)
{
	ClearInfoBox();
	InfoBox->AddChildToWrapBox(PokedexInfo);
}

void UPokedexWidget::ClearSlotBox()
{
	if (PokedexSlotBox->HasAnyChildren()) {
		PokedexSlotBox->ClearChildren();
	}
}

void UPokedexWidget::ClearInfoBox()
{
	if (InfoBox->HasAnyChildren()) {
		InfoBox->ClearChildren();
	}
}

void UPokedexWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Back->OnClicked.AddDynamic(this, &UPokedexWidget::OnBackClicked);
}

