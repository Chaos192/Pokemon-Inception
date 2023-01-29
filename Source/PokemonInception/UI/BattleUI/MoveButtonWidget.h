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
	
private:
	UFUNCTION()
	void OnButtonClicked();

	int MoveId;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* MoveName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* MovePP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* MoveMaxPP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* MoveButton;

	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void InitButton(FText Name, int CurrPP, int MaxPP, ETypes MoveType);

	UFUNCTION()
	void SetMove(int InMoveId);

	FElementIDSignature ButtonClicked;
};
