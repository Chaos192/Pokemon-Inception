// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AreaBox.generated.h"

UCLASS()
class POKEMONINCEPTION_API AAreaBox : public AActor
{
	GENERATED_BODY()
	
public:	
	AAreaBox();

	UPROPERTY(EditAnywhere)
	class UBoxComponent* TriggerBox = nullptr;

	UPROPERTY(EditAnywhere)
	FString AreaName;

	UFUNCTION()
	void OnAreaEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
