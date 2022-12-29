// Fill out your copyright notice in the Description page of Project Settings.


#include "PokemonWidget.h"

void UPokemonWidget::OnBackClicked()
{
	BackClicked.Broadcast();
}

void UPokemonWidget::AddToWrapBox(UPokemonSlotWidget* PokemonWidget)
{
	WrapBox->AddChildToWrapBox(PokemonWidget);
}

void UPokemonWidget::ClearWrapBox()
{
	if (WrapBox->HasAnyChildren()) {
		WrapBox->ClearChildren();
	}
}

void UPokemonWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Back->OnClicked.AddDynamic(this, &UPokemonWidget::OnBackClicked);
}
