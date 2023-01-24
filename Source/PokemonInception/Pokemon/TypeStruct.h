// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TypeStruct.generated.h"


UENUM()
enum class ETypes {
	None,
	Normal,
	Grass,
	Fire,
	Water,
	Earth,
	Air,
	Electric,
	Bug,
	Light,
	Dark,
	Dragon
};

USTRUCT(BlueprintType)
struct FTypeStruct : public FTableRowBase
{
	GENERATED_BODY()

	FTypeStruct() {
		SuperEffectiveAgainst.Add(ETypes::None);
		NotVeryEffectiveAgainst.Add(ETypes::None);
		NoEffectAgainst.Add(ETypes::None);
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETypes TypeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ETypes> SuperEffectiveAgainst;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ETypes> NotVeryEffectiveAgainst;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ETypes> NoEffectAgainst;
};
