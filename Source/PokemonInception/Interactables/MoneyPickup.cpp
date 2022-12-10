// Fill out your copyright notice in the Description page of Project Settings.


#include "MoneyPickup.h"
#include "Components/StaticMeshComponent.h"
#include "../Player/PlayerCharacterController.h"

AMoneyPickup::AMoneyPickup()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	Action = "pick up";
	money = 100;
}

void AMoneyPickup::Interact(APlayerController* Controller)
{
	APlayerCharacterController* cont = Cast<APlayerCharacterController>(Controller);
	if (cont != nullptr) {
		cont->RecieveMoney(money);
		Destroy();
	}
}
