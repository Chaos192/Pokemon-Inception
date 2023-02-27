// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCBase.h"
#include "Shopkeeper.generated.h"


UCLASS()
class POKEMONINCEPTION_API AShopkeeper : public ANPCBase
{
	GENERATED_BODY()

public:
	AShopkeeper();

	UFUNCTION()
	virtual void Interact(APlayerController* PlayerController) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> ItemsToSell;
};
