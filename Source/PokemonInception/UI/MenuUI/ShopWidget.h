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
	
private:
	UFUNCTION()
	void OnExitClicked();

	UFUNCTION()
	void OnBuyModeClicked();

	UFUNCTION()
	void OnSellModeClicked();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* Exit;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* BuyMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* SellMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UScrollBox* ItemBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UWrapBox* InfoBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* Money;

public:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void DisplayInShop(UItemSlotWidget* ItemWidget);

	UFUNCTION()
	void ShowShopInfo(UItemShopInfoWidget* InfoWidget);

	UFUNCTION()
	void ShowMoney(FString Message);

	void ClearShop();
	void ClearShopInfo();

	FButtonClicked ExitClicked;
	FButtonClicked BuyModeClicked;
	FButtonClicked SellModeClicked;
};
