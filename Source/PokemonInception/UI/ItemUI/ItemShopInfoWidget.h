// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "../ButtonClick.h"
#include "ItemShopInfoWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UItemShopInfoWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	int ItemID;

	UFUNCTION()
	void OnOperationClicked();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* Operation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* OperationText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* DescriptionText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* PriceText;

public:
	virtual void NativeOnInitialized() override;

	void SetID(int InItemID);

	void SetOperation(FString OperationName);
	void SetDescription(FText Description);
	void SetPrice(int Price);

	FElementIDSignature OperationClicked;
};
