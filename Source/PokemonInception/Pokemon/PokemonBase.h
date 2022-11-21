// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PokemonBase.generated.h"

UCLASS()
class POKEMONINCEPTION_API APokemonBase : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	int MaxHP = 0;

	UPROPERTY(VisibleAnywhere)
	int CurrHP = 0;

	UPROPERTY(VisibleAnywhere)
	int Attack = 0;

	UPROPERTY(VisibleAnywhere)
	int Defence = 0;

	UPROPERTY(VisibleAnywhere)
	int Speed = 0;

	UPROPERTY(VisibleAnywhere)
	int Exp = 0;

	UPROPERTY(VisibleAnywhere)
	int CurrExp = 0;

	UPROPERTY(VisibleAnywhere)
	int RequiredExp = 0;

	bool IsFainted = false;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	FString Name;

	UPROPERTY(EditAnywhere)
	FString Nickname;

	UPROPERTY(EditAnywhere)
	float BaseHP = 1;

	UPROPERTY(EditAnywhere)
	float BaseAttack = 1;

	UPROPERTY(EditAnywhere)
	float BaseDefence = 1;
	
	UPROPERTY(EditAnywhere)
	float BaseSpeed = 1;

	UPROPERTY(EditAnywhere)
	int Level = 1;

	UPROPERTY(EditAnywhere)
	int LevelToEvolve = 1;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APokemonBase> EvolvedFormClass;

	UPROPERTY()
	class APokemonBase* EvolvedForm;

public:
	APokemonBase();

	UFUNCTION()
	void CalculateStats();

	UFUNCTION()
	void SetLevel(int LevelToSet);

	UFUNCTION()
	void GainExp(int GainedExp);

	UFUNCTION()
	void LevelUp();
	
	UFUNCTION()
	void Evolve();

	UFUNCTION()
	void RecieveDamage(int Damage);

	UFUNCTION()
	void RestoreHP(int RestoredHP);

	UFUNCTION()
	bool IsPokemonFainted();

	UFUNCTION()
	void RecoverStatus();


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
