// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "../MoveUI/MoveButtonWidget.h"
#include "../ButtonClick.h"
#include "MoveSelectionPopupWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UMoveSelectionPopupWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UFUNCTION()
	void OnBackClicked();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* Back;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UWrapBox* WrapBox;

public:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void AddToWrapBox(UMoveButtonWidget* MoveButton);

	void ClearWrapBox();

	FButtonClicked BackClicked;
};
