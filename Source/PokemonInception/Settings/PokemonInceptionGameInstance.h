// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameFramework/GameUserSettings.h"
#include "PokemonInceptionGameInstance.generated.h"


UCLASS()
class POKEMONINCEPTION_API UPokemonInceptionGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USoundClass* SEClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USoundClass* PokemonCryClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USoundClass* BGMClass;

public:
	UFUNCTION()
	void ChangeScalability(float NewValue);

	UFUNCTION()
	void ChangeSEVolume(float NewValue);

	UFUNCTION()
	void ChangePokemonCryVolume(float NewValue);

	UFUNCTION()
	void ChangeBGMVolume(float NewValue);

	UFUNCTION()
	int GetGraphicsQuality();

	UFUNCTION()
	float GetSEVolume();

	UFUNCTION()
	float GetPokemonCryVolume();

	UFUNCTION()
	float GetBGMVolume();
};
