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
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(GetWorld()->GetAuthGameMode());
	if (!IsValid(GameMode)) {
		return;
	}

	AOverworldHUD* Hud = Cast<AOverworldHUD>(PlayerController->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	GameMode->InitItemsToSell(ItemsToSell);
	Hud->ShowBuyShop();
}
