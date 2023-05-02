// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../InteractableInterface.h"
#include "PickupBase.generated.h"


UCLASS()
class POKEMONINCEPTION_API APickupBase : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* Box = nullptr;

	UPROPERTY(EditAnywhere)
	FString Name;

	virtual void Interact(APlayerController* PlayerController) override;
};
