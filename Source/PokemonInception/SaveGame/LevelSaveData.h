// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "../WildPokemon/WildPokemon.h"
#include "../Pokemon/PokemonStruct.h"
#include "../Interactables/NPC/Trainer.h"
#include "LevelSaveData.generated.h"

USTRUCT()
struct FWildPokemonData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int PokemonLevel = 0;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWildPokemon> PokemonClass;

	UPROPERTY(EditDefaultsOnly)
	FVector PokemonLocation = FVector();

	UPROPERTY(EditDefaultsOnly)
	FRotator PokemonRotation = FRotator();
};

UCLASS()
class POKEMONINCEPTION_API ULevelSaveData : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TMap<class AWildPokemonSpawner*, FWildPokemonData> PokemonSpawners;
	
	UPROPERTY(EditDefaultsOnly)
	FVector PlayerLocation;

	UPROPERTY(EditDefaultsOnly)
	FRotator PlayerRotation;

	UPROPERTY(EditDefaultsOnly)
	TArray<FName> PickUpsToDestroy;
};
