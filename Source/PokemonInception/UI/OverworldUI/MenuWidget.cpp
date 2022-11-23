// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"

void UMenuWidget::OnPokedexClicked()
{
	PokedexClicked.Broadcast();
}

void UMenuWidget::OnPokemonClicked()
{
	PokemonClicked.Broadcast();
}

void UMenuWidget::OnBagClicked()
{
	BagClicked.Broadcast();
}

void UMenuWidget::OnTrainerCardClicked()
{
	TrainerCardClicked.Broadcast();
}

void UMenuWidget::OnSaveClicked()
{
	SaveClicked.Broadcast();
}

void UMenuWidget::OnSettingsClicked()
{
	SettingsClicked.Broadcast();
}

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Pokedex->OnClicked.AddDynamic(this, &UMenuWidget::OnPokedexClicked);
	Pokemon->OnClicked.AddDynamic(this, &UMenuWidget::OnPokemonClicked);
	Bag->OnClicked.AddDynamic(this, &UMenuWidget::OnBagClicked);
	TrainerCard->OnClicked.AddDynamic(this, &UMenuWidget::OnTrainerCardClicked);
	Save->OnClicked.AddDynamic(this, &UMenuWidget::OnSaveClicked);
	Settings->OnClicked.AddDynamic(this, &UMenuWidget::OnSettingsClicked);
}
