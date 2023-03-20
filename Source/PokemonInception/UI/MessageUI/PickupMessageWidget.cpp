// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupMessageWidget.h"

void UPickupMessageWidget::SetImage(UTexture2D* InImage)
{
	Image->SetBrushFromTexture(InImage);
}
