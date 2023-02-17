// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveInfoWidget.h"

void UMoveInfoWidget::SetMoveName(FText Name)
{
	MoveName->SetText(Name);
}

void UMoveInfoWidget::SetMoveType(FText Type)
{
	MoveType->SetText(Type);
}

void UMoveInfoWidget::SetMovePower(FText Power)
{
	MovePower->SetText(Power);
}

void UMoveInfoWidget::SetMoveDescription(FText Description)
{
	MoveDescription->SetText(Description);
}
