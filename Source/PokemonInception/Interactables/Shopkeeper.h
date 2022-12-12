// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Shopkeeper.generated.h"


UCLASS()
class POKEMONINCEPTION_API AShopkeeper : public AInteractable
{
	GENERATED_BODY()

public:
	AShopkeeper();

	UFUNCTION()
	virtual void Interact(APlayerController* Controller) override;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* Capsule = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> ItemsToSell;
};
