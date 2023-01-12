// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../Pokemon/PokemonBaseStruct.h"
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

	UFUNCTION()
	void LoadInventory(TArray<FItemBaseStruct> InventoryData);

	UFUNCTION()
	void LoadPokemonParty(TArray<FPokemonStruct> PartyData);

	UFUNCTION()
	void LoadPokemonStorage(TArray<FPokemonStruct> StorageData);

	UFUNCTION()
	void RegisterToPokedex(FPokemonBaseStruct Species);

	UFUNCTION()
	void LoadPokedexData(TArray<FPokemonBaseStruct> PokedexData);

	UFUNCTION()
	bool bIsRegisteredInPokedex(FName ID);

	int GetMoney() const;

	void RecieveMoney(int AddedMoney);

	void LoseMoney(int LostMoney);

	TArray<FItemBaseStruct> GetInventory() const;

	TArray<FPokemonStruct> GetPokemonParty() const;

	TArray<FPokemonStruct> GetPokemonStorage() const;

	TArray<FPokemonBaseStruct> GetPokedexData() const;

	FPauseSignature PauseDelegate;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FItemBaseStruct> Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FPokemonBaseStruct> Pokedex;

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
