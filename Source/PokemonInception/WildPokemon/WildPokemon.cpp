// Fill out your copyright notice in the Description page of Project Settings.


#include "WildPokemon.h"
#include "../Player/PokemonInceptionCharacter.h"
#include "../Player/PlayerCharacterController.h"
#include "../GameModes/OverworldGameMode.h"
#include "WildPokemonSpawner.h"
#include "Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"


AWildPokemon::AWildPokemon()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AWildPokemon::OnBeginOverlap);
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	//NotificationDelegate.AddDynamic(SpawnerRef, &AWildPokemonSpawner::OnSpawnedPokemonDestroyed);
}

void AWildPokemon::Tick(float DeltaTime)
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

	if (FVector::Dist(GetActorLocation(), Player->GetActorLocation()) > 100) {
		//NotificationDelegate.Broadcast();
		Destroy();
	}
}

void AWildPokemon::InitPokemon(UDataTable* PokemonDatatable, int Level, TArray<UDataTable*> MoveTables)
{
	FPokemonBaseStruct* PokemonSpecies = PokemonDatatable->FindRow<FPokemonBaseStruct>(PokemonID, "");
	Pokemon.Init(Level, *PokemonSpecies);
	Pokemon.InitMoves(MoveTables);
}

void AWildPokemon::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr) {
		return;
	}

	APokemonInceptionCharacter* Player = Cast<APokemonInceptionCharacter>(OtherActor);
	if (IsValid(Player)) {
		//NotificationDelegate.Broadcast();
		GameMode->SaveGame();
		GameMode->SaveOpponent(Pokemon);
		UGameplayStatics::OpenLevel(GetWorld(), FName("BattleMap"));
		Destroy();
	}
}

