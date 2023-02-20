// Fill out your copyright notice in the Description page of Project Settings.


#include "PokemonStorageManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "../../Player/PlayerCharacterController.h"
#include "../../GameModes/OverworldGameMode.h"
#include "../../UI/HUD/OverworldHUD.h"

APokemonStorageManager::APokemonStorageManager()
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));

	Capsule->SetupAttachment(SkeletalMesh);
}

void APokemonStorageManager::Interact(APlayerController* Controller)
{
	AOverworldHUD* Hud = Cast<AOverworldHUD>(Controller->GetHUD());
	if (Hud == nullptr) {
		return;
	}

	Hud->ShowPokemonStorage();
}
