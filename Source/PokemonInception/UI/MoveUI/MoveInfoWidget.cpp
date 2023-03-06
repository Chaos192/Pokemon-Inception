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

void UMoveInfoWidget::SetWidgetColor(ETypes Type)
{
	switch (Type) {
		case ETypes::Normal: WidgetBorder->SetBrushColor(FLinearColor(0.811, 0.811, 0.811, 1));
			break;
		case ETypes::Grass: WidgetBorder->SetBrushColor(FLinearColor(0.209, 1, 0.083, 1));
			break;
		case ETypes::Fire: WidgetBorder->SetBrushColor(FLinearColor(1, 0.123, 0, 1));
			break;
		case ETypes::Water: WidgetBorder->SetBrushColor(FLinearColor(0, 0.454, 0.721, 1));
			break;
		case ETypes::Earth: WidgetBorder->SetBrushColor(FLinearColor(0.598, 0.492, 0.186, 1));
			break;
		case ETypes::Air: WidgetBorder->SetBrushColor(FLinearColor(0.352, 0.984, 0.917, 1));
			break;
		case ETypes::Electric: WidgetBorder->SetBrushColor(FLinearColor(1, 1, 0, 1));
			break;
		case ETypes::Bug: WidgetBorder->SetBrushColor(FLinearColor(0.543, 0.862, 0.100, 1));
			break;
		case ETypes::Light: WidgetBorder->SetBrushColor(FLinearColor(0.968, 0.490, 0.873, 1));
			break;
		case ETypes::Dark: WidgetBorder->SetBrushColor(FLinearColor(0.062, 0.042, 0.084, 1));
			break;
		case ETypes::Dragon: WidgetBorder->SetBrushColor(FLinearColor(0.523, 0.092, 0.515, 1));
			break;
	}
}
