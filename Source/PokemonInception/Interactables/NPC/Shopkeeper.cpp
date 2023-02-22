// Fill out your copyright notice in the Description page of Project Settings.


#include "Shopkeeper.h"
#include "../../Player/PlayerCharacterController.h"
#include "../../GameModes/OverworldGameMode.h"
#include "../../UI/HUD/OverworldHUD.h"

AShopkeeper::AShopkeeper()
{
	Name = "Shopkeeper";
}

void AShopkeeper::Interact(APlayerController* PlayerController)
{
	AOverworldHUD* Hud = Cast<AOverworldHUD>(PlayerController->GetHUD());
	if (Hud != nullptr) {
		Hud->ShowShop(ItemsToSell);
	}
}
