// Fill out your copyright notice in the Description page of Project Settings.


#include "PokedexSlotWidget.h"

void UPokedexSlotWidget::OnSlotClicked()
{
	SlotClicked.Broadcast(Data);
}

void UPokedexSlotWidget::InitEmptySlot(FName ID)
{
	PokemonID->SetText(FText::FromName(ID));
}

void UPokedexSlotWidget::InitFilledSlot(FPokemonBaseStruct PokedexData)
{
	Data = PokedexData;
	PokemonID->SetText(FText::FromName(PokedexData.PokemonID));
	PokemonImage->SetBrushFromTexture(PokedexData.Image);
	SlotButton->OnClicked.AddDynamic(this, &UPokedexSlotWidget::OnSlotClicked);
}

