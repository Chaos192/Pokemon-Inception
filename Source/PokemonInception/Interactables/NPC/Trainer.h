// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCBase.h"
#include "../../Pokemon/PokemonStruct.h"
#include "Trainer.generated.h"


USTRUCT()
struct FTrainerPokemonData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName PokemonID;

	UPROPERTY(EditAnywhere)
	int PokemonLevel;

	UPROPERTY(EditAnywhere)
	FName Move1 = "None";

	UPROPERTY(EditAnywhere)
	FName Move2 = "None";

	UPROPERTY(EditAnywhere)
	FName Move3 = "None";

	UPROPERTY(EditAnywhere)
	FName Move4 = "None";
};

UCLASS()
class POKEMONINCEPTION_API ATrainer : public ANPCBase
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* ThrowAnimMontage = nullptr;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TArray<FTrainerPokemonData> TrainerTeamData;

	UPROPERTY()
	TArray<FPokemonStruct> TrainerTeam;

public:
	ATrainer();
	void Throw();

	UFUNCTION()
	virtual void Interact(APlayerController* PlayerController) override;

	UPROPERTY(EditAnywhere)
	int DefeatedTrainersRequiredToChallange = 0;

	UPROPERTY(VisibleAnywhere)
	bool bHasBeenDefeated = false;
};
