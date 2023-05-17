// Fill out your copyright notice in the Description page of Project Settings.

#include "PokemonInceptionGameInstance.h"
#include "Sound/SoundClass.h"
#include "../SaveGame/UserSaveData.h"


void UPokemonInceptionGameInstance::ChangeScalability(float NewValue)
{
	/*UUserSaveData* UserSettings = Cast<UUserSaveData>(UGameUserSettings::GetGameUserSettings());
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
	}*/
}

void UPokemonInceptionGameInstance::ChangeSEVolume(float NewValue)
{
	/*UUserSaveData* UserSettings = Cast<UUserSaveData>(UGameUserSettings::GetGameUserSettings());
	if (!IsValid(UserSettings)) {
		return;
	}

	if (!IsValid(SEClass)) {
		return;
	}

	GEngine->AddOnScreenDebugMessage(10, 1, FColor::Yellow, "SE Volume: " + FString::SanitizeFloat(NewValue));

	UserSettings->SEVolume = NewValue;
	UserSettings->SaveSettings();
	SEClass->Properties.Volume = NewValue;*/
}

void UPokemonInceptionGameInstance::ChangePokemonCryVolume(float NewValue)
{
	/*UUserSaveData* UserSettings = Cast<UUserSaveData>(UGameUserSettings::GetGameUserSettings());
	if (!IsValid(UserSettings)) {
		return;
	}

	if (!IsValid(PokemonCryClass)) {
		return;
	}

	GEngine->AddOnScreenDebugMessage(10, 1, FColor::Yellow, "PokemonCry Volume: " + FString::SanitizeFloat(NewValue));

	UserSettings->PokemonCryVolume = NewValue;
	UserSettings->SaveSettings();
	PokemonCryClass->Properties.Volume = NewValue;*/
}

void UPokemonInceptionGameInstance::ChangeBGMVolume(float NewValue)
{
	UUserSaveData* UserSettings = Cast<UUserSaveData>(UGameUserSettings::GetGameUserSettings());
	if (!IsValid(UserSettings)) {
		return;
	}

	if (!IsValid(BGMClass)) {
		return;
	}

	GEngine->AddOnScreenDebugMessage(10, 1, FColor::Yellow, "BGM Volume: " + FString::SanitizeFloat(NewValue));

	UserSettings->BGMVolume = NewValue;
	UserSettings->SaveSettings();
	BGMClass->Properties.Volume = NewValue;
}

int UPokemonInceptionGameInstance::GetGraphicsQuality()
{
	/*UUserSaveData* UserSettings = Cast<UUserSaveData>(UGameUserSettings::GetGameUserSettings());
	if (!IsValid(UserSettings)) {
		return -1;
	}

	return UserSettings->GetOverallScalabilityLevel();*/
	return -1;
}

float UPokemonInceptionGameInstance::GetSEVolume()
{
	/*UUserSaveData* UserSettings = Cast<UUserSaveData>(UGameUserSettings::GetGameUserSettings());
	if (!IsValid(UserSettings)) {
		return -1;
	}

	return UserSettings->SEVolume;*/
	return -1;
}

float UPokemonInceptionGameInstance::GetPokemonCryVolume()
{
	/*UUserSaveData* UserSettings = Cast<UUserSaveData>(UGameUserSettings::GetGameUserSettings());
	if (!IsValid(UserSettings)) {
		return -1;
	}

	return UserSettings->PokemonCryVolume;*/
	return -1;
}

float UPokemonInceptionGameInstance::GetBGMVolume()
{
	UUserSaveData* UserSettings = Cast<UUserSaveData>(UGameUserSettings::GetGameUserSettings());
	if (!IsValid(UserSettings)) {
		return -1;
	}

	return UserSettings->BGMVolume;
	//return -1;
}

