// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "MoveButtonWidget.h"
#include "MoveInfoWidget.h"
#include "../ButtonClick.h"
#include "FightWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UFightWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UFUNCTION()
	void OnBackClicked();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* Back;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UWrapBox* WrapBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UWrapBox* InfoWrapBox;

public:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void AddToWrapBox(UMoveButtonWidget* MoveButton);
	UFUNCTION()
	void ShowMoveInfo(UMoveInfoWidget* MoveInfo);

	void ClearWrapBox();
	void ClearInfoBox();

	FButtonClicked BackClicked;
};
