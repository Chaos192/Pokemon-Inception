// Fill out your copyright notice in the Description page of Project Settings.


#include "PokedexInfoWidget.h"

void UPokedexInfoWidget::SetPokedexInfo(FPokemonBaseStruct SpeciesData, FString Type)
{
	PokemonImage->SetBrushFromTexture(SpeciesData.Image);
	PokemonName->SetText(SpeciesData.Name);
	PokemonID->SetText(FText::FromName(SpeciesData.PokemonID));
	PokemonType->SetText(FText::FromString(Type));
	DescriptionText->SetText(SpeciesData.Info);
}
