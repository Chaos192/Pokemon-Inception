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

private:
	int ItemID;

	UFUNCTION()
	void OnUseClicked();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* Use;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* DescriptionText;

public:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void SetDescription(FText Description);

	UFUNCTION()
	void SetID(int InItemID);

	UFUNCTION()
	void SetUseButton(bool bUsable);

	FElementIDSignature UseClicked;
};
