// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TypeStruct.h"
#include "PokemonBaseStruct.generated.h"

USTRUCT(BlueprintType)
struct FLearnableMoveStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int LevelWhenLearned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle Move;
};

USTRUCT(BlueprintType)
struct FPokemonBaseStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName PokemonID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Info;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETypes Type1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETypes Type2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Defence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BaseExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CatchRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FLearnableMoveStruct> MovePool;
};
