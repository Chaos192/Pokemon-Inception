// Fill out your copyright notice in the Description page of Project Settings.


#include "FightWidget.h"

void UFightWidget::OnBackClicked()
{
	BackClicked.Broadcast();
}

void UFightWidget::AddToWrapBox(UMoveButtonWidget* MoveButton)
{
	WrapBox->AddChildToWrapBox(MoveButton);
}

void UFightWidget::ShowMoveInfo(UMoveInfoWidget* MoveInfo)
{
	InfoWrapBox->AddChildToWrapBox(MoveInfo);
}

void UFightWidget::ClearWrapBox()
{
	if (WrapBox->HasAnyChildren()) {
		WrapBox->ClearChildren();
	}
}

void UFightWidget::ClearInfoBox()
{
	if (InfoWrapBox->HasAnyChildren()) {
		InfoWrapBox->ClearChildren();
	}
}

void UFightWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Back->OnClicked.AddDynamic(this, &UFightWidget::OnBackClicked);
}
