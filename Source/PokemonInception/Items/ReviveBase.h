// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "ReviveBase.generated.h"


UCLASS()
class POKEMONINCEPTION_API UReviveBase : public UItemBase
{
	GENERATED_BODY()
	
public:
	UReviveBase();

protected:
	UPROPERTY(EditAnywhere)
	float RevivePercent;
};
