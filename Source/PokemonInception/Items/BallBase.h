// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "BallBase.generated.h"


UCLASS()
class POKEMONINCEPTION_API UBallBase : public UItemBase
{
	GENERATED_BODY()
	
public:
	UBallBase();

protected:
	UPROPERTY(EditAnywhere)
	float CatchMultiplier;
};
