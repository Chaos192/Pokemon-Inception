// Fill out your copyright notice in the Description page of Project Settings.


#include "PokemonStorageWidget.h"

void UPokemonStorageWidget::OnBackClicked()
{
	BackClicked.Broadcast();
}

void UPokemonStorageWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Back->OnClicked.AddDynamic(this, &UPokemonStorageWidget::OnBackClicked);
}
