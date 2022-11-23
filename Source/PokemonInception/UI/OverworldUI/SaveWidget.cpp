// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveWidget.h"

void USaveWidget::OnBackClicked()
{
	BackClicked.Broadcast();
}

void USaveWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Back->OnClicked.AddDynamic(this, &USaveWidget::OnBackClicked);
}
