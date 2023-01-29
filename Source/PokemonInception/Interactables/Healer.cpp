// Fill out your copyright notice in the Description page of Project Settings.


#include "Healer.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "../Player/PlayerCharacterController.h"
#include "../GameModes/OverworldGameMode.h"
#include "../UI/OverworldUI/OverworldHUD.h"

AHealer::AHealer()
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));

	Capsule->SetupAttachment(SkeletalMesh);
}

void AHealer::Interact(APlayerController* Controller)
{
	AOverworldHUD* Hud = Cast<AOverworldHUD>(Controller->GetHUD());
	if (Hud == nullptr) {
		return;
	}

	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(Controller);

	PlayerController->FullRestoreAllPokemon();
	Hud->OnScreenMessage("Your Pokemon were fully healed!");
}
