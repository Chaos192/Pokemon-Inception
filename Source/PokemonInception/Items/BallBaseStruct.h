// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBaseStruct.h"
#include "BallActor.h"
#include "BallBaseStruct.generated.h"

USTRUCT(BlueprintType)
struct FBallBaseStruct : public FItemBaseStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CatchMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ABallActor> BallActor;
};
