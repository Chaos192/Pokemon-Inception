// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TypeStruct.h"
#include "MoveBaseStruct.h"
#include "PokemonBaseStruct.h"
#include "PokemonStruct.generated.h"

USTRUCT(BlueprintType)
struct FPokemonStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPokemonBaseStruct SpeciesData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrHP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxHP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Attack = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Defence = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Speed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMoveBaseStruct> CurrentMoves;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMoveBaseStruct> Moves;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Exp = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrExp = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int RequiredExp = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Level = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsFainted = 0;

	void Init(int StartingLevel, FPokemonBaseStruct Species) {
		SpeciesData = Species;
		Level = StartingLevel;

		CalculateStats();
	}

	void CalculateStats() {
		MaxHP = ((Level / 100 + 1) * SpeciesData.HP + Level);
		CurrHP = MaxHP;

		Attack = (int)(((Level / 50 + 1) * SpeciesData.Attack) / 1.5f);
		Defence = (int)(((Level / 50 + 1) * SpeciesData.Defence) / 1.5f);
		Speed = (int)(((Level / 50 + 1) * SpeciesData.Speed) / 1.5f);

		if (Level == 1) {
			Exp = 0;
		}
		else {
			Exp = Level * Level * Level;
		}

		RequiredExp = (Level + 1) * (Level + 1) * (Level + 1) - Exp;
	}

	void InitMoves(TArray<UDataTable*> MoveTables) {
		for (auto& Move : SpeciesData.MovePool) {
			if (Move.Key <= Level) {
				FMoveBaseStruct* AddedMove = nullptr;

				for (UDataTable* DataTable : MoveTables) {
					AddedMove = DataTable->FindRow<FMoveBaseStruct>(Move.Value, "");
					
					if (AddedMove) {
						Moves.Add(*AddedMove);
					}
				}
			}
		}

		InitCurrentMoves();
	}

	void InitCurrentMoves() {
		if (Moves.Num() <= 4) {
			CurrentMoves = Moves;
		}

		else {
			for (int i = Moves.Num() - 4; i < Moves.Num(); i++) {
				CurrentMoves.Add(Moves[i]);
			}
		}
	}

	bool CheckForNewMoves() {
		return false;
	}

	void GainExp(int GainedExp) {
		if (Level == 100) {
			return;
		}

		CurrExp += GainedExp;
		if (CurrExp >= RequiredExp) {
			LevelUp();
		}
	}

	void LevelUp() {
		Level++;
		if (Level == 100) {
			CurrExp = RequiredExp;
		}
		else {
			CurrExp -= RequiredExp;
		}

		CalculateStats();
	}

	void RecieveDamage(int Damage) {
		if (Damage > CurrHP) {
			CurrHP = 0;
			bIsFainted = true;
		}
		else {
			CurrHP -= Damage;
		}
	}

	void RestoreHP(int RestoredHP) {
		if ((CurrHP + RestoredHP) == MaxHP) {
			CurrHP = MaxHP;
		}
		else {
			CurrHP += RestoredHP;
		}
	}

	bool IsPokemonFainted() {
		return bIsFainted;
	}

	void RecoverStatus() {
		bIsFainted = false;
		CurrHP = 1;
	}
};
