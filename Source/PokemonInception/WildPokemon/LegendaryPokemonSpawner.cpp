// Fill out your copyright notice in the Description page of Project Settings.


#include "LegendaryPokemonSpawner.h"
#include "WildPokemon.h"
#include "../Player/PokemonInceptionCharacter.h"
#include "../Player/PlayerCharacterController.h"
#include "../GameModes/OverworldGameMode.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


void ALegendaryPokemonSpawner::Generate()
{
	Super::Generate();
	if (IsValid(SpawnedPokemon)) {
		return;
	}

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

	if (FVector::Dist(GetActorLocation(), Player->GetActorLocation()) > 1500 || FVector::Dist(GetActorLocation(), Player->GetActorLocation()) < 1000) {
		return;
	}

	if (PlayerController->TrainersDefeated < DefeatedTrainersRequired) {
		return;
	}

	if (PlayerController->bIsRegisteredInPokedex(LegendaryPokemonID)) {
		return;
	}

	FRotator Rotation;
	Rotation.Yaw = (FMath::RandRange(0, 360));

	SpawnedPokemon = GetWorld()->SpawnActor<AWildPokemon>(LegendaryPokemonToSpawn, GetActorLocation(), Rotation);

	if (IsValid(SpawnedPokemon)) {
		int SpawnLevel = (FMath::RandRange(MinLevel, MaxLevel));
		SpawnedPokemon->InitPokemon(GameMode->PokemonDT, SpawnLevel, GameMode->GetMoveDT());
		SpawnedPokemon->OnDestroyed.AddDynamic(this, &AWildPokemonSpawner::ClearPokemonReference);

		GameMode->PokemonInLevel.Add(SpawnedPokemon);
	}
}
