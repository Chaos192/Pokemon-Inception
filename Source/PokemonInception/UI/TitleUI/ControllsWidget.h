// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "../ButtonClick.h"
#include "ControllsWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UControlsWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION()
	void OnBackClicked();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* Back;

public:
	virtual void NativeOnInitialized() override;

	FButtonClicked BackClicked;
};
