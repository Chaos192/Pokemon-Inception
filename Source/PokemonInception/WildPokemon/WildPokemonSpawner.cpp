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

	//GEngine->AddOnScreenDebugMessage(2, 1, FColor::Red, FString::SanitizeFloat(FVector::Dist(GetActorLocation(), Player->GetActorLocation())));
	if (!IsValid(SpawnedPokemon) && FVector::Dist(GetActorLocation(), Player->GetActorLocation()) < 1500) {
		Generate();
		//GEngine->AddOnScreenDebugMessage(1, 1, FColor::Green, TEXT("Can spawn Pokemon"));
	}
	else {
		//GEngine->AddOnScreenDebugMessage(1, 1, FColor::Red, TEXT("Can't spawn Pokemon"));
	}
}

void AWildPokemonSpawner::Generate()
{
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr) {
		return;
	}

	int index = (FMath::RandHelper(PokemonToSpawn.Num()));
	int SpawnLevel = (FMath::RandRange(MinLevel, MaxLevel));

	FRotator Rotation;
	Rotation.Yaw = (FMath::RandRange(0, 360));

	SpawnedPokemon = GetWorld()->SpawnActor<AWildPokemon>(PokemonToSpawn[index], GetActorLocation(), Rotation);
	SpawnedPokemon->InitPokemon(GameMode->PokemonDT, SpawnLevel, GameMode->GetMoveDT());
}

void AWildPokemonSpawner::ManualGenerate(FWildPokemonData SaveData)
{
	if (IsValid(SpawnedPokemon)) {
		SpawnedPokemon->Destroy();
	}

	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr) {
		return;
	}

	SpawnedPokemon = GetWorld()->SpawnActor<AWildPokemon>(SaveData.PokemonClass, SaveData.PokemonLocation, SaveData.PokemonRotation);
	SpawnedPokemon->Pokemon = SaveData.PokemonStruct;
}

