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

	FString ToString(ETypes Type) {
		switch (Type) {
			case ETypes::Normal:
				return "Normal";
			case ETypes::Grass:
				return "Grass";
			case ETypes::Fire:
				return "Fire";
			case ETypes::Water:
				return "Water";
			case ETypes::Earth:
				return "Earth";
			case ETypes::Air:
				return "Air";
			case ETypes::Electric:
				return "Electric";
			case ETypes::Bug:
				return "Bug";
			case ETypes::Light:
				return "Light";
			case ETypes::Dark:
				return "Dark";
			case ETypes::Dragon:
				return "Dragon";
			default:
				return " ";
		}
	}
};
