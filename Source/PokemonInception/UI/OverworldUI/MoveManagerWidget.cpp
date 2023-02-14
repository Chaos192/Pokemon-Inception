// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveManagerWidget.h"

void UMoveManagerWidget::OnBackClicked()
{
	BackClicked.Broadcast();
}

void UMoveManagerWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Back->OnClicked.AddDynamic(this, &UMoveManagerWidget::OnBackClicked);
}

void UMoveManagerWidget::AddToCurrentMoves(UMoveButtonWidget* MoveButton)
{
	CurrentMovesBox->AddChildToWrapBox(MoveButton);
}

void UMoveManagerWidget::AddToAvailableMoves(UMoveButtonWidget* MoveButton)
{
	AvailableMovesBox->AddChild(MoveButton);
}

void UMoveManagerWidget::ShowMoveInfo(UMoveInfoWidget* MoveInfo)
{
	if (MoveDescriptionBox->HasAnyChildren()) {
		MoveDescriptionBox->ClearChildren();
	}
	MoveDescriptionBox->AddChildToWrapBox(MoveInfo);
}

void UMoveManagerWidget::Clear()
{
	if (CurrentMovesBox->HasAnyChildren()) {
		CurrentMovesBox->ClearChildren();
	}
	if (AvailableMovesBox->HasAnyChildren()) {
		AvailableMovesBox->ClearChildren();
	}
	if (MoveDescriptionBox->HasAnyChildren()) {
		MoveDescriptionBox->ClearChildren();
	}
}
