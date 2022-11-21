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
	int index = (FMath::RandHelper(PokemonToSpawn.Num()));
	Rotation.Yaw = (FMath::RandRange(0, 360));

	AWildPokemon* SpawnedPokemon = nullptr;
	SpawnedPokemon = GetWorld()->SpawnActor<AWildPokemon>(PokemonToSpawn[index], GetActorLocation(), Rotation, SpawnInfo);
}

