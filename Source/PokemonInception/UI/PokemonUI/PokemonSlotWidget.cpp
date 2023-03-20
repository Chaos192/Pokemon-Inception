// Fill out your copyright notice in the Description page of Project Settings.


#include "PokemonSlotWidget.h"

void UPokemonSlotWidget::OnPokemonClicked()
{
	PokemonClick.Broadcast(PokemonID);
}

void UPokemonSlotWidget::SetPokemonName(FText Name)
{
	PokemonName->SetText(Name);
}

void UPokemonSlotWidget::SetPokemonLevel(int Level)
{
	PokemonLevel->SetText(FText::FromString("Lv. " + FString::FromInt(Level)));
}

void UPokemonSlotWidget::SetPokemonImage(UTexture2D* Image)
{
	PokemonImage->SetBrushFromTexture(Image);
}

void UPokemonSlotWidget::SetPokemonHP(int CurrHP, int MaxHP)
{
	FText HP = FText::FromString(FString::FromInt(CurrHP) + "/" + FString::FromInt(MaxHP));
	PokemonHP->SetText(HP);

	if (CurrHP == 0) {
		PokemonButton->SetBackgroundColor(FLinearColor(0.583, 0.051, 0.051, 1));
	}

	float HpPercent = float(CurrHP) / float(MaxHP);
	HPBar->SetPercent(HpPercent);
}

void UPokemonSlotWidget::SetPokemonEXP(int CurrExp, int MaxExp)
{
	float ExpPercent = float(CurrExp) / float(MaxExp);
	EXPBar->SetPercent(ExpPercent);
}

void UPokemonSlotWidget::SetPokemon(int InPokemonID)
{
	PokemonID = InPokemonID;
}

void UPokemonSlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	PokemonButton->OnClicked.AddDynamic(this, &UPokemonSlotWidget::OnPokemonClicked);
}