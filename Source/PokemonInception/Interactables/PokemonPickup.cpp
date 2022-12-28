// Fill out your copyright notice in the Description page of Project Settings.


#include "PokemonPickup.h"
#include "../Player/PlayerCharacterController.h"
#include "../GameModes/OverworldGameMode.h"
#include "../UI/OverworldUI/OverworldHUD.h"

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

	AOverworldHUD* Hud = Cast<AOverworldHUD>(PlayerController->GetHUD());
	if (Hud == nullptr) {
		return;
	}

	FPokemonBaseStruct* PokemonSpecies = PokemonDatatable->FindRow<FPokemonBaseStruct>(PokemonID, "");
	FPokemonStruct AddedPokemon;
	AddedPokemon.Init(PokemonLevel, *PokemonSpecies);

	Hud->OnScreenMessage("You got a " + PokemonSpecies->Name.ToString() + "!");
	PlayerController->ObtainPokemon(AddedPokemon);
	Destroy();
}
