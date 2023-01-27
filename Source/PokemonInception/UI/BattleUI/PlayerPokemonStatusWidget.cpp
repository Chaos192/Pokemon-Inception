// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPokemonStatusWidget.h"

void UPlayerPokemonStatusWidget::SetPokemonEXP(int CurrEXP, int MaxEXP)
{
	float EXPPercent = float(CurrEXP) / float(MaxEXP);
	EXPBar->SetPercent(EXPPercent);
}
