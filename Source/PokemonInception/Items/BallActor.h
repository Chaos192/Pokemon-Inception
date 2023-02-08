// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallActor.generated.h"

UCLASS()
class POKEMONINCEPTION_API ABallActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABallActor();

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* Mesh = nullptr;
};
