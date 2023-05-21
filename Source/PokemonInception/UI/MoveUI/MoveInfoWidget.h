// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "../../Pokemon/TypeStruct.h"
#include "MoveInfoWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UMoveInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* MoveName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* MoveType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* MovePower;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* MoveDescription;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UBorder* WidgetBorder;

public:
	void SetMoveName(FText Name);
	void SetMoveType(FText Type);
	void SetMovePower(FText Power);
	void SetMoveDescription(FText Description);

	void SetWidgetColor(ETypes Type);
};
