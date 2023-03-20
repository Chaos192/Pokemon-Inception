// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "PickupMessageWidget.h"
#include "InGameWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UInGameWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* Message;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UWrapBox* PickupMessageBox;

public:
	UFUNCTION()
	void SetMessage(FString InMessage);

	void ShowPickupMessage(UPickupMessageWidget* PickupMessage);
	void ClearPickupMessages();
};
