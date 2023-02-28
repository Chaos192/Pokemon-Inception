// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../SaveGame/WildPokemonSaveData.h"
#include "WildPokemonSpawner.generated.h"

UCLASS()
class POKEMONINCEPTION_API AWildPokemonSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AWildPokemonSpawner();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void ClearPokemonReference(AActor* Pokemon);

	void Generate();

	void ManualGenerate(FWildPokemonData SaveData);

	class AWildPokemon* SpawnedPokemon = nullptr;

protected:
	UPROPERTY(EditAnywhere)
	int MinLevel = 1;

	UPROPERTY(EditAnywhere)
	int MaxLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<class AWildPokemon>> PokemonToSpawn;
};
