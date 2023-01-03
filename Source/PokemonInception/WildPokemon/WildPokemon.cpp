// Fill out your copyright notice in the Description page of Project Settings.


#include "WildPokemon.h"
#include "../Player/PokemonInceptionCharacter.h"
#include "../GameModes/OverworldGameMode.h"
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
}

void AWildPokemon::InitPokemon(UDataTable* PokemonDatatable, int Level)
{
	FPokemonBaseStruct* PokemonSpecies = PokemonDatatable->FindRow<FPokemonBaseStruct>(PokemonID, "");
	Pokemon.Init(Level, *PokemonSpecies);
}


void AWildPokemon::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr) {
		return;
	}

	APokemonInceptionCharacter* Player = Cast<APokemonInceptionCharacter>(OtherActor);
	if (IsValid(Player)) {
		GameMode->SaveGame();
		GameMode->SaveOpponent(Pokemon);
		UGameplayStatics::OpenLevel(GetWorld(), FName("BattleMap"));
	}
}

