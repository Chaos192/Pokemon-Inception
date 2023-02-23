// Fill out your copyright notice in the Description page of Project Settings.


#include "OverheadNameWidget.h"

void UOverheadNameWidget::SetName(FString InName)
{
	Name->SetText(FText::FromString(InName));
}
