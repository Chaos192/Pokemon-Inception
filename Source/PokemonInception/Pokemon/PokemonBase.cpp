// Fill out your copyright notice in the Description page of Project Settings.


#include "PokemonBase.h"

// Sets default values
APokemonBase::APokemonBase()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APokemonBase::BeginPlay()
{
	Super::BeginPlay();
	CalculateStats();
}

void APokemonBase::CalculateStats()
{
	MaxHP = ((Level / 100 + 1) * BaseHP + Level);
	CurrHP = MaxHP;

	Attack = (int)(((Level / 50 + 1) * BaseAttack) / 1.5f);
	Defence = (int)(((Level / 50 + 1) * BaseDefence) / 1.5f);
	Speed = (int)(((Level / 50 + 1) * BaseSpeed) / 1.5f);

	if (Level == 1) {
		Exp = 0;
	}
	else {
		Exp = Level * Level * Level;
	}

	RequiredExp = (Level + 1) * (Level + 1) * (Level + 1) - Exp;
}

void APokemonBase::SetLevel(int LevelToSet)
{
	Level = LevelToSet;
	CalculateStats();
}

void APokemonBase::GainExp(int GainedExp)
{
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

	if (Level >= LevelToEvolve) {
		Evolve();
	}
}

void APokemonBase::Evolve()
{
	//EvolvedForm = 
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Evolving"));
}

void APokemonBase::RecieveDamage(int Damage)
{
	if (Damage > CurrHP) {
		CurrHP = 0;
		IsFainted = true;
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
	return IsFainted;
}

void APokemonBase::RecoverStatus()
{
	IsFainted = false;
	CurrHP = 1;
}


void APokemonBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


