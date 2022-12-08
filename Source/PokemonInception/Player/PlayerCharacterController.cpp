// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterController.h"
#include "../PokemonInception.h"
#include "../Interactables/Interactable.h"
#include "../GameModes/OverworldGameMode.h"
#include "PokemonInceptionCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../UI/OverworldUI/OverworldHUD.h"

void APlayerCharacterController::Interact()
{
	APokemonInceptionCharacter* PlayerOwner = Cast<APokemonInceptionCharacter>(GetPawn());
	//if (PlayerOwner == nullptr) {
	//	return;
	//}
	
	FHitResult HitResult;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(PlayerOwner);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Destructible));

	FVector BoxHalfSize = FVector(50, 50, 100);
	FRotator PlayerFaceDirection = PlayerOwner->GetActorRotation();
	FVector BoxLocation = PlayerOwner->GetActorLocation();

	BoxLocation += PlayerFaceDirection.Vector() * BoxHalfSize.Y;

	FRotator BoxDirection = PlayerFaceDirection;
	BoxDirection.Yaw += 90;

	if (UKismetSystemLibrary::BoxTraceSingleForObjects(GetWorld(), BoxLocation, BoxLocation, BoxHalfSize, BoxDirection,
		ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true,
		FLinearColor(1, 0, 0), FLinearColor(0, 1, 0), 1.f) == false) 
	{
		return;
	}
	
	AInteractable* CurrentInteractable = Cast<AInteractable>(HitResult.GetActor());
	if (CurrentInteractable != nullptr) {
		CurrentInteractable->Interact(this);
	}
}

void APlayerCharacterController::TogglePause()
{
	PauseDelegate.Broadcast();
}

void APlayerCharacterController::ObtainItem(FName ID)
{
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr) {
		return;
	}

	AOverworldHUD* Hud = Cast<AOverworldHUD>(GetHUD());
	if (GameMode == nullptr) {
		return;
	}

	TArray<UDataTable*> ItemTables = GameMode->GetItemDT();
	FItemBaseStruct* AddedItem = nullptr;

	for (UDataTable* ItemTable : ItemTables) {
		AddedItem = ItemTable->FindRow<FItemBaseStruct>(ID, "");

		if (AddedItem) {
			Hud->OnScreenMessage("You got a " + AddedItem->Name.ToString() + "!");
			Inventory.Add(*AddedItem);

			return;
		}
	}
}

TArray<FItemBaseStruct> APlayerCharacterController::GetInventory() const
{
	return Inventory;
}

void APlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacterController::Interact);
	InputComponent->BindAction("Menu", IE_Pressed, this, &APlayerCharacterController::TogglePause).bExecuteWhenPaused = true;
}
