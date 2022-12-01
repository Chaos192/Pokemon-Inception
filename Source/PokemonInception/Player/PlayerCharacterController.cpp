// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterController.h"
#include "../PokemonInception.h"
#include "../Interactables/Interactable.h"
#include "../GameModes/OverworldGameMode.h"

void APlayerCharacterController::Interact()
{
	if (CurrentInteratable) {
		CurrentInteratable->Interact(this);
	}
}

void APlayerCharacterController::ObtainItem(FName ItemID)
{
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(GetWorld()->GetAuthGameMode());
	UDataTable* ItemTable = GameMode->GetItemDT();

	FItemBaseStruct* AddedItem = ItemTable->FindRow<FItemBaseStruct>("ItemID", "");

	if (AddedItem) {
		Inventory.Add(*AddedItem);
	}
}

void APlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacterController::Interact);
}
