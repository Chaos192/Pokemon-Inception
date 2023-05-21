// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "../ButtonClick.h"
#include "ItemInfoWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UItemInfoWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	int ItemID;

	UFUNCTION()
	void OnUseClicked();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* Use;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* DescriptionText;

public:
	virtual void NativeOnInitialized() override;

	void SetID(int InItemID);

	void SetDescription(FText Description);
	void SetUseButton(bool bUsable);

	FElementIDSignature UseClicked;
};
