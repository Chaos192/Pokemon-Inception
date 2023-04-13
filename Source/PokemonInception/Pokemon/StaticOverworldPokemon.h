// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StaticOverworldPokemon.generated.h"

UCLASS()
class POKEMONINCEPTION_API AStaticOverworldPokemon : public ACharacter
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* RoarAnimMontage = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* AttackAnimMontage = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* DamageAnimMontage = nullptr;

	UFUNCTION()
	void PlayCry();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class USoundBase* Cry = nullptr;

public:
	UPROPERTY()
	ACameraActor* Camera;

	void Roar();
	void Attack();
	void GetDamaged();
};
