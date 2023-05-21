// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../Pokemon/PokemonStruct.h"
#include "../Items/ItemBaseStruct.h"
#include "BattleController.generated.h"


UCLASS()
class POKEMONINCEPTION_API ABattleController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FItemBaseStruct> Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FPokemonBaseStruct> Pokedex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FPokemonStruct> PokemonParty;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FPokemonStruct> PokemonStorage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int Money;

	UPROPERTY(VisibleAnywhere)
	int TrainersDefeated;

	
	bool bIsPartyDefeated();
	bool bCanObtainMorePokemon();
	bool bIsRegisteredInPokedex(FName ID);

	int GetLeadPokemon();

	void ObtainPokemon(FPokemonStruct Pokemon);
	void RegisterToPokedex(FPokemonBaseStruct Species);
};
