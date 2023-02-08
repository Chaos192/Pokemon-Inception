// Fill out your copyright notice in the Description page of Project Settings.


#include "PokemonWidget.h"

void UPokemonWidget::OnBackClicked()
{
	BackClicked.Broadcast();
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Native Construct"));
}

void UPokemonWidget::AddToWrapBox(UPokemonSlotWidget* PokemonWidget)
{
	WrapBox->AddChildToWrapBox(PokemonWidget);
}

void UPokemonWidget::AddToInfoWrapBox(UPokemonSummaryWidget* PokemonSummaryWidget)
{
	ClearSummaryBox();
	InfoWrapBox->AddChildToWrapBox(PokemonSummaryWidget);
}

void UPokemonWidget::ClearWrapBox()
{
	if (WrapBox->HasAnyChildren()) {
		WrapBox->ClearChildren();
	}
}

void UPokemonWidget::ClearSummaryBox()
{
	if (InfoWrapBox->HasAnyChildren()) {
		InfoWrapBox->ClearChildren();
	}
}

void UPokemonWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Back->OnClicked.AddDynamic(this, &UPokemonWidget::OnBackClicked);
}
