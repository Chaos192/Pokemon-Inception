// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupBase.h"
#include "../../Pokemon/PokemonStruct.h"
#include "../../Pokemon/PokemonBaseStruct.h"
#include "PokemonPickup.generated.h"


UCLASS()
class POKEMONINCEPTION_API APokemonPickup : public APickupBase
{
	GENERATED_BODY()

public:
	APokemonPickup();

	UFUNCTION()
	virtual void Interact(APlayerController* Controller) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int PokemonLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName PokemonID;
};
