// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "../WildPokemon/WildPokemon.h"
#include "WorldSaveData.generated.h"


UCLASS()
class POKEMONINCEPTION_API UWorldSaveData : public USaveGame
{
	GENERATED_BODY()

public:
	TArray<AWildPokemon> SpawnedPokemon;
};
