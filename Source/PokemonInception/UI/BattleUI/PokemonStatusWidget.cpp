// Fill out your copyright notice in the Description page of Project Settings.


#include "PokemonStatusWidget.h"

void UPokemonStatusWidget::SetPokemonName(FText Name)
{
	PokemonName->SetText(Name);
}

void UPokemonStatusWidget::SetPokemonLevel(int Level)
{
	PokemonLevel->SetText(FText::FromString(FString::FromInt(Level)));
}

void UPokemonStatusWidget::SetPokemonHP(int CurrHP, int MaxHP)
{
	PokemonCurrHP->SetText(FText::FromString(FString::FromInt(CurrHP)));
	PokemonMaxHP->SetText(FText::FromString(FString::FromInt(MaxHP)));

	float HpPercent = float(CurrHP) / float(MaxHP);
	HPBar->SetPercent(HpPercent);
}
