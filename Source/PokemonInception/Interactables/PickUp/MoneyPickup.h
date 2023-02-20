// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupBase.h"
#include "MoneyPickup.generated.h"


UCLASS()
class POKEMONINCEPTION_API AMoneyPickup : public APickupBase
{
	GENERATED_BODY()
	
public:
	AMoneyPickup();

	UFUNCTION()
	virtual void Interact(APlayerController* Controller) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int money;
};
