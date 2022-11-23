// Fill out your copyright notice in the Description page of Project Settings.


#include "BagWidget.h"

void UBagWidget::OnBackClicked()
{
	BackClicked.Broadcast();
}

void UBagWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Back->OnClicked.AddDynamic(this, &UBagWidget::OnBackClicked);
}
