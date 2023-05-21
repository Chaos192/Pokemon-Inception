// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "../ItemUI/ItemSlotWidget.h"
#include "../ItemUI/ItemInfoWidget.h"
#include "../ButtonClick.h"
#include "BagWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UBagWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UFUNCTION()
	void OnBackClicked();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* Back;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* ItemBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UWrapBox* InfoBox;

public:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void AddToItemBox(UItemSlotWidget* ItemWidget);
	UFUNCTION()
	void ShowInfo(UItemInfoWidget* ItemInfoWidget);
	
	void ClearItemBox();
	void ClearInfoBox();

	FButtonClicked BackClicked;
};
