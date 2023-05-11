// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCBase.h"
#include "Guide.generated.h"


UCLASS()
class POKEMONINCEPTION_API AGuide : public ANPCBase
{
	GENERATED_BODY()
	
public:
	AGuide();

	UFUNCTION()
	virtual void Interact(APlayerController* PlayerController) override;
};
