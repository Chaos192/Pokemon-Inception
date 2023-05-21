// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "../ButtonClick.h"
#include "../../Items/ItemBaseStruct.h"
#include "ItemSlotWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	int ItemID;

	UFUNCTION()
	void OnItemClicked();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* ItemButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UImage* ItemImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ItemCount;

public:
	virtual void NativeOnInitialized() override;

	void SetItemID(int InID);

	void SetItemName(FText Name);
	void SetItemImage(UTexture2D* Image);
	void SetItemCount(int Count);

	FElementIDSignature ItemClicked;
};
