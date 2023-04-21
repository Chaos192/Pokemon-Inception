// Fill out your copyright notice in the Description page of Project Settings.

#include "PokemonInceptionGameInstance.h"
#include "Sound/SoundClass.h"


UPokemonInceptionGameInstance::UPokemonInceptionGameInstance()
{
	GameSettings = GEngine->GetGameUserSettings();
}

void UPokemonInceptionGameInstance::ChangeScalability(float NewValue)
{
	if (!IsValid(GameSettings)) {
		return;
	}

	GraphicsQualityValue = NewValue;
	GEngine->AddOnScreenDebugMessage(12, 1, FColor::Yellow, "Current Graphics Quality: " + FString::FromInt(GameSettings->GetOverallScalabilityLevel()));
	
	if (GraphicsQualityValue != GameSettings->GetTextureQuality()) {
		GameSettings->SetTextureQuality(GraphicsQualityValue);
		GameSettings->SaveSettings();
	}
}

void UPokemonInceptionGameInstance::ChangeSEVolume(float NewValue)
{
	if (!IsValid(SEClass)) {
		return;
	}

	SEVolume = NewValue;
	GEngine->AddOnScreenDebugMessage(10, 1, FColor::Yellow, "SE Volume: " + FString::SanitizeFloat(SEVolume));

	SEClass->Properties.Volume = SEVolume;
}

void UPokemonInceptionGameInstance::ChangeBGMVolume(float NewValue)
{
	if (!IsValid(BGMClass)) {
		return;
	}

	BGMVolume = NewValue;
	GEngine->AddOnScreenDebugMessage(10, 1, FColor::Yellow, "BGM Volume: " + FString::SanitizeFloat(BGMVolume));

	BGMClass->Properties.Volume = BGMVolume;
}

int UPokemonInceptionGameInstance::GetGraphicsQuality()
{
	return GraphicsQualityValue;
}

float UPokemonInceptionGameInstance::GetSEVolume()
{
	return SEVolume;
}

float UPokemonInceptionGameInstance::GetBGMVolume()
{
	return BGMVolume;
}

