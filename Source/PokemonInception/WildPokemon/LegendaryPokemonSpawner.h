// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WildPokemonSpawner.h"
#include "LegendaryPokemonSpawner.generated.h"


UCLASS()
class POKEMONINCEPTION_API ALegendaryPokemonSpawner : public AWildPokemonSpawner
{
	GENERATED_BODY()
	
public:
	virtual void Generate() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWildPokemon> LegendaryPokemonToSpawn;

	UPROPERTY(EditAnywhere)
	FName LegendaryPokemonID;

	UPROPERTY(EditAnywhere)
	int DefeatedTrainersRequired = 0;

};
