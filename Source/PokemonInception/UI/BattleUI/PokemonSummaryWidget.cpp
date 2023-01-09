// Fill out your copyright notice in the Description page of Project Settings.


#include "PokemonSummaryWidget.h"

void UPokemonSummaryWidget::SetImage(UTexture2D* Image)
{
	PokemonImage->SetBrushFromTexture(Image);
}

void UPokemonSummaryWidget::SetGeneralInfo(FText Name, FName Id, FString Type, int Level, int RequiredEXP)
{
	PokemonName->SetText(Name);
	PokemonID->SetText(FText::FromName(Id));
	PokemonType->SetText(FText::FromString(Type));
	PokemonLevel->SetText(FText::FromString(FString::FromInt(Level)));
	EXPRequired->SetText(FText::FromString(FString::FromInt(RequiredEXP)));
}

void UPokemonSummaryWidget::SetStats(FString HP, int Attack, int Defence, int Speed)
{;
	PokemonHP->SetText(FText::FromString(HP));
	PokemonAttack->SetText(FText::FromString(FString::FromInt(Attack)));
	PokemonDefence->SetText(FText::FromString(FString::FromInt(Defence)));
	PokemonSpeed->SetText(FText::FromString(FString::FromInt(Speed)));
}

void UPokemonSummaryWidget::AddMove(UMoveButtonWidget* Move)
{
	MoveWrapBox->AddChildToWrapBox(Move);
}
