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
	bool bIsPartyDefeated();

	bool bCanRemoveFromParty(int PokemonID);

	bool bIsPartyFull();

	bool bCanObtainMorePokemon();

	UFUNCTION()
	void ObtainItem(FItemBaseStruct Item);

	UFUNCTION()
	void LoseItem(FItemBaseStruct Item);

	UFUNCTION()
	void ObtainPokemon(FPokemonStruct Pokemon);

	UFUNCTION()
	void MovePokemonToParty(int PokemonID);

	UFUNCTION()
	void MovePokemonToStorage(int PokemonID);

	UFUNCTION()
	void ReleasePokemonFromParty(int PokemonID);

	UFUNCTION()
	void ReleasePokemonFromStorage(int PokemonID);

	UFUNCTION()
	void RegisterToPokedex(FPokemonBaseStruct Species);

	UFUNCTION()
	bool bIsRegisteredInPokedex(FName ID);

	UFUNCTION()
	void FullRestoreAllPokemon();

	int GetMoney() const;

	void RecieveMoney(int AddedMoney);

	void LoseMoney(int LostMoney);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FItemBaseStruct> Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FPokemonBaseStruct> Pokedex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FPokemonStruct> PokemonParty;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FPokemonStruct> PokemonStorage;

	FPauseSignature PauseDelegate;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int money;

	void Interact();

	void TogglePause();

	virtual void SetupInputComponent() override;
};
