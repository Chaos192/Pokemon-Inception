// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleWidget.h"

void UTitleWidget::OnPlayClicked()
{
	PlayClicked.Broadcast();
}

void UTitleWidget::OnControllsClicked()
{
	ControllsClicked.Broadcast();
}

void UTitleWidget::OnQuitClicked()
{
	QuitClicked.Broadcast();
}

void UTitleWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Play->OnClicked.AddDynamic(this, &UTitleWidget::OnPlayClicked);
	Controlls->OnClicked.AddDynamic(this, &UTitleWidget::OnControllsClicked);
	Quit->OnClicked.AddDynamic(this, &UTitleWidget::OnQuitClicked);
}
