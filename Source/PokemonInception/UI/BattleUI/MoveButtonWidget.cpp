// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveButtonWidget.h"

void UMoveButtonWidget::OnButtonClicked()
{
	ButtonClicked.Broadcast();
}

void UMoveButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();
	MoveButton->OnClicked.AddDynamic(this, &UMoveButtonWidget::OnButtonClicked);
}

void UMoveButtonWidget::InitButton(FText Name, int CurrPP, int MaxPP, ETypes MoveType)
{
	MoveName->SetText(Name);
	MovePP->SetText(FText::FromString(FString::FromInt(CurrPP)));
	MoveMaxPP->SetText(FText::FromString(FString::FromInt(MaxPP)));

	switch (MoveType) {
		case ETypes::Normal: MoveButton->SetBackgroundColor(FLinearColor(0.811, 0.811, 0.811, 1));
			break;
		case ETypes::Grass: MoveButton->SetBackgroundColor(FLinearColor(0.209, 1, 0.083, 1));
			break;
		case ETypes::Fire: MoveButton->SetBackgroundColor(FLinearColor(1, 0.123, 0, 1));
			break;
		case ETypes::Water: MoveButton->SetBackgroundColor(FLinearColor(0, 0.454, 0.721, 1));
			break;
		case ETypes::Earth: MoveButton->SetBackgroundColor(FLinearColor(0.598, 0.492, 0.186, 1));
			break;
		case ETypes::Air: MoveButton->SetBackgroundColor(FLinearColor(0.352, 0.984, 0.917, 1));
			break;
		case ETypes::Electric: MoveButton->SetBackgroundColor(FLinearColor(1, 1, 0, 1));
			break;
		case ETypes::Bug: MoveButton->SetBackgroundColor(FLinearColor(0.543, 0.862, 0.100, 1));
			break;
		case ETypes::Light: MoveButton->SetBackgroundColor(FLinearColor(0.968, 0.490, 0.873, 1));
			break;
		case ETypes::Dark: MoveButton->SetBackgroundColor(FLinearColor(0.062, 0.042, 0.084, 1));
			break;
		case ETypes::Dragon: MoveButton->SetBackgroundColor(FLinearColor(0.523, 0.092, 0.515, 1));
			break;
	}
}
