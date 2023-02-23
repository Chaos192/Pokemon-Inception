// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCBase.h"
#include "MoveTutor.generated.h"


UCLASS()
class POKEMONINCEPTION_API AMoveTutor : public ANPCBase
{
	GENERATED_BODY()

public:
	AMoveTutor();

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void Interact(APlayerController* PlayerController) override;
};
