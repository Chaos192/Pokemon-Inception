// Fill out your copyright notice in the Description page of Project Settings.


#include "MoneyPickup.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "../../Player/PlayerCharacterController.h"
#include "../../GameModes/OverworldGameMode.h"
#include "../../UI/HUD/OverworldHUD.h"

AMoneyPickup::AMoneyPickup()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(Mesh);

	Name = "Money Pickup";
	Money = 100;
}

void AMoneyPickup::Interact(APlayerController* Controller)
{
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(GetWorld()->GetAuthGameMode());
	if (!IsValid(GameMode)) {
		return;
	}

	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(Controller);
	if (!IsValid(PlayerController)) {
		return;
	}
	
	AOverworldHUD* Hud = Cast<AOverworldHUD>(PlayerController->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}
	
	if ((PlayerController->Money + Money) > 1000000) {
		Hud->OnScreenMessage("You can't get more money!");
	}

	Hud->ShowPickupMessage("You got " + FString::FromInt(Money) + "$!", nullptr);
	PlayerController->Money += Money;
	GameMode->MarkActorAsDestroyed(this);
	Destroy();
}
