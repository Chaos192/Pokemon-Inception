// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "MoneyPickup.generated.h"


UCLASS()
class POKEMONINCEPTION_API AMoneyPickup : public AInteractable
{
	GENERATED_BODY()
	
public:
	AMoneyPickup();

	UFUNCTION()
	virtual void Interact(APlayerController* Controller) override;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int money;
};
