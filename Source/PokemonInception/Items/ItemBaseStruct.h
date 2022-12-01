// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "../Interactables/ItemPickup.h"
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
	UTexture2D* Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AItemPickup> Pickup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUsable;

	//constructor
	//operator overload
};
