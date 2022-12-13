// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TypeStruct.generated.h"


USTRUCT(BlueprintType)
struct FTypeStruct : public FTableRowBase
{
	GENERATED_BODY()

	FTypeStruct() {
		SuperEffectiveAgainst.Add(FName("None"));
		NotVeryEffectiveAgainst.Add(FName("None"));
		NoEffectAgainst.Add(FName("None"));
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TypeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> SuperEffectiveAgainst;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> NotVeryEffectiveAgainst;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> NoEffectAgainst;

	bool operator > (const FTypeStruct& Type) const {
		if (SuperEffectiveAgainst.Contains(Type.TypeName)) {
			return true;
		}
		else return false;
	}

	bool operator < (const FTypeStruct& Type) const {
		if (NotVeryEffectiveAgainst.Contains(Type.TypeName)) {
			return true;
		}
		else return false;
	}

	bool operator != (const FTypeStruct& Type) const {
		if (NoEffectAgainst.Contains(Type.TypeName)) {
			return true;
		}
		else return false;
	}
};
