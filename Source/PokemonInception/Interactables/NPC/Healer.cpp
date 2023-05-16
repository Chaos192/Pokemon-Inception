// Fill out your copyright notice in the Description page of Project Settings.


#include "Healer.h"
#include "../../Player/PlayerCharacterController.h"
#include "../../GameModes/OverworldGameMode.h"
#include "../../UI/HUD/OverworldHUD.h"

AHealer::AHealer()
{
	Name = "Healer";
}

void AHealer::Interact(APlayerController* PlayerController)
{
	AOverworldHUD* Hud = Cast<AOverworldHUD>(PlayerController->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	APlayerCharacterController* ThePlayerController = Cast<APlayerCharacterController>(PlayerController);

	ThePlayerController->FullRestoreAllPokemon();
	Hud->OnScreenMessage("Your Pokemon were fully healed!");
}
