// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoveBaseStruct.h"
#include "TypeStruct.h"
#include "AttackMoveStruct.generated.h"


USTRUCT(BlueprintType)
struct FAttackMoveStruct : public FMoveBaseStruct
{
	GENERATED_BODY()

	FAttackMoveStruct() {
		BaseDamage = 0;
		Name = FText::FromString("None");
		MoveType = ETypes::None;
		PowerPoints = 0;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BaseDamage;
};
