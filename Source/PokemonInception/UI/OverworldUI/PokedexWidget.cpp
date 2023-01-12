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

void UPokedexWidget::ClearSlotBox()
{
	if (PokedexSlotBox->HasAnyChildren()) {
		PokedexSlotBox->ClearChildren();
	}
}

void UPokedexWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Back->OnClicked.AddDynamic(this, &UPokedexWidget::OnBackClicked);
}

