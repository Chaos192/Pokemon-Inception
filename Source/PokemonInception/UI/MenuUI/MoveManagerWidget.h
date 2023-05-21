// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "Components/ScrollBox.h"
#include "../ButtonClick.h"
#include "../MoveUI/MoveButtonWidget.h"
#include "../MoveUI/MoveInfoWidget.h"
#include "MoveManagerWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UMoveManagerWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UFUNCTION()
	void OnBackClicked();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* Back;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UWrapBox* CurrentMovesBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* AvailableMovesBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UWrapBox* MoveDescriptionBox;

public:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void AddToCurrentMoves(UMoveButtonWidget* MoveInfo);
	UFUNCTION()
	void AddToAvailableMoves(UMoveButtonWidget* MoveButton);

	void ShowMoveInfo(UMoveInfoWidget* MoveButton);
	void Clear();

	FButtonClicked BackClicked;
};
