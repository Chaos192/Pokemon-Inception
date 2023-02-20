// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupBase.h"
#include "ItemPickup.generated.h"


UCLASS()
class POKEMONINCEPTION_API AItemPickup : public APickupBase
{
	GENERATED_BODY()
	
public:
	AItemPickup();

	UFUNCTION()
	virtual void Interact(APlayerController* Controller) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;
};
