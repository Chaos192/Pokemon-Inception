// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainerCardWidget.h"

void UTrainerCardWidget::OnBackClicked()
{
	BackClicked.Broadcast();
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Native On Initialize"));
}

void UTrainerCardWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Back->OnClicked.AddDynamic(this, &UTrainerCardWidget::OnBackClicked);
}
