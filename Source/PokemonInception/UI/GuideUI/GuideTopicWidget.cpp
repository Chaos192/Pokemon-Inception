// Fill out your copyright notice in the Description page of Project Settings.


#include "GuideTopicWidget.h"

void UGuideTopicWidget::OnBackClicked()
{
	BackClicked.Broadcast();
}

void UGuideTopicWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Back->OnClicked.AddDynamic(this, &UGuideTopicWidget::OnBackClicked);
}
