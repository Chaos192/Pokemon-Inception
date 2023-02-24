// Fill out your copyright notice in the Description page of Project Settings.


#include "WildPokemonSpawner.h"
#include "WildPokemon.h"
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
	Generate();
}

void AWildPokemonSpawner::Generate()
{
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(GetWorld()->GetAuthGameMode());
	if (!IsValid(GameMode)) {
		return;
	}

	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	APokemonInceptionCharacter* Player = Cast<APokemonInceptionCharacter>(PlayerController->GetPawn());
	if (!IsValid(Player)) {
		return;
	}

	if (IsValid(SpawnedPokemon)) {
		return;
	}

	if (FVector::Dist(GetActorLocation(), Player->GetActorLocation()) > 1500 || FVector::Dist(GetActorLocation(), Player->GetActorLocation()) < 1000) {
		return;
	}

	int Index = (FMath::RandHelper(PokemonToSpawn.Num()));
	int SpawnLevel = (FMath::RandRange(MinLevel, MaxLevel));

	FRotator Rotation;
	Rotation.Yaw = (FMath::RandRange(0, 360));

	if (!IsValid(PokemonToSpawn[Index])) {
		return;
	}

	SpawnedPokemon = GetWorld()->SpawnActor<AWildPokemon>(PokemonToSpawn[Index], GetActorLocation(), Rotation);

	if (IsValid(SpawnedPokemon)) {
		SpawnedPokemon->InitPokemon(GameMode->PokemonDT, SpawnLevel, GameMode->GetMoveDT()); //this right here, officer
	}
}

void AWildPokemonSpawner::ManualGenerate(FWildPokemonData SaveData)
{
	if (IsValid(SpawnedPokemon)) {
		SpawnedPokemon->Destroy();
	}

	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(GetWorld()->GetAuthGameMode());
	if (!IsValid(GameMode)) {
		return;
	}

	SpawnedPokemon = GetWorld()->SpawnActor<AWildPokemon>(SaveData.PokemonClass, SaveData.PokemonLocation, SaveData.PokemonRotation);
	SpawnedPokemon->Pokemon = SaveData.PokemonStruct;
}

