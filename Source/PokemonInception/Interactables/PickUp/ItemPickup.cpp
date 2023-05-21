// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemPickup.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "../../Player/PlayerCharacterController.h"
#include "../../GameModes/OverworldGameMode.h"
#include "../../UI/HUD/OverworldHUD.h"

AItemPickup::AItemPickup()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(Mesh);

	Name = "Item Pickup";
	ItemID = FName("None");
}

void AItemPickup::Interact(APlayerController* Controller)
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
		
	TArray<UDataTable*> ItemTables = GameMode->GetItemDT();
	FItemBaseStruct* AddedItem = nullptr;

	for (UDataTable* ItemTable : ItemTables) {
		AddedItem = ItemTable->FindRow<FItemBaseStruct>(ItemID, "");

		if (AddedItem) {
			Hud->ShowPickupMessage("You got a " + AddedItem->Name.ToString() + "!", AddedItem->Image);
			PlayerController->ObtainItem(*AddedItem);
			GameMode->MarkActorAsDestroyed(this);
			Destroy();

			return;
		}
	}
}
