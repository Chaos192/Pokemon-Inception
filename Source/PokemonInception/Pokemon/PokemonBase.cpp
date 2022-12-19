// Fill out your copyright notice in the Description page of Project Settings.


#include "PokemonBase.h"


void APokemonBase::Init(int StartingLevel)
{
	Level = StartingLevel;
	CalculateStats();
}

void APokemonBase::CalculateStats()
{
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

void APokemonBase::GainExp(int GainedExp)
{
	if (Level == 100) {
		return;
	}

	CurrExp += GainedExp;
	if (CurrExp >= RequiredExp) {
		LevelUp();
	}
}

void APokemonBase::LevelUp()
{
	Level++;
	if (Level == 100) {
		CurrExp = RequiredExp;
	}
	else {
		CurrExp -= RequiredExp;
	}

	CalculateStats();
}

void APokemonBase::MovesInit()
{
}

void APokemonBase::RecieveDamage(int Damage)
{
	if (Damage > CurrHP) {
		CurrHP = 0;
		bIsFainted = true;
	}
	else {
		CurrHP -= Damage;
	}
}

void APokemonBase::RestoreHP(int RestoredHP)
{
	if ((CurrHP + RestoredHP) == MaxHP) {
		CurrHP = MaxHP;
	}
	else {
		CurrHP += RestoredHP;
	}
}

bool APokemonBase::IsPokemonFainted()
{
	return bIsFainted;
}

void APokemonBase::RecoverStatus()
{
	bIsFainted = false;
	CurrHP = 1;
}