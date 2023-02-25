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

private:
	int ItemID;

	UFUNCTION()
	void OnOperationClicked();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* Operation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* OperationText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* DescriptionText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* PriceText;

public:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void SetOperation(FString OperationName);

	UFUNCTION()
	void SetDescription(FText Description);

	UFUNCTION()
	void SetPrice(int Price);

	UFUNCTION()
	void SetID(int InItemID);

	FElementIDSignature OperationClicked;
};
