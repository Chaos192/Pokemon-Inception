// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoveBaseStruct.h"
#include "TypeStruct.h"
#include "StatusMoveStruct.generated.h"

UENUM()
enum class ETarget {
	Self,
	Opponent
};

UENUM()
enum class EEffect {
	RestoreHP,
	AttackUp,
	AttackDown,
	DefenceUp,
	DefenceDown,
	SpeedUp,
	SpeedDown
};

USTRUCT(BlueprintType)
struct FStatusMoveStruct : public FMoveBaseStruct
{
	GENERATED_BODY()

	FStatusMoveStruct() {
		Target = ETarget::Self;
		MoveEffects.Add(EEffect::RestoreHP);
		Name = FText::FromString("None");
		MoveType = ETypes::None;
		PowerPoints = 0;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETarget Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EEffect> MoveEffects;
};
