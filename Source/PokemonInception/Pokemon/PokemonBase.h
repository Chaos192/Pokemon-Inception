// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PokemonBaseStruct.h"
#include "MoveBaseStruct.h"
#include "PokemonBase.generated.h"

UCLASS()
class POKEMONINCEPTION_API APokemonBase : public AActor
{
	GENERATED_BODY()
	
public:	
	UFUNCTION()
	void Init(int StartingLevel);

	UFUNCTION()
	void CalculateStats();

	UFUNCTION()
	void GainExp(int GainedExp);

	UFUNCTION()
	void LevelUp();

	UFUNCTION()
	void MovesInit();

	UFUNCTION()
	void RecieveDamage(int Damage);

	UFUNCTION()
	void RestoreHP(int RestoredHP);

	UFUNCTION()
	bool IsPokemonFainted();

	UFUNCTION()
	void RecoverStatus();

	UFUNCTION()
	FText GetPokemonName();


protected:
	UPROPERTY(EditDefaultsOnly)
	FPokemonBaseStruct SpeciesData;

	UPROPERTY(VisibleAnywhere)
	int MaxHP;

	UPROPERTY(VisibleAnywhere)
	int CurrHP;

	UPROPERTY(VisibleAnywhere)
	int Attack;

	UPROPERTY(VisibleAnywhere)
	int Defence;

	UPROPERTY(VisibleAnywhere)
	int Speed;

	UPROPERTY(VisibleAnywhere)
	TArray<FMoveBaseStruct> Moves;

	UPROPERTY(VisibleAnywhere)
	TArray<FMoveBaseStruct> CurrentMoves;
	
private:
	int Exp;

	int CurrExp;

	int RequiredExp;

	int Level;

	bool bIsFainted;
};
