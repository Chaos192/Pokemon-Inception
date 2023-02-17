// Fill out your copyright notice in the Description page of Project Settings.


#include "ControllsWidget.h"

void UControllsWidget::OnBackClicked()
{
	BackClicked.Broadcast();
}

void UControllsWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Back->OnClicked.AddDynamic(this, &UControllsWidget::OnBackClicked);
}
