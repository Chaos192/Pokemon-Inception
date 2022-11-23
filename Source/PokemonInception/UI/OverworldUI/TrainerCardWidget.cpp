// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainerCardWidget.h"

void UTrainerCardWidget::OnBackClicked()
{
	BackClicked.Broadcast();
}

void UTrainerCardWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Back->OnClicked.AddDynamic(this, &UTrainerCardWidget::OnBackClicked);
}
