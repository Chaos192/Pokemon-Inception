// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PokemonBase.h"
#include "../Player/PokemonInceptionCharacter.h"
#include "GiftPokemon.generated.h"

UCLASS()
class POKEMONINCEPTION_API AGiftPokemon : public AActor
{
	GENERATED_BODY()
	
public:	
	AGiftPokemon();

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Mesh = nullptr;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* Box = nullptr;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class APokemonBase> Pokemon;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


private:
	UFUNCTION()
	void GetPokemon();

};
