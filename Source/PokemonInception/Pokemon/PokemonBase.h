// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PokemonBaseStruct.h"
#include "PokemonBase.generated.h"

UCLASS()
class POKEMONINCEPTION_API APokemonBase : public AActor
{
	GENERATED_BODY()
	
public:	
	APokemonBase();

protected:
	UPROPERTY()
	FPokemonBaseStruct SpeciesData;

	virtual void BeginPlay() override;


};
