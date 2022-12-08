// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemPickup.h"
#include "Components/StaticMeshComponent.h"
#include "../Player/PlayerCharacterController.h"

AItemPickup::AItemPickup()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	Action = "pick up";
	ItemID = FName("None");
}

void AItemPickup::Interact(APlayerController* Controller)
{
	APlayerCharacterController* cont = Cast<APlayerCharacterController>(Controller);
	if (cont != nullptr) {
		cont->ObtainItem(ItemID);
		Destroy();
	}	
}
