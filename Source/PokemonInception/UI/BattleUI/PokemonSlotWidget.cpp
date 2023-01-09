// Fill out your copyright notice in the Description page of Project Settings.


#include "PokemonSlotWidget.h"

void UPokemonSlotWidget::OnPokemonClicked()
{
	PokemonClick.Broadcast(Pokemon);
}

void UPokemonSlotWidget::SetPokemonName(FText Name)
{
	PokemonName->SetText(Name);
}

void UPokemonSlotWidget::SetPokemonLevel(int Level)
{
	PokemonLevel->SetText(FText::FromString(FString::FromInt(Level)));
}

void UPokemonSlotWidget::SetPokemonImage(UTexture2D* Image)
{
	PokemonImage->SetBrushFromTexture(Image);
}

void UPokemonSlotWidget::SetPokemonHP(int CurrHP, int MaxHP)
{
	PokemonCurrHP->SetText(FText::FromString(FString::FromInt(CurrHP)));
	PokemonMaxHP->SetText(FText::FromString(FString::FromInt(MaxHP)));
	HPBar->SetPercent(CurrHP / MaxHP);
}

void UPokemonSlotWidget::SetPokemon(FPokemonStruct InPokemon)
{
	Pokemon = InPokemon;
}

void UPokemonSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PokemonButton->OnClicked.AddDynamic(this, &UPokemonSlotWidget::OnPokemonClicked);
}