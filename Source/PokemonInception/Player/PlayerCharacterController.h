// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../Pokemon/PokemonStruct.h"
#include "../Interactables/Interactable.h"
#include "../Items/ItemBaseStruct.h"
#include "PlayerCharacterController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPauseSignature);

UCLASS()
class POKEMONINCEPTION_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void ObtainItem(FItemBaseStruct Item);

	UFUNCTION()
	void LoseItem(FItemBaseStruct Item);

	UFUNCTION()
	void ObtainPokemon(FPokemonStruct Pokemon);

	TArray<FItemBaseStruct> GetInventory() const;

	TArray<FPokemonStruct> GetPokemonParty() const;

	TArray<FPokemonStruct> GetPokemonStorage() const;

	UFUNCTION()
	void LoadInventory(TArray<FItemBaseStruct> InventoryData);

	UFUNCTION()
	void LoadPokemonParty(TArray<FPokemonStruct> PartyData);

	UFUNCTION()
	void LoadPokemonStorage(TArray<FPokemonStruct> StorageData);

	int GetMoney() const;

	void RecieveMoney(int AddedMoney);

	void LoseMoney(int LostMoney);

	FPauseSignature PauseDelegate;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FItemBaseStruct> Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FPokemonStruct> PokemonParty;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FPokemonStruct> PokemonStorage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int money;

	void Interact();

	void TogglePause();

	virtual void SetupInputComponent() override;
};
