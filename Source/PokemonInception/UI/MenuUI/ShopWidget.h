// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "../ItemUI/ItemSlotWidget.h"
#include "../ItemUI/ItemShopInfoWidget.h"
#include "../ButtonClick.h"
#include "ShopWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UShopWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UFUNCTION()
	void OnExitClicked();

	UFUNCTION()
	void OnBuyModeClicked();

	UFUNCTION()
	void OnSellModeClicked();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* Exit;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* BuyMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* SellMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* ItemBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UWrapBox* InfoBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* Money;

public:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void DisplayInShop(UItemSlotWidget* ItemWidget);
	UFUNCTION()
	void ShowShopInfo(UItemShopInfoWidget* InfoWidget);

	void ShowMoney(FString Message);

	void ClearShop();
	void ClearShopInfo();

	FButtonClicked ExitClicked;
	FButtonClicked BuyModeClicked;
	FButtonClicked SellModeClicked;
};
