// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCBase.h"
#include "../../Pokemon/PokemonStruct.h"
#include "Trainer.generated.h"


UCLASS()
class POKEMONINCEPTION_API ATrainer : public ANPCBase
{
	GENERATED_BODY()
	
public:
	ATrainer();

	UFUNCTION()
	virtual void Interact(APlayerController* PlayerController) override;

	UPROPERTY(EditAnywhere)
	TArray<FName> PokemonIDs;

	UPROPERTY(EditAnywhere)
	TArray<int> PokemonLevels;
};
