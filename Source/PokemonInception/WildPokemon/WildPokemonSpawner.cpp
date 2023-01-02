// Fill out your copyright notice in the Description page of Project Settings.


#include "WildPokemonSpawner.h"


AWildPokemonSpawner::AWildPokemonSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWildPokemonSpawner::BeginPlay()
{
	Super::BeginPlay();
	Generate();
	GetWorldTimerManager().SetTimer(SpawnHandle, this, &AWildPokemonSpawner::Generate, SpawnTime, true);
}

void AWildPokemonSpawner::Generate()
{
	int index = (FMath::RandHelper(PokemonToSpawn.Num()));
	int SpawnLevel = (FMath::RandRange(MinLevel, MaxLevel));
	Rotation.Yaw = (FMath::RandRange(0, 360));

	AWildPokemon* SpawnedPokemon = nullptr;
	SpawnedPokemon = GetWorld()->SpawnActor<AWildPokemon>(PokemonToSpawn[index], GetActorLocation(), Rotation, SpawnInfo);
	SpawnedPokemon->InitPokemon(SpawnLevel);
}

