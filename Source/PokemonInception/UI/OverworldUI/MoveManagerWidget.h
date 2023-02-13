// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "Components/ScrollBox.h"
#include "../ButtonClick.h"
#include "../BattleUI/MoveButtonWidget.h"
#include "MoveManagerWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UMoveManagerWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UFUNCTION()
	void OnBackClicked();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* Back;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UWrapBox* CurrentMovesBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UScrollBox* AvailableMovesBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UWrapBox* MoveDescriptionBox;

public:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void AddToCurrentMoves(UMoveButtonWidget* MoveButton);

	UFUNCTION()
	void AddToAvailableMoves(UMoveButtonWidget* MoveButton);

	void Clear();

	FButtonClicked BackClicked;
};
