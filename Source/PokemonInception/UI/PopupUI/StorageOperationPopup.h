// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PopupUI/PopupSelectionWidget.h"
#include "StorageOperationPopup.generated.h"


UCLASS()
class POKEMONINCEPTION_API UStorageOperationPopup : public UPopupSelectionWidget
{
	GENERATED_BODY()
	
protected:
	UFUNCTION()
	void OnOperationClicked();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* Operation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* OperationText;

public:
	virtual void NativeOnInitialized() override;

	void SetOperationText(FText Text);

	FElementIDSignature OperationClicked;
};
