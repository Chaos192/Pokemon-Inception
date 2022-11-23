// Fill out your copyright notice in the Description page of Project Settings.


#include "PokemonWidget.h"

void UPokemonWidget::OnBackClicked()
{
	BackClicked.Broadcast();
}

void UPokemonWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Back->OnClicked.AddDynamic(this, &UPokemonWidget::OnBackClicked);
}
