// Fill out your copyright notice in the Description page of Project Settings.


#include "PokedexWidget.h"

void UPokedexWidget::OnBackClicked()
{
	BackClicked.Broadcast();
}

void UPokedexWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Back->OnClicked.AddDynamic(this, &UPokedexWidget::OnBackClicked);
	
}

