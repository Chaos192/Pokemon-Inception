// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "Components/TextBlock.h"
#include "ItemShopSlotWidget.h"
#include "../ButtonClick.h"
#include "ShopWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UShopWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UFUNCTION()
	void OnExitClicked();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* Exit;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UWrapBox* WrapBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* TextBox;

public:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void DisplayInShop(UItemShopSlotWidget* ItemWidget);

	UFUNCTION()
	void ShowText(FString Message);

	void ClearShop();

	FButtonClicked ExitClicked;
};
