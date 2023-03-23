// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerNameWidget.h"

void UPlayerNameWidget::OnPlayClicked()
{
	PlayClicked.Broadcast(NameText->GetText().ToString());
}

void UPlayerNameWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Play->OnClicked.AddDynamic(this, &UPlayerNameWidget::OnPlayClicked);
}

void UPlayerNameWidget::SetErrorText(FString InErrorText)
{
	ErrorText->SetText(FText::FromString(InErrorText));
}
