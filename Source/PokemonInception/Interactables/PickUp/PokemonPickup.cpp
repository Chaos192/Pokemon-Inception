// Fill out your copyright notice in the Description page of Project Settings.


#include "PokemonPickup.h"
#include "Components/BoxComponent.h"
#include "../../Player/PlayerCharacterController.h"
#include "../../GameModes/OverworldGameMode.h"
#include "../../UI/HUD/OverworldHUD.h"

APokemonPickup::APokemonPickup()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(Mesh);

	Name = "Pokemon Pickup";
}

void APokemonPickup::Interact(APlayerController* Controller)
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(Controller);
	if (!IsValid(PlayerController)) {
		return;
	}

	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(GetWorld()->GetAuthGameMode());
	if (!IsValid(GameMode)) {
		return;
	}

	AOverworldHUD* Hud = Cast<AOverworldHUD>(PlayerController->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	if (PlayerController->bCanObtainMorePokemon() == false) {
		Hud->OnScreenMessage("You can't obtain more pokemon!");
		return;
	}

	FPokemonBaseStruct* PokemonSpecies = GameMode->PokemonDT->FindRow<FPokemonBaseStruct>(PokemonID, "");

	if (PokemonSpecies) {
		FPokemonStruct AddedPokemon;
		AddedPokemon.Init(PokemonLevel, *PokemonSpecies);
		AddedPokemon.InitMoves(GameMode->GetMoveDT());

		Hud->ShowPickupMessage("You got a " + PokemonSpecies->Name.ToString() + "!", PokemonSpecies->Image);
		PlayerController->ObtainPokemon(AddedPokemon);
		GameMode->MarkActorAsDestroyed(this);
		Destroy();
	}
}
