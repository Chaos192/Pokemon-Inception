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

	UFUNCTION()
	void Generate();

	UFUNCTION()
	void OnSpawnedPokemonDestroyed();

	UDataTable* GetPokemonTable();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float SpawnTime = 10;

	UPROPERTY(EditAnywhere)
	int MinLevel = 1;

	UPROPERTY(EditAnywhere)
	int MaxLevel = 1;

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* PokemonDatatable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<AWildPokemon>> PokemonToSpawn;

	FRotator Rotation;
	FActorSpawnParameters SpawnInfo;

	bool bIsSpawnedPokemonInWorld;
	
};
