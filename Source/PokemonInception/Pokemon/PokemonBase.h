// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PokemonBase.generated.h"

UCLASS()
class POKEMONINCEPTION_API APokemonBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APokemonBase();

	USkeletalMesh* GetPokemonMesh();
	FString GetMeshReference();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* AlternateMeshAsset;

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

	UPROPERTY(EditAnywhere)
	FString MeshReference;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
