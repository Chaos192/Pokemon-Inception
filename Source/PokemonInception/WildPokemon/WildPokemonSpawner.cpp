// Fill out your copyright notice in the Description page of Project Settings.


#include "WildPokemonSpawner.h"
#include "../Player/PokemonInceptionCharacter.h"
#include "../Player/PlayerCharacterController.h"
#include "../GameModes/OverworldGameMode.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


AWildPokemonSpawner::AWildPokemonSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWildPokemonSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController == nullptr) {
		return;
	}

	APokemonInceptionCharacter* Player = Cast<APokemonInceptionCharacter>(PlayerController->GetPawn());
	if (Player == nullptr) {
		return;
	}


	if (!bIsSpawnedPokemonInWorld && FVector::Dist(GetActorLocation(), Player->GetActorLocation()) < 50) {
		Generate();
	}
}

void AWildPokemonSpawner::BeginPlay()
{
	Super::BeginPlay();
}

void AWildPokemonSpawner::Generate()
{
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr) {
		return;
	}

	int index = (FMath::RandHelper(PokemonToSpawn.Num()));
	int SpawnLevel = (FMath::RandRange(MinLevel, MaxLevel));

	Rotation.Yaw = (FMath::RandRange(0, 360));

	AWildPokemon* SpawnedPokemon = nullptr;
	SpawnedPokemon = GetWorld()->SpawnActor<AWildPokemon>(PokemonToSpawn[index], GetActorLocation(), Rotation, SpawnInfo);
	SpawnedPokemon->InitPokemon(PokemonDatatable, SpawnLevel, GameMode->GetMoveDT());
	SpawnedPokemon->SpawnerRef = this;

	bIsSpawnedPokemonInWorld = true;
}

void AWildPokemonSpawner::OnSpawnedPokemonDestroyed()
{
	bIsSpawnedPokemonInWorld = false;
}

UDataTable* AWildPokemonSpawner::GetPokemonTable()
{
	return PokemonDatatable;
}

