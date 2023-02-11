// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WildPokemon.h"
#include "WildPokemonSpawner.generated.h"

UCLASS()
class POKEMONINCEPTION_API AWildPokemonSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AWildPokemonSpawner();

	virtual void Tick(float DeltaTime) override;

	void Generate();

protected:
	virtual void BeginPlay() override;

	AWildPokemon* SpawnedPokemon = nullptr;

	UPROPERTY(EditAnywhere)
	int MinLevel = 1;

	UPROPERTY(EditAnywhere)
	int MaxLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<AWildPokemon>> PokemonToSpawn;

	FRotator Rotation;
	FActorSpawnParameters SpawnInfo;
	
};
