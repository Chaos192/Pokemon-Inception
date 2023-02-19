// Fill out your copyright notice in the Description page of Project Settings.


#include "ControllsWidget.h"

void UControlsWidget::OnBackClicked()
{
	BackClicked.Broadcast();
}

void UControlsWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Back->OnClicked.AddDynamic(this, &UControlsWidget::OnBackClicked);
}
