// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterController.h"
#include "../PokemonInception.h"
#include "../Interactables/InteractableInterface.h"
#include "../Interactables/PickUp/PickupBase.h"
#include "../Interactables/NPC/NPCBase.h"
#include "../GameModes/OverworldGameMode.h"
#include "PokemonInceptionCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../UI/HUD/OverworldHUD.h"

void APlayerCharacterController::Interact()
{
	if (!IsValid(FoundInteractable)) {
		return;
	}

	Cast<IInteractableInterface>(FoundInteractable)->Interact(this);
}

bool APlayerCharacterController::bIsPartyDefeated()
{
	for (FPokemonStruct Pokemon : PokemonParty) {
		if (Pokemon.bIsFainted == false) {
			return false;
		}
	}
	return true;
}

void APlayerCharacterController::ToggleMainMenu()
{
	PauseDelegate.Broadcast();
}

void APlayerCharacterController::TogglePeacefulMode()
{
	PeacefulModeDelegate.Broadcast();
}

bool APlayerCharacterController::bCanRemoveFromParty(int PokemonID)
{
	if (PokemonParty.Num() == 1) {
		return false;
	}

	for (int i = 0; i < PokemonParty.Num(); i++) {
		if (PokemonParty[i].bIsFainted == false && i != PokemonID) {
			return true;
		}
	}

	return false;
}

bool APlayerCharacterController::bIsPartyFull()
{
	if (PokemonParty.Num() == 6) {
		return true;
	}

	return false;
}

bool APlayerCharacterController::bCanObtainMorePokemon()
{
	if (PokemonParty.Num() + PokemonStorage.Num() == 100) {
		return false;
	}
	
	return true;
}

void APlayerCharacterController::SetFoundInteractable(AActor* Interactable)
{
	FoundInteractable = Interactable;

	if (!FoundInteractable) {
		InteractableMessageDelegate.Broadcast(" ");
		return;
	}

	if (ANPCBase* NPC = Cast<ANPCBase>(FoundInteractable)) {
		InteractableMessageDelegate.Broadcast(NPC->Name + ": Press E to talk to");
		return;
	}

	if (APickupBase* Pickup = Cast<APickupBase>(FoundInteractable)) {
		InteractableMessageDelegate.Broadcast(Pickup->Name + ": Press E to pick up");
		return;
	}
}

void APlayerCharacterController::ObtainItem(FItemBaseStruct Item)
{
	Inventory.Add(Item);
}

void APlayerCharacterController::LoseItem(FItemBaseStruct Item)
{
	Inventory.RemoveSingle(Item);
}

int APlayerCharacterController::GetItemCount(FName ItemID)
{
	int Count = 0;

	for (FItemBaseStruct Item : Inventory) {
		if (Item.ItemID == ItemID) {
			Count++;
		}
	}

	return Count;
}

void APlayerCharacterController::ObtainPokemon(FPokemonStruct Pokemon)
{
	if (bIsRegisteredInPokedex(Pokemon.SpeciesData.PokemonID) == false) {
		RegisterToPokedex(Pokemon.SpeciesData);
	}

	if (PokemonParty.Num() == 6) {
		PokemonStorage.Add(Pokemon);
	}
	else PokemonParty.Add(Pokemon);
}

void APlayerCharacterController::MovePokemonToParty(int PokemonID)
{
	AOverworldHUD* Hud = Cast<AOverworldHUD>(GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	if (bIsPartyFull()) {
		Hud->ClearPopup();
		Hud->OnScreenMessage("Can't add more pokemon to your party!");
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, "Adding " + PokemonStorage[PokemonID].SpeciesData.Name.ToString() + " to the party!");

	PokemonParty.Add(PokemonStorage[PokemonID]);
	PokemonStorage.RemoveAt(PokemonID);

	Hud->ShowPokemonStorage();
}

void APlayerCharacterController::MovePokemonToStorage(int PokemonID)
{
	AOverworldHUD* Hud = Cast<AOverworldHUD>(GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	if (!bCanRemoveFromParty(PokemonID)) {
		Hud->ClearPopup();
		Hud->OnScreenMessage("You need at least one healthy pokemon!");
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, "Adding " + PokemonParty[PokemonID].SpeciesData.Name.ToString() + " to the storage!");

	PokemonStorage.Add(PokemonParty[PokemonID]);
	PokemonParty.RemoveAt(PokemonID);

	Hud->ShowPokemonStorage();
}

void APlayerCharacterController::ReleasePokemonFromParty(int PokemonID)
{
	AOverworldHUD* Hud = Cast<AOverworldHUD>(GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	if (!bCanRemoveFromParty(PokemonID)) {
		Hud->ClearPopup();
		Hud->OnScreenMessage("You need at least one healthy pokemon!");
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, "Releasing " + PokemonParty[PokemonID].SpeciesData.Name.ToString() + ", who was in the party!");

	PokemonParty.RemoveAt(PokemonID);

	Hud->ShowPokemonStorage();
}

void APlayerCharacterController::ReleasePokemonFromStorage(int PokemonID)
{
	AOverworldHUD* Hud = Cast<AOverworldHUD>(GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, "Releasing " + PokemonStorage[PokemonID].SpeciesData.Name.ToString() + ", who was in the storage!");

	PokemonStorage.RemoveAt(PokemonID);

	Hud->ShowPokemonStorage();
}

void APlayerCharacterController::LearnMove(int MoveID)
{
	AOverworldHUD* Hud = Cast<AOverworldHUD>(GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	if (PokemonParty[0].CurrentMoves.Num() == 4) {
		Hud->ClearPopup();
		Hud->OnScreenMessage("Your pokemon can't learn more moves!");
		return;
	}

	PokemonParty[0].CurrentMoves.Add(MoveID);
	Hud->ShowMoveManager(0);
}

void APlayerCharacterController::ForgetMove(int MoveID)
{
	AOverworldHUD* Hud = Cast<AOverworldHUD>(GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	if (PokemonParty[0].CurrentMoves.Num() == 1) {
		Hud->ClearPopup();
		Hud->OnScreenMessage("Your pokemon must know at least one move!");
		return;
	}

	PokemonParty[0].CurrentMoves.RemoveSingle(MoveID);
	Hud->ShowMoveManager(0);
}

void APlayerCharacterController::RegisterToPokedex(FPokemonBaseStruct Species)
{
	Pokedex.Add(Species);
}

bool APlayerCharacterController::bIsRegisteredInPokedex(FName ID)
{
	for (FPokemonBaseStruct PokemonData : Pokedex) {
		if (PokemonData.PokemonID == ID) {
			return true;
		}
	}
	return false;
}

void APlayerCharacterController::FullRestoreAllPokemon()
{
	for (int i = 0; i < PokemonParty.Num(); i++) {
		PokemonParty[i].FullRestore();
		GEngine->AddOnScreenDebugMessage(1, 1, FColor::Green, TEXT("RESTORED!"));
	}
	for (int i = 0; i < PokemonStorage.Num(); i++) {
		PokemonStorage[i].FullRestore();
	}
}

void APlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacterController::Interact);
	InputComponent->BindAction("PeacefulMode", IE_Pressed, this, &APlayerCharacterController::TogglePeacefulMode);
	InputComponent->BindAction("Menu", IE_Pressed, this, &APlayerCharacterController::ToggleMainMenu).bExecuteWhenPaused = true;
}
