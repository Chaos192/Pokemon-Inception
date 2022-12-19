// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "../Pokemon/PokemonBase.h"
#include "PokemonPickup.generated.h"


UCLASS()
class POKEMONINCEPTION_API APokemonPickup : public AInteractable
{
	GENERATED_BODY()

public:
	APokemonPickup();

	UFUNCTION()
	virtual void Interact(APlayerController* Controller) override;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class APokemonBase* Pokemon;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int PokemonLevel;
};
