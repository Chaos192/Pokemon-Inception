// Fill out your copyright notice in the Description page of Project Settings.


#include "PokemonStorageWidget.h"

void UPokemonStorageWidget::OnBackClicked()
{
	BackClicked.Broadcast();
}

void UPokemonStorageWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Back->OnClicked.AddDynamic(this, &UPokemonStorageWidget::OnBackClicked);
}

void UPokemonStorageWidget::AddToPartyBox(UPokemonIconWidget* PokemonIcon)
{
	PartyWrapBox->AddChildToWrapBox(PokemonIcon);
}

void UPokemonStorageWidget::AddToStorageBox(UPokemonIconWidget* PokemonIcon)
{
	StorageWrapBox->AddChildToWrapBox(PokemonIcon);
}

void UPokemonStorageWidget::ClearPartyBox()
{
	if (PartyWrapBox->HasAnyChildren()) {
		PartyWrapBox->ClearChildren();
	}
}

void UPokemonStorageWidget::ClearStorageBox()
{
	if (StorageWrapBox->HasAnyChildren()) {
		StorageWrapBox->ClearChildren();
	}
}
