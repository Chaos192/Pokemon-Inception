// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveSelectionPopupWidget.h"

void UMoveSelectionPopupWidget::OnBackClicked()
{
	BackClicked.Broadcast();
}

void UMoveSelectionPopupWidget::AddToWrapBox(UMoveButtonWidget* MoveButton)
{
	WrapBox->AddChildToWrapBox(MoveButton);
}

void UMoveSelectionPopupWidget::ClearWrapBox()
{
	if (WrapBox->HasAnyChildren()) {
		WrapBox->ClearChildren();
	}
}

void UMoveSelectionPopupWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Back->OnClicked.AddDynamic(this, &UMoveSelectionPopupWidget::OnBackClicked);
}