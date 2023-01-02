// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Pokemon/PokemonBaseStruct.h"
#include "../Pokemon/PokemonStruct.h"
#include "WildPokemon.generated.h"

UCLASS()
class POKEMONINCEPTION_API AWildPokemon : public ACharacter
{
	GENERATED_BODY()

public:
	AWildPokemon();

	UFUNCTION()
	void InitPokemon(int Level);

private:
	UPROPERTY(EditDefaultsOnly)
	class UDataTable* PokemonDatatable;

	UPROPERTY(EditDefaultsOnly)
	FName PokemonID;

	UPROPERTY()
	FPokemonStruct Pokemon;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
