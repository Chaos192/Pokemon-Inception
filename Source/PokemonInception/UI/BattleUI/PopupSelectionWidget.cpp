// Fill out your copyright notice in the Description page of Project Settings.


#include "PopupSelectionWidget.h"

void UPopupSelectionWidget::OnActionClicked()
{
	ActionClicked.Broadcast(Id);
}

void UPopupSelectionWidget::OnCancelClicked()
{
	CancelClicked.Broadcast();
}

void UPopupSelectionWidget::SetId(int InId)
{
	Id = InId;
}

void UPopupSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Action->OnClicked.AddDynamic(this, &UPopupSelectionWidget::OnActionClicked);
	Cancel->OnClicked.AddDynamic(this, &UPopupSelectionWidget::OnCancelClicked);
}