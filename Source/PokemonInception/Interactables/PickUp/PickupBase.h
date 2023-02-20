// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Interactable.h"
#include "PickupBase.generated.h"


UCLASS()
class POKEMONINCEPTION_API APickupBase : public AInteractable
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Mesh = nullptr;
};
