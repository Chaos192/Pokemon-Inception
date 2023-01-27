// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "../WildPokemon/WildPokemon.h"
#include "../Items/ItemBaseStruct.h"
#include "../Pokemon/PokemonStruct.h"
#include "WorldSaveData.generated.h"

USTRUCT()
struct FGameMapData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FVector PlayerLocation;

	UPROPERTY(EditDefaultsOnly)
	FRotator PlayerRotation;

	UPROPERTY(EditDefaultsOnly)
	TArray<AActor*> ActorsToDestroy;
};

UCLASS()
class POKEMONINCEPTION_API UWorldSaveData : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FGameMapData GameMapData;

	UPROPERTY(EditDefaultsOnly)
	TArray<FItemBaseStruct> InventoryData;

	UPROPERTY(EditDefaultsOnly)
	TArray<FPokemonBaseStruct> PokedexData;

	UPROPERTY(EditDefaultsOnly)
	int MoneyData;

	UPROPERTY(EditDefaultsOnly)
	TArray<FPokemonStruct> OpponentData;

	UPROPERTY(EditDefaultsOnly)
	TArray<FPokemonStruct> PartyData;

	UPROPERTY(EditDefaultsOnly)
	TArray<FPokemonStruct> StorageData;

};
