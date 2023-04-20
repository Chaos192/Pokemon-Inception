// Fill out your copyright notice in the Description page of Project Settings.


#include "PokemonInceptionGameInstance.h"

void UPokemonInceptionGameInstance::ChangeScalability(float NewValue)
{
	int Value = NewValue;

	if (Value != GameSettings->GetOverallScalabilityLevel()) {
		GameSettings->SetOverallScalabilityLevel(Value);
	}
}
