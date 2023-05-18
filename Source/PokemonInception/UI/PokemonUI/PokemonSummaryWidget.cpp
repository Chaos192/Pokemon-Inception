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

void UPokemonSummaryWidget::SetStats(FString HP, int Attack, int Defence, int Speed, TArray<EEffect> Effects)
{;
	PokemonHP->SetText(FText::FromString(HP));

	
	if (Effects.Contains(EEffect::AttackUp)) {
			PokemonAttack->SetText(FText::FromString(FString::FromInt(Attack * 1.5)));
			PokemonAttack->SetColorAndOpacity(FLinearColor(1, 0, 0, 1));
	}
	else if (Effects.Contains(EEffect::AttackDown)) {
		PokemonAttack->SetText(FText::FromString(FString::FromInt(Attack * 0.5)));
		PokemonAttack->SetColorAndOpacity(FLinearColor(0, 0.1, 0.6, 1));
	}
	else {
		PokemonAttack->SetText(FText::FromString(FString::FromInt(Attack)));
		PokemonAttack->SetColorAndOpacity(FLinearColor(1, 1, 1, 1));
	}

	if (Effects.Contains(EEffect::DefenceUp)) {
		PokemonDefence->SetText(FText::FromString(FString::FromInt(Defence * 1.5)));
		PokemonDefence->SetColorAndOpacity(FLinearColor(1, 0, 0, 1));
	}
	else if (Effects.Contains(EEffect::DefenceDown)) {
		PokemonDefence->SetText(FText::FromString(FString::FromInt(Defence * 0.5)));
		PokemonDefence->SetColorAndOpacity(FLinearColor(0, 0.1, 0.6, 1));
	}
	else {
		PokemonDefence->SetText(FText::FromString(FString::FromInt(Defence)));
		PokemonDefence->SetColorAndOpacity(FLinearColor(1, 1, 1, 1));
	}

	if (Effects.Contains(EEffect::SpeedUp)) {
		PokemonSpeed->SetText(FText::FromString(FString::FromInt(Speed * 1.5)));
		PokemonSpeed->SetColorAndOpacity(FLinearColor(1, 0, 0, 1));
	}
	else if (Effects.Contains(EEffect::SpeedDown)) {
		PokemonSpeed->SetText(FText::FromString(FString::FromInt(Speed * 0.5)));
		PokemonSpeed->SetColorAndOpacity(FLinearColor(0, 0.1, 0.6, 1));
	}
	else {
		PokemonSpeed->SetText(FText::FromString(FString::FromInt(Speed)));
		PokemonSpeed->SetColorAndOpacity(FLinearColor(1, 1, 1, 1));
	}
}

void UPokemonSummaryWidget::AddMove(UMoveButtonWidget* Move)
{
	MoveWrapBox->AddChildToWrapBox(Move);
}

void UPokemonSummaryWidget::ClearMoves()
{
	if (MoveWrapBox->HasAnyChildren()) {
		MoveWrapBox->ClearChildren();
	}
}
