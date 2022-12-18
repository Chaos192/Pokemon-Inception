// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleStartWidget.h"

void UBattleStartWidget::OnFightClicked()
{
	FightClicked.Broadcast();
}

void UBattleStartWidget::OnBagClicked()
{
	BagClicked.Broadcast();
}

void UBattleStartWidget::OnRunClicked()
{
	RunClicked.Broadcast();
}

void UBattleStartWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Fight->OnClicked.AddDynamic(this, &UBattleStartWidget::OnFightClicked);
	Pokemon->OnClicked.AddDynamic(this, &UBattleStartWidget::OnRunClicked);
	Bag->OnClicked.AddDynamic(this, &UBattleStartWidget::OnBagClicked);
	Run->OnClicked.AddDynamic(this, &UBattleStartWidget::OnRunClicked);
}
