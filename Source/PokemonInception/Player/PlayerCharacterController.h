// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../Interactables/Interactable.h"
#include "../Items/ItemBaseStruct.h"
#include "PlayerCharacterController.generated.h"


UCLASS()
class POKEMONINCEPTION_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FItemBaseStruct> Inventory;

	UFUNCTION()
	void ObtainItem(FName ItemID);

protected:
	void Interact();

	virtual void SetupInputComponent() override;
};
