// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TypeStruct.h"
#include "MoveBaseStruct.h"
#include "PokemonBaseStruct.h"
#include "StatusMoveStruct.h"
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
	TArray<int> CurrentMoves;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EEffect> Effects;

	void Init(int StartingLevel, FPokemonBaseStruct Species) {
		SpeciesData = Species;
		Level = StartingLevel;

		CalculateStats();
	}

	void CalculateStats() {
		int OldMaxHP = MaxHP;
		MaxHP = ((Level / 100 + 1) * SpeciesData.HP + Level);
		
		if (bIsFainted == true) {
			CurrHP = 1;
			bIsFainted = false;
		}
		else CurrHP += MaxHP - OldMaxHP;

		Attack = (((Level / 50.0 + 1) * SpeciesData.Attack) / 1.5);
		Defence = (((Level / 50.0 + 1) * SpeciesData.Defence) / 1.5);
		Speed = (((Level / 50.0 + 1) * SpeciesData.Speed) / 1.5);

		if (Level == 1) {
			Exp = 0;
		}
		else {
			Exp = Level * Level * Level;
		}

		RequiredExp = (Level + 1) * (Level + 1) * (Level + 1) - Exp;
	}

	void InitMoves(TArray<UDataTable*> MoveTables) {
		int LastLearnedMoveID = -1;
		for (auto& Move : SpeciesData.MovePool) {
			FMoveBaseStruct* AddedMove = nullptr;

			for (UDataTable* DataTable : MoveTables) {
				AddedMove = DataTable->FindRow<FMoveBaseStruct>(Move.Value, "");

				if (AddedMove) {
					AddedMove->CurrPowerPoints = AddedMove->PowerPoints;
					if (Move.Key <= Level) {
						AddedMove->bIsLocked = false;
					}

					Moves.Add(*AddedMove);
					LastLearnedMoveID++;
				}
			}
		}

		InitCurrentMoves(LastLearnedMoveID);
	}

	void InitCurrentMoves(int LastLearnedMoveID) {
		for (int i = LastLearnedMoveID - 4; i <= LastLearnedMoveID; i++) {
			if (i < 0) {
				continue;
			}

			if (Moves[i].bIsLocked == false) {
				CurrentMoves.Add(i);
			}
		}
	}

	bool CheckForNewMoves() {
		bool bHasLearnedMove = false;

		int i = 0;
		for (auto& Move : SpeciesData.MovePool) {
			if (Move.Key <= Level && Moves[i].bIsLocked == true) {
				Moves[i].bIsLocked = false;

				if (CurrentMoves.Num() < 4) {
					CurrentMoves.Add(i);
				}

				bHasLearnedMove = true;
			}

			i++;
		}

		return bHasLearnedMove;
	}

	bool GainExp(int GainedExp) {
		if (Level == 100) {
			return false;
		}

		CurrExp += GainedExp;
		if (CurrExp >= RequiredExp) {
			LevelUp();
			return true;
		}

		return false;
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
		if (CurrExp >= RequiredExp) {
			LevelUp();
		}
	}

	void RecieveDamage(int Damage) {
		CurrHP -= Damage;

		if (CurrHP <= 0) {
			CurrHP = 0;
			ClearEffects();
			bIsFainted = true;
		}
	}

	bool RestoreHP(int RestoredHP) {
		if (CurrHP == MaxHP) {
			return false;
		}
		
		if ((CurrHP + RestoredHP) >= MaxHP) {
			CurrHP = MaxHP;
		}

		else {
			CurrHP += RestoredHP;
		}

		return true;
	}

	bool RestorePP(int RestoredPP, int MoveID) {
		if (Moves[MoveID].CurrPowerPoints == Moves[MoveID].PowerPoints) {
			return false;
		}

		if ((Moves[MoveID].CurrPowerPoints + RestoredPP) >= Moves[MoveID].PowerPoints) {
			Moves[MoveID].CurrPowerPoints = Moves[MoveID].PowerPoints;
		}

		else {
			Moves[MoveID].CurrPowerPoints += RestoredPP;
		}

		return true;
	}

	void RecoverStatus() {
		bIsFainted = false;
		CurrHP = 1;
	}

	bool bHasRanOutOfPP() {
		for (int i = 0; i < CurrentMoves.Num(); i++) {
			if (Moves[CurrentMoves[i]].CurrPowerPoints > 0) {
				return false;
			}
		}

		return true;
	}

	bool bIsFullHp() {
		if (CurrHP == MaxHP) {
			return true;
		}

		return false;
	}

	void FullRestore() {
		if (bIsFainted == true) {
			RecoverStatus();
		}

		RestoreHP(MaxHP);

		for (int i = 0; i < Moves.Num(); i++) {
			RestorePP(Moves[i].PowerPoints, i);
		}
	}

	int GetSpeed() {
		if (Effects.Contains(EEffect::SpeedUp)) {
			return Speed * 1.5;
		}

		if (Effects.Contains(EEffect::SpeedDown)) {
			return Speed * 0.5;
		}

		return Speed;
	}

	bool AddEffect(EEffect Effect) {
		if (Effects.Contains(Effect) == true) {
			return false;
		}

		if (Effects.Contains(EEffect::AttackDown) && Effect == EEffect::AttackUp) {
			Effects.Remove(EEffect::AttackDown);
			return true;
		}

		if (Effects.Contains(EEffect::AttackUp) && Effect == EEffect::AttackDown) {
			Effects.Remove(EEffect::AttackUp);
			return true;
		}

		if (Effects.Contains(EEffect::DefenceDown) && Effect == EEffect::DefenceUp) {
			Effects.Remove(EEffect::DefenceDown);
			return true;
		}

		if (Effects.Contains(EEffect::DefenceUp) && Effect == EEffect::DefenceDown) {
			Effects.Remove(EEffect::DefenceUp);
			return true;
		}

		if (Effects.Contains(EEffect::SpeedDown) && Effect == EEffect::SpeedUp) {
			Effects.Remove(EEffect::SpeedDown);
			return true;
		}

		if (Effects.Contains(EEffect::SpeedUp) && Effect == EEffect::SpeedDown) {
			Effects.Remove(EEffect::SpeedDown);
			return true;
		}

		Effects.Add(Effect);
		return true;
	}

	void ClearEffects() {
		Effects.Empty();
	}
};
