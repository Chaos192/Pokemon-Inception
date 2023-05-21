// Fill out your copyright notice in the Description page of Project Settings.


#include "PokemonStorageManager.h"
#include "../../Player/PlayerCharacterController.h"
#include "../../GameModes/OverworldGameMode.h"
#include "../../UI/HUD/OverworldHUD.h"

APokemonStorageManager::APokemonStorageManager()
{
	Name = "Pokemon Storage Manager";
}

void APokemonStorageManager::Interact(APlayerController* PlayerController)
{
	AOverworldHUD* Hud = Cast<AOverworldHUD>(PlayerController->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	Hud->ShowPokemonStorage();
}
