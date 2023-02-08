// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "MoveButtonWidget.h"
#include "../ButtonClick.h"
#include "MoveSelectionPopupWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UMoveSelectionPopupWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UFUNCTION()
	void OnBackClicked();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* Back;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UWrapBox* WrapBox;

public:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void AddToWrapBox(UMoveButtonWidget* MoveButton);

	void ClearWrapBox();

	FButtonClicked BackClicked;
};
