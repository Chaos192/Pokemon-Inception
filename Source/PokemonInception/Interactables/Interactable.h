// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UCLASS()
class POKEMONINCEPTION_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteractable();

	UPROPERTY(EditAnywhere)
	class UBoxComponent* Box = nullptr;

	virtual void Interact(APlayerController* Controller){};

	UPROPERTY(EditDefaultsOnly)
	FString Action;

protected:
	virtual void BeginPlay() override;
};
