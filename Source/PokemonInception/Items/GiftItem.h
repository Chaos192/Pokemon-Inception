// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.h"
#include "../Player/PokemonInceptionCharacter.h"
#include "GiftItem.generated.h"

UCLASS()
class POKEMONINCEPTION_API AGiftItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AGiftItem();

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Mesh = nullptr;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* Box = nullptr;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UItemBase> Item;

private:
	UFUNCTION()
	void GetItem(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
