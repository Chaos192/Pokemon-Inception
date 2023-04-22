// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../SaveGame/LevelSaveData.h"
#include "WildPokemonSpawner.generated.h"

UCLASS()
class POKEMONINCEPTION_API AWildPokemonSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AWildPokemonSpawner();

	UPROPERTY()
	class AWildPokemon* SpawnedPokemon = nullptr;

	virtual void Tick(float DeltaTime) override;
	virtual void Generate();

	void ManualGenerate(FWildPokemonData SaveData);

	UFUNCTION()
	void ClearPokemonReference(AActor* Pokemon);

protected:
	UPROPERTY(EditAnywhere)
	int MinLevel = 1;

	UPROPERTY(EditAnywhere)
	int MaxLevel = 1;

private:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class AWildPokemon>> PokemonToSpawn;
};
