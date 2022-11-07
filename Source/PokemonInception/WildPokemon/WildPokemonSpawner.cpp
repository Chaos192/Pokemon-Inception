// Fill out your copyright notice in the Description page of Project Settings.


#include "WildPokemonSpawner.h"


AWildPokemonSpawner::AWildPokemonSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWildPokemonSpawner::BeginPlay()
{
	Super::BeginPlay();
	Generate();
	GetWorldTimerManager().SetTimer(SpawnHandle, this, &AWildPokemonSpawner::Generate, SpawnTime, true);
}

void AWildPokemonSpawner::Generate()
{
	AWildPokemon* SpawnedPokemon = nullptr;
	SpawnedPokemon = GetWorld()->SpawnActor<AWildPokemon>(PokemonToSpawn, GetActorLocation(), Rotation, SpawnInfo);

	int index = (FMath::RandHelper(SpawnablePokemon.Num()));
	SpawnedPokemon->Init(SpawnablePokemon[index]);

}

