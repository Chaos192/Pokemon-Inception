// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "MoveInfoWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UMoveInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* MoveName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* MoveType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* MovePower;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* MoveDescription;

public:
	UFUNCTION()
	void SetMoveName(FText Name);

	UFUNCTION()
	void SetMoveType(FText Type);

	UFUNCTION()
	void SetMovePower(FText Power);

	UFUNCTION()
	void SetMoveDescription(FText Description);
};
