// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsWidget.h"

void USettingsWidget::OnBackClicked()
{
	BackClicked.Broadcast();
}

void USettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Back->OnClicked.AddDynamic(this, &USettingsWidget::OnBackClicked);
}
