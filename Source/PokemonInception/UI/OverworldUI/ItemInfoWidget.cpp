// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInfoWidget.h"

void UItemInfoWidget::OnUseClicked()
{
	UseClicked.Broadcast();
}

void UItemInfoWidget::SetDescription(FText Description)
{
	DescriptionText->SetText(Description);
}

void UItemInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Use->OnClicked.AddDynamic(this, &UItemInfoWidget::OnUseClicked);
}
