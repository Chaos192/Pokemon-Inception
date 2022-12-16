// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBaseStruct.h"
#include "EtherBaseStruct.generated.h"

USTRUCT(BlueprintType)
struct FEtherBaseStruct : public FItemBaseStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int RestoredPP;
};
