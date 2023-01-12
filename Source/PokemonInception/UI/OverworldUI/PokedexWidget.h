// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "PokedexSlotWidget.h"
#include "../ButtonClick.h"
#include "PokedexWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UPokedexWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UFUNCTION()
	void OnBackClicked();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* Back;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UWrapBox* PokedexSlotBox;

	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void AddPokedexSlotToBox(UPokedexSlotWidget* PokedexSlot);

	void ClearSlotBox();

	FButtonClicked BackClicked;
};
