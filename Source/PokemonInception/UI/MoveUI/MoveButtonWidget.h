// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "../ButtonClick.h"
#include "../../Pokemon/TypeStruct.h"
#include "../../Pokemon/MoveBaseStruct.h"
#include "MoveButtonWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UMoveButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	int MoveId;

	UFUNCTION()
	void OnButtonClicked();

	UFUNCTION()
	void OnButtonHovered();

	UFUNCTION()
	void OnButtonUnHovered();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* MoveName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* MoveEffectiveness;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* MovePP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* MoveButton;

public:
	virtual void NativeOnInitialized() override;

	void SetMove(int InMoveId);

	void InitButton(FText Name, int CurrPP, int MaxPP, ETypes MoveType);
	void SetEffectiveness(FString Effectiveness);

	FElementIDSignature ButtonClicked;
	FElementIDSignature ButtonHovered;
	FButtonClicked ButtonUnHovered;
};
