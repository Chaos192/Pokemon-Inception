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

void UFightWidget::ClearWrapBox()
{
	if (WrapBox->HasAnyChildren()) {
		WrapBox->ClearChildren();
	}
}

void UFightWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Back->OnClicked.AddDynamic(this, &UFightWidget::OnBackClicked);
}
