// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pokemon00_Base.generated.h"

UCLASS()
class POKEMONINCEPTION_API APokemon00_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	APokemon00_Base();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCapsuleComponent* Capsule = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent* Mesh = nullptr;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	FString Name;

	UPROPERTY(EditAnywhere)
	int MaxHp = 1;

	UPROPERTY(VisibleAnywhere)
	int CurrHp;

	UPROPERTY(EditAnywhere)
	int Attack = 1;

	UPROPERTY(EditAnywhere)
	int Defence = 1;

	UPROPERTY(EditAnywhere)
	int Speed = 1;

public:	
	virtual void Tick(float DeltaTime) override;

	USkeletalMeshComponent* GetPokemonMesh();

};
