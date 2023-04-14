// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallActor.generated.h"

UCLASS()
class POKEMONINCEPTION_API ABallActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABallActor();

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* BallClick = nullptr;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* BallBreak = nullptr;

	UPROPERTY(EditDefaultsOnly)
	class UNiagaraSystem* BallSparkle = nullptr;

	UPROPERTY(EditDefaultsOnly)
	class UNiagaraSystem* BallFail = nullptr;

	void CatchSuccess();
	void CatchFail();
};
