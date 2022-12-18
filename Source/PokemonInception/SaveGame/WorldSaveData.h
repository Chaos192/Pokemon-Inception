// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "../WildPokemon/WildPokemon.h"
#include "../Items/ItemBaseStruct.h"
#include "WorldSaveData.generated.h"


UCLASS()
class POKEMONINCEPTION_API UWorldSaveData : public USaveGame
{
	GENERATED_BODY()

public:
	/*
	UPROPERTY(EditDefaultsOnly)
	TArray<AWildPokemon> SpawnedPokemon;
	*/
	UPROPERTY(EditDefaultsOnly)
	FVector PlayerLocation;

	UPROPERTY(EditDefaultsOnly)
	TArray<FItemBaseStruct> InventoryData;

	UPROPERTY(EditDefaultsOnly)
	int MoneyData;
};
