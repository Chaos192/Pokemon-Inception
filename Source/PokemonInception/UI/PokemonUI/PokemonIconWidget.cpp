// Fill out your copyright notice in the Description page of Project Settings.


#include "PokemonIconWidget.h"

void UPokemonIconWidget::OnPokemonClicked()
{
	PokemonClick.Broadcast(PokemonID);
}

void UPokemonIconWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	PokemonButton->OnClicked.AddDynamic(this, &UPokemonIconWidget::OnPokemonClicked);
}

void UPokemonIconWidget::SetPokemonName(FText Name)
{
	PokemonName->SetText(Name);
}

void UPokemonIconWidget::SetPokemonLevel(int Level)
{
	PokemonLevel->SetText(FText::FromString("Lv." + FString::FromInt(Level)));
}

void UPokemonIconWidget::SetPokemonImage(UTexture2D* Image)
{
	PokemonImage->SetBrushFromTexture(Image);
}

void UPokemonIconWidget::SetPokemonHP(int CurrHP, int MaxHP)
{
	PokemonHP->SetText(FText::FromString(FString::FromInt(CurrHP) + "/" + FString::FromInt(MaxHP)));

	if (CurrHP == 0) {
		PokemonButton->SetBackgroundColor(FLinearColor(0.583, 0.051, 0.051, 1));
	}

	float HpPercent = float(CurrHP) / float(MaxHP);
	HPBar->SetPercent(HpPercent);
}

void UPokemonIconWidget::SetPokemon(int InPokemonID)
{
	PokemonID = InPokemonID;
}
