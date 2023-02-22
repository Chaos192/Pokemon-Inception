// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCBase.h"
#include "PokemonStorageManager.generated.h"


UCLASS()
class POKEMONINCEPTION_API APokemonStorageManager : public ANPCBase
{
	GENERATED_BODY()
	
public:
	APokemonStorageManager();

	UFUNCTION()
	virtual void Interact(APlayerController* PlayerController) override;
};
