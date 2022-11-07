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
	// Sets default values for this actor's properties
	AWildPokemonSpawner();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float SpawnTime = 10;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class APokemon00_Base>> SpawnablePokemon;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWildPokemon> PokemonToSpawn;

	FTimerHandle SpawnHandle;

	FRotator Rotation;
	FActorSpawnParameters SpawnInfo;

public:	
	UFUNCTION()
	void Generate();
};
