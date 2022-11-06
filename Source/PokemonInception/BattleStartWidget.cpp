// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleStartWidget.h"

void UBattleStartWidget::OnRunClicked()
{
	RunClicked.Broadcast();
}

void UBattleStartWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Run->OnClicked.AddDynamic(this, &UBattleStartWidget::OnRunClicked);
}
