// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveTutor.h"
#include "../../GameModes/OverworldGameMode.h"
#include "../../UI/HUD/OverworldHUD.h"

AMoveTutor::AMoveTutor()
{
	Name = "Move Tutor";
}

void AMoveTutor::Interact(APlayerController* PlayerController)
{
	AOverworldHUD* Hud = Cast<AOverworldHUD>(PlayerController->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	Hud->ShowMoveManager(0);
}
