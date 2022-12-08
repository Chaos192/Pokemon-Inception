// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
//#include "../ButtonClick.h"
#include "Blueprint/UserWidget.h"
#include "ItemShopSlotWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UItemShopSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UFUNCTION()
	void OnBuyClicked();

	UFUNCTION()
	void OnSellClicked();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* ItemName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UImage* ItemImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* ItemCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* Buy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* Sell;

	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void SetItemName(FText Name);

	UFUNCTION()
	void SetItemImage(UTexture2D* Image);

	UFUNCTION()
	void SetItemCount(int Count);

	//FButtonClicked BuyClicked;
	//FButtonClicked SellClicked;
};
