// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameWidget.h"

void UInGameWidget::SetMessage(FString InMessage)
{
	Message->SetText(FText::FromString(InMessage));
}
