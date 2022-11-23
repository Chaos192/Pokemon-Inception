// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleStartWidget.h"

void UBattleStartWidget::OnRunClicked()
{
	RunClicked.Broadcast();
}

void UBattleStartWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Fight->OnClicked.AddDynamic(this, &UBattleStartWidget::OnRunClicked);
	Pokemon->OnClicked.AddDynamic(this, &UBattleStartWidget::OnRunClicked);
	Bag->OnClicked.AddDynamic(this, &UBattleStartWidget::OnRunClicked);
	Run->OnClicked.AddDynamic(this, &UBattleStartWidget::OnRunClicked);
}
