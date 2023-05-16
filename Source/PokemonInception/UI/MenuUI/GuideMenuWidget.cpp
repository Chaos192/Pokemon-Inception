// Fill out your copyright notice in the Description page of Project Settings.


#include "GuideMenuWidget.h"

void UGuideMenuWidget::OnExitClicked()
{
	ExitClicked.Broadcast();
}

void UGuideMenuWidget::OnIslandClicked()
{
	IslandClicked.Broadcast(0);
}

void UGuideMenuWidget::OnNPCsClicked()
{
	NPCsClicked.Broadcast(1);
}

void UGuideMenuWidget::OnItemsClicked()
{
	ItemsClicked.Broadcast(2);
}

void UGuideMenuWidget::OnWildPokemonClicked()
{
	WildPokemonClicked.Broadcast(3);
}

void UGuideMenuWidget::OnBattlesClicked()
{
	BattlesClicked.Broadcast(4);
}

void UGuideMenuWidget::OnTypesClicked()
{
	TypesClicked.Broadcast(5);
}

void UGuideMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Exit->OnClicked.AddDynamic(this, &UGuideMenuWidget::OnExitClicked);
	Island->OnClicked.AddDynamic(this, &UGuideMenuWidget::OnIslandClicked);
	NPCs->OnClicked.AddDynamic(this, &UGuideMenuWidget::OnNPCsClicked);
	Items->OnClicked.AddDynamic(this, &UGuideMenuWidget::OnItemsClicked);
	WildPokemon->OnClicked.AddDynamic(this, &UGuideMenuWidget::OnWildPokemonClicked);
	Battles->OnClicked.AddDynamic(this, &UGuideMenuWidget::OnBattlesClicked);
	Types->OnClicked.AddDynamic(this, &UGuideMenuWidget::OnTypesClicked);
}
