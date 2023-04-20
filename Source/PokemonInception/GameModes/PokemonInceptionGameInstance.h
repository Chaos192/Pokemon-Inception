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
	UGameUserSettings* GameSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int GraphicsQualityValue = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SEVolume = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BGMVolume = 1.0f;

public:
	UFUNCTION()
	void ChangeScalability(float NewValue);
	
};
