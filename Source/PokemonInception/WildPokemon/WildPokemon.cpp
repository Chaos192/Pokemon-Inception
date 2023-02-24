// Fill out your copyright notice in the Description page of Project Settings.


#include "WildPokemon.h"
#include "../Player/PokemonInceptionCharacter.h"
#include "../Player/PlayerCharacterController.h"
#include "../GameModes/OverworldGameMode.h"
#include "Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"


AWildPokemon::AWildPokemon()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AWildPokemon::Collide);
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AWildPokemon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	APokemonInceptionCharacter* Player = Cast<APokemonInceptionCharacter>(PlayerController->GetPawn());
	if (!IsValid(Player)) {
		return;
	}

	if (FVector::Dist(GetActorLocation(), Player->GetActorLocation()) > 3000) {
		Destroy();
	}
}

void AWildPokemon::InitPokemon(UDataTable* PokemonDatatable, int Level, TArray<UDataTable*> MoveTables)
{
	FPokemonBaseStruct* PokemonSpecies = PokemonDatatable->FindRow<FPokemonBaseStruct>(PokemonID, ""); //this right here, officer

	if (PokemonSpecies) {
		Pokemon.Init(Level, *PokemonSpecies);
		Pokemon.InitMoves(MoveTables);
	}
}

void AWildPokemon::Collide(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(GetWorld()->GetAuthGameMode());
	if (!IsValid(GameMode)) {
		return;
	}

	APokemonInceptionCharacter* Player = Cast<APokemonInceptionCharacter>(OtherActor);
	if (!IsValid(Player)) {
		return;
	}

	TArray<FPokemonStruct> Opponent;
	Opponent.Add(Pokemon);

	GameMode->SaveGame();
	GameMode->SaveLevelData(this);
	GameMode->InitiateBattle(Opponent, false);
}

