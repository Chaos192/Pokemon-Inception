// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "PotionBase.generated.h"


UCLASS()
class POKEMONINCEPTION_API UPotionBase : public UItemBase
{
	GENERATED_BODY()
	
public:
	UPotionBase();

protected:
	UPROPERTY(EditAnywhere)
	int RestoredHP;
};
