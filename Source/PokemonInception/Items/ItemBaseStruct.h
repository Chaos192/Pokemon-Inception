// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemBaseStruct.generated.h"


USTRUCT(BlueprintType)
struct FItemBaseStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Image = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUsableInBattle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUsableOutsideBattle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemStructType;

	bool operator==(const FItemBaseStruct& Item) const {
		if (ItemID == Item.ItemID) {
			return true;
		}
		else return false;
	}
};
