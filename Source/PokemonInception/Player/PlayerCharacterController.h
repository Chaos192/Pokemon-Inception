// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../Interactables/Interactable.h"
#include "../Items/ItemBaseStruct.h"
#include "PlayerCharacterController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPauseSignature);

UCLASS()
class POKEMONINCEPTION_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FItemBaseStruct> Inventory;

	UFUNCTION()
	void ObtainItem(FItemBaseStruct Item);

	UFUNCTION()
	void LoseItem(FItemBaseStruct Item);

	TArray<FItemBaseStruct> GetInventory() const;

	int GetMoney() const;

	void RecieveMoney(int AddedMoney);

	void LoseMoney(int LostMoney);

	FPauseSignature PauseDelegate;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int money;

	void Interact();

	void TogglePause();

	virtual void SetupInputComponent() override;
};
