// Fill out your copyright notice in the Description page of Project Settings.


#include "Guide.h"
#include "../../UI/HUD/OverworldHUD.h"

AGuide::AGuide()
{
	Name = "Guide";
}

void AGuide::Interact(APlayerController* PlayerController)
{
	AOverworldHUD* Hud = Cast<AOverworldHUD>(PlayerController->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	Hud->ShowGuideMenu();
}
