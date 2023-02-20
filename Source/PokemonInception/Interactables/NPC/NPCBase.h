// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Interactable.h"
#include "NPCBase.generated.h"


UCLASS()
class POKEMONINCEPTION_API ANPCBase : public AInteractable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* Capsule = nullptr;
};
