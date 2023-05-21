// Fill out your copyright notice in the Description page of Project Settings.


#include "PokedexSlotWidget.h"

void UPokedexSlotWidget::OnSlotClicked()
{
	SlotClicked.Broadcast(ID);
}

void UPokedexSlotWidget::InitEmptySlot(FName InID)
{
	PokemonID->SetText(FText::FromName(InID));
}

void UPokedexSlotWidget::InitFilledSlot(FPokemonBaseStruct PokedexData)
{
	ID = PokedexData.PokemonID;
	PokemonID->SetText(FText::FromName(PokedexData.PokemonID));
	PokemonImage->SetBrushFromTexture(PokedexData.Image);

	SlotButton->OnClicked.AddDynamic(this, &UPokedexSlotWidget::OnSlotClicked);
}

