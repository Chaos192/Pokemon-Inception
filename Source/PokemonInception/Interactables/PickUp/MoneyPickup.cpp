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
	money = 100;
}

void AMoneyPickup::Interact(APlayerController* Controller)
{
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr) {
		return;
	}

	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(Controller);
	if (PlayerController == nullptr) {
		return;
	}
	
	AOverworldHUD* Hud = Cast<AOverworldHUD>(PlayerController->GetHUD());
	if (Hud == nullptr) {
		return;
	}
	
	if ((PlayerController->Money + money) > 1000000) {
		Hud->OnScreenMessage("You can't get more money!");
	}

	Hud->ShowPickupMessage("You got " + FString::FromInt(money) + "$!", nullptr);
	PlayerController->Money += money;
	GameMode->MarkActorAsDestroyed(this);
	Destroy();
}
