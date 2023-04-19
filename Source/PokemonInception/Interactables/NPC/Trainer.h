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
	
private:
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* ThrowAnimMontage = nullptr;

public:
	ATrainer();
	void Throw();

	UFUNCTION()
	virtual void Interact(APlayerController* PlayerController) override;

	UPROPERTY(EditAnywhere)
	TArray<FName> PokemonIDs;

	UPROPERTY(EditAnywhere)
	TArray<int> PokemonLevels;

	UPROPERTY(EditAnywhere)
	int DefeatedTrainersRequiredToChallange = 0;

	UPROPERTY(VisibleAnywhere)
	bool bHasBeenDefeated = false;
};
