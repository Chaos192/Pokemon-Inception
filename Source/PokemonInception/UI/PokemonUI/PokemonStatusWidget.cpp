// Fill out your copyright notice in the Description page of Project Settings.

#include "PokemonStatusWidget.h"
#include "../HUD/BattleHUD.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

void UPokemonStatusWidget::SetPokemonName(FText Name)
{
	PokemonName->SetText(Name);
}

void UPokemonStatusWidget::SetPokemonLevel(int Level)
{
	PokemonLevel->SetText(FText::FromString("Lv. " + FString::FromInt(Level)));
}

void UPokemonStatusWidget::SetPokemonHP(int CurrHP, int MaxHP)
{
	FText HP = FText::FromString(FString::FromInt(CurrHP) + "/" + FString::FromInt(MaxHP));
	PokemonHP->SetText(HP);

	float HpPercent = float(CurrHP) / float(MaxHP);
	HPBar->SetPercent(HpPercent);
}

void UPokemonStatusWidget::SetPokemonStatus(TArray<EEffect> Effects)
{
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	if (Effects.Contains(EEffect::AttackUp)) {
		AttackStatus->SetBrushFromTexture(Hud->AttackUpImage);
		AttackStatus->SetOpacity(1);
	}
	else if(Effects.Contains(EEffect::AttackDown)) {
		AttackStatus->SetBrushFromTexture(Hud->AttackDownImage);
		AttackStatus->SetOpacity(1);
	}
	else {
		AttackStatus->SetOpacity(0);
	}

	if (Effects.Contains(EEffect::DefenceUp)) {
		DefenceStatus->SetBrushFromTexture(Hud->DefenceUpImage);
		DefenceStatus->SetOpacity(1);
	}
	else if (Effects.Contains(EEffect::DefenceDown)) {
		DefenceStatus->SetBrushFromTexture(Hud->DefenceDownImage);
		DefenceStatus->SetOpacity(1);
	}
	else {
		DefenceStatus->SetOpacity(0);
	}

	if (Effects.Contains(EEffect::SpeedUp)) {
		SpeedStatus->SetBrushFromTexture(Hud->SpeedUpImage);
		SpeedStatus->SetOpacity(1);
	}
	else if (Effects.Contains(EEffect::SpeedDown)) {
		SpeedStatus->SetBrushFromTexture(Hud->SpeedDownImage);
		SpeedStatus->SetOpacity(1);
	}
	else {
		SpeedStatus->SetOpacity(0);
	}
}
