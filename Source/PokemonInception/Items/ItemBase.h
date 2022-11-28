// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemBase.generated.h"


UCLASS(Blueprintable)
class POKEMONINCEPTION_API UItemBase : public UObject
{
	GENERATED_BODY()
	
public:
	UItemBase();

protected:
	UPROPERTY(EditAnywhere)
	FString Name;

	UPROPERTY(EditAnywhere)
	int Value;

	UPROPERTY(EditAnywhere)
	UTexture2D* Picture;
};
