// Fill out your copyright notice in the Description page of Project Settings.


#include "PokemonPickup.h"
#include "../Player/PlayerCharacterController.h"
#include "../GameModes/OverworldGameMode.h"
#include "../UI/HUD/OverworldHUD.h"

APokemonPickup::APokemonPickup()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	Action = "pick up";
}

void APokemonPickup::Interact(APlayerController* Controller)
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(Controller);
	if (PlayerController == nullptr) {
		return;
	}

	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr) {
		return;
	}

	AOverworldHUD* Hud = Cast<AOverworldHUD>(PlayerController->GetHUD());
	if (Hud == nullptr) {
		return;
	}

	if (PlayerController->bCanObtainMorePokemon() == false) {
		Hud->OnScreenMessage("You can't obtain more pokemon!");
		return;
	}

	FPokemonBaseStruct* PokemonSpecies = PokemonDatatable->FindRow<FPokemonBaseStruct>(PokemonID, "");
	FPokemonStruct AddedPokemon;
	AddedPokemon.Init(PokemonLevel, *PokemonSpecies);
	AddedPokemon.InitMoves(GameMode->GetMoveDT());

	Hud->OnScreenMessage("You got a " + PokemonSpecies->Name.ToString() + "!");
	PlayerController->ObtainPokemon(AddedPokemon);
	GameMode->MarkActorAsDestroyed(this);
	Destroy();
}
