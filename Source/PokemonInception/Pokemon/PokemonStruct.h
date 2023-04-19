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

	UPROPERTY(VisibleAnywhere)
	FPokemonBaseStruct SpeciesData;

	UPROPERTY(VisibleAnywhere)
	int CurrHP = 0;

	UPROPERTY(VisibleAnywhere)
	int MaxHP = 0;

	UPROPERTY(VisibleAnywhere)
	int Attack = 0;

	UPROPERTY(VisibleAnywhere)
	int Defence = 0;

	UPROPERTY(VisibleAnywhere)
	int Speed = 0;

	UPROPERTY(VisibleAnywhere)
	TArray<int> CurrentMoves;

	UPROPERTY(VisibleAnywhere)
	TArray<FMoveBaseStruct> Moves;

	UPROPERTY(VisibleAnywhere)
	int Exp = 0;

	UPROPERTY(VisibleAnywhere)
	int CurrExp = 0;

	UPROPERTY(VisibleAnywhere)
	int RequiredExp = 0;

	UPROPERTY(VisibleAnywhere)
	int Level = 0;

	UPROPERTY(VisibleAnywhere)
	bool bIsFainted = 0;

	UPROPERTY(VisibleAnywhere)
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

		if (Level == 100) {
			RequiredExp = 0;
		}
		else {
			RequiredExp = (Level + 1) * (Level + 1) * (Level + 1) - Exp;
		}
	}

	void InitMoves(TArray<UDataTable*> MoveTables) {
		int LearnedMoves = 0;

		for (auto& Move : SpeciesData.MovePool) {
			FMoveBaseStruct* MoveRef = nullptr;

			for (UDataTable* DataTable : MoveTables) {
				MoveRef = DataTable->FindRow<FMoveBaseStruct>(Move.Value, "");

				if (MoveRef) {
					FMoveBaseStruct AddedMove = *MoveRef;

					if (Move.Key <= Level) {
						AddedMove.bIsLocked = false;
						LearnedMoves++;
					}

					Moves.Add(AddedMove);
				}
			}
		}

		InitCurrentMoves(LearnedMoves);
	}

	void InitCurrentMoves(int LearnedMoves) {
		for (int i = LearnedMoves - 4; i < LearnedMoves; i++) {
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

	FString GetLatestMoveName() {
		FMoveBaseStruct LatestMove;

		for (FMoveBaseStruct i : Moves) {
			if (!i.bIsLocked) {
				LatestMove = i;
			}
		}

		return LatestMove.Name.ToString();
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

	bool bIsHighLevel(int DefeatedTrainers) {
		float i = float(Level) / float(DefeatedTrainers + 1);
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, FString::SanitizeFloat(i));
		if (i <= 10.0f) {
			return false;
		}
		return true;
	}

	bool AddEffect(EEffect Effect) {
		if (Effects.Contains(Effect) == true) {
			return false;
		}

		if (Effect == EEffect::RestoreHP) {
			if (!bIsFullHp()) {
				RestoreHP(MaxHP / 2);
				return true;
			}
			else {
				return false;
			}
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
