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
		case ETypes::Normal: MoveButton->SetBackgroundColor(FLinearColor(207, 207, 207, 1));
			break;
		case ETypes::Grass: MoveButton->SetBackgroundColor(FLinearColor(181, 230, 29, 1));
			break;
		case ETypes::Fire: MoveButton->SetBackgroundColor(FLinearColor(177, 42, 0, 1));
			break;
		case ETypes::Water: MoveButton->SetBackgroundColor(FLinearColor(0, 116, 184, 1));
			break;
		case ETypes::Earth: MoveButton->SetBackgroundColor(FLinearColor(127, 100, 22, 1));
			break;
		case ETypes::Air: MoveButton->SetBackgroundColor(FLinearColor(39, 200, 183, 1));
			break;
		case ETypes::Electric: MoveButton->SetBackgroundColor(FLinearColor(255, 231, 15, 1));
			break;
		case ETypes::Bug: MoveButton->SetBackgroundColor(FLinearColor(215, 220, 51, 1));
			break;
		case ETypes::Light: MoveButton->SetBackgroundColor(FLinearColor(219, 176, 187, 1));
			break;
		case ETypes::Dark: MoveButton->SetBackgroundColor(FLinearColor(90, 88, 85, 1));
			break;
		case ETypes::Dragon: MoveButton->SetBackgroundColor(FLinearColor(159, 100, 157, 1));
			break;
	}
}
