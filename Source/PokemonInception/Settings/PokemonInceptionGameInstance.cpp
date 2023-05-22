// Fill out your copyright notice in the Description page of Project Settings.

#include "PokemonInceptionGameInstance.h"
#include "Sound/SoundClass.h"


void UPokemonInceptionGameInstance::ChangeScalability(float NewValue)
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();
	if (!IsValid(UserSettings)) {
		GEngine->AddOnScreenDebugMessage(12, 1, FColor::Red, "ERROR!!!");
		return;
	}

	int GraphicsQualityValue = NewValue;
	GEngine->AddOnScreenDebugMessage(12, 1, FColor::Yellow, "Current Graphics Quality: " + FString::FromInt(UserSettings->GetOverallScalabilityLevel()));
	
	if (GraphicsQualityValue != UserSettings->GetOverallScalabilityLevel()) {
		UserSettings->SetOverallScalabilityLevel(GraphicsQualityValue);
		UserSettings->ApplyNonResolutionSettings();
		UserSettings->SaveSettings();
	}
}

void UPokemonInceptionGameInstance::ChangeSEVolume(float NewValue)
{
	if (!IsValid(SEClass)) {
		return;
	}

	GEngine->AddOnScreenDebugMessage(10, 1, FColor::Yellow, "SE Volume: " + FString::SanitizeFloat(NewValue));

	SEVolume = NewValue;
	SEClass->Properties.Volume = NewValue;
}

void UPokemonInceptionGameInstance::ChangePokemonCryVolume(float NewValue)
{
	if (!IsValid(PokemonCryClass)) {
		return;
	}

	GEngine->AddOnScreenDebugMessage(10, 1, FColor::Yellow, "PokemonCry Volume: " + FString::SanitizeFloat(NewValue));

	PokemonCryVolume = NewValue;
	PokemonCryClass->Properties.Volume = NewValue;
}

void UPokemonInceptionGameInstance::ChangeBGMVolume(float NewValue)
{
	if (!IsValid(BGMClass)) {
		return;
	}

	GEngine->AddOnScreenDebugMessage(10, 1, FColor::Yellow, "BGM Volume: " + FString::SanitizeFloat(NewValue));

	BGMVolume = NewValue;
	BGMClass->Properties.Volume = NewValue;
}

int UPokemonInceptionGameInstance::GetGraphicsQuality()
{
	UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();
	if (!IsValid(UserSettings)) {
		return -1;
	}

	return UserSettings->GetOverallScalabilityLevel();
}

float UPokemonInceptionGameInstance::GetSEVolume()
{
	return SEVolume;
}

float UPokemonInceptionGameInstance::GetPokemonCryVolume()
{
	return PokemonCryVolume;
}

float UPokemonInceptionGameInstance::GetBGMVolume()
{
	return BGMVolume;
}

