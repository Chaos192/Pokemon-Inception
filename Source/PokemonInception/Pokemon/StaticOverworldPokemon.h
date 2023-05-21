// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MoveBaseStruct.h"
#include "StaticOverworldPokemon.generated.h"

UCLASS()
class POKEMONINCEPTION_API AStaticOverworldPokemon : public ACharacter
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly)
	class USoundBase* BallOpenSound = nullptr;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* BallRetrieveSound = nullptr;

	UPROPERTY(EditDefaultsOnly)
	class UNiagaraSystem* BallOpen = nullptr;

	FTimerHandle ChangeSizeTimer;
	FTimerDelegate ChangeSizeDelegate;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* RoarAnimMontage = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* AttackAnimMontage = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* DamageAnimMontage = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* FaintAnimMontage = nullptr;

public:
	void Roar();
	void Attack();
	void GetDamaged();
	void Hide(bool bIsHidden);

	UFUNCTION()
	void ComeOut(float Scale);

	UFUNCTION()
	void ShowBallOpen();

	UFUNCTION()
	void CastMove(FMoveBaseStruct Move);

	UFUNCTION()
	void GetSplashed(FMoveBaseStruct Move);

	UFUNCTION()
	void Faint(float Scale, bool bIsTrainerPokemon);
};
