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
	MaxHP = ((Level/100 + 1) * BaseHP + Level);
	CurrHP = MaxHP;

	Attack = (int)(((Level/50 + 1) * BaseAttack) / 1.5f);
	Defence = (int)(((Level / 50 + 1) * BaseDefence) / 1.5f);
	Speed = (int)(((Level / 50 + 1) * BaseSpeed) / 1.5f);

	if (Level == 1) {
		EXP = 0;
	}
	else {
		EXP = Level * Level * Level;
	}

	RequiredExp = (Level + 1) * (Level + 1) * (Level + 1) - EXP;
}

void APokemonBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


