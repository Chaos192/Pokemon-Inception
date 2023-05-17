// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "UserSaveData.generated.h"


UCLASS()
class POKEMONINCEPTION_API UUserSaveData : public UGameUserSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float SEVolume = 0.8f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float BGMVolume = 0.8f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float PokemonCryVolume = 0.8f;
};
