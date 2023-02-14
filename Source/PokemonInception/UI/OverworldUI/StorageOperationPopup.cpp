// Fill out your copyright notice in the Description page of Project Settings.


#include "StorageOperationPopup.h"

void UStorageOperationPopup::OnOperationClicked()
{
	OperationClicked.Broadcast(Id);
}

void UStorageOperationPopup::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Operation->OnClicked.AddDynamic(this, &UStorageOperationPopup::OnOperationClicked);
	Action->OnClicked.AddDynamic(this, &UStorageOperationPopup::OnActionClicked);
	Cancel->OnClicked.AddDynamic(this, &UStorageOperationPopup::OnCancelClicked);
}

void UStorageOperationPopup::SetOperationText(FText Text)
{
	OperationText->SetText(Text);
}
