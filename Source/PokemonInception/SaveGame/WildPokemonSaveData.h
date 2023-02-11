// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "../WildPokemon/WildPokemon.h"
#include "../Pokemon/PokemonStruct.h"
#include "WildPokemonSaveData.generated.h"

USTRUCT()
struct FWildPokemonData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	AWildPokemon* Pokemon;

	UPROPERTY(EditDefaultsOnly)
	FPokemonStruct PokemonStruct;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWildPokemon> PokemonClass;

	UPROPERTY(EditDefaultsOnly)
	FVector PokemonLocation;

	UPROPERTY(EditDefaultsOnly)
	FRotator PokemonRotation;
};

UCLASS()
class POKEMONINCEPTION_API UWildPokemonSaveData : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TMap<class AWildPokemonSpawner*, FWildPokemonData> PokemonSpawners;
	
};
