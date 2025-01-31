// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TypeStruct.h"
#include "MoveBaseStruct.generated.h"


USTRUCT(BlueprintType)
struct FMoveBaseStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MoveID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETypes MoveType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int PowerPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrPowerPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MoveStructType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsLocked;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UNiagaraSystem* Particle = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class USoundBase* Sound = nullptr;

	bool bHasMaxPP() {
		if (CurrPowerPoints == PowerPoints) {
			return true;
		}

		return false;
	}
};
