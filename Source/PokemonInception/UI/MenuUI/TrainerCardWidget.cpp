// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainerCardWidget.h"

void UTrainerCardWidget::OnBackClicked()
{
	BackClicked.Broadcast();
}

void UTrainerCardWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Back->OnClicked.AddDynamic(this, &UTrainerCardWidget::OnBackClicked);
}

void UTrainerCardWidget::SetPlayerName(FString Name)
{
	PlayerName->SetText(FText::FromString(Name));
}

void UTrainerCardWidget::SetRegisteredSpecies(int Num)
{
	SpeciesRegistered->SetText(FText::FromString(FString::FromInt(Num)));
}

void UTrainerCardWidget::SetPokemonCaught(int Num)
{
	PokemonCaught->SetText(FText::FromString(FString::FromInt(Num)));
}

void UTrainerCardWidget::SetMoneyObtained(int Num)
{
	MoneyObtained->SetText(FText::FromString(FString::FromInt(Num)));
}

void UTrainerCardWidget::SetTrainersDefeated(int Num)
{
	TrainersDefeated->SetText(FText::FromString(FString::FromInt(Num)));
}
