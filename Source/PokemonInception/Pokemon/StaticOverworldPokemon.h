// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StaticOverworldPokemon.generated.h"

UCLASS()
class POKEMONINCEPTION_API AStaticOverworldPokemon : public AActor
{
	GENERATED_BODY()
	
public:	
	AStaticOverworldPokemon();

	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* Mesh = nullptr;
};
