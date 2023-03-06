// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "../WildPokemon/WildPokemon.h"
#include "../Items/ItemBaseStruct.h"
#include "../Pokemon/PokemonStruct.h"
#include "PlayerSaveData.generated.h"


UCLASS()
class POKEMONINCEPTION_API UPlayerSaveData : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FItemBaseStruct> InventoryData;

	UPROPERTY(EditDefaultsOnly)
	TArray<FPokemonBaseStruct> PokedexData;

	UPROPERTY(EditDefaultsOnly)
	int MoneyData;

	UPROPERTY(EditDefaultsOnly)
	TArray<FPokemonStruct> OpponentData;

	UPROPERTY(EditDefaultsOnly)
	bool bIsOpponentTrainer;

	UPROPERTY(EditDefaultsOnly)
	TArray<FPokemonStruct> PartyData;

	UPROPERTY(EditDefaultsOnly)
	TArray<FPokemonStruct> StorageData;

};
