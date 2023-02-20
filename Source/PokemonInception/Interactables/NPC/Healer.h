// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCBase.h"
#include "Healer.generated.h"


UCLASS()
class POKEMONINCEPTION_API AHealer : public ANPCBase
{
	GENERATED_BODY()

public:
	AHealer();

	UFUNCTION()
	virtual void Interact(APlayerController* Controller) override;
};
