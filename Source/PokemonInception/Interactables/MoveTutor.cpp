// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveTutor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "../GameModes/OverworldGameMode.h"
#include "../UI/OverworldUI/OverworldHUD.h"

AMoveTutor::AMoveTutor()
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));

	Capsule->SetupAttachment(SkeletalMesh);
}

void AMoveTutor::Interact(APlayerController* Controller)
{
	AOverworldHUD* Hud = Cast<AOverworldHUD>(Controller->GetHUD());
	if (Hud == nullptr) {
		return;
	}

	Hud->ShowMoveManager(0);
}
