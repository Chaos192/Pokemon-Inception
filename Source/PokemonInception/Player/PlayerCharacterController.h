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
	void ObtainItem(FName ID);

	TArray<FItemBaseStruct> GetInventory() const;

	FPauseSignature PauseDelegate;

protected:
	void Interact();

	void TogglePause();

	virtual void SetupInputComponent() override;
};
