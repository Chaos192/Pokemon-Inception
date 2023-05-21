// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "../PokemonUI/PokemonSlotWidget.h"
#include "../PokemonUI/PokemonSummaryWidget.h"
#include "../ButtonClick.h"
#include "PokemonWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UPokemonWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION()
	void OnBackClicked();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* Back;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UWrapBox* WrapBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UWrapBox* InfoWrapBox;

public:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void AddToWrapBox(UPokemonSlotWidget* PokemonWidget);
	UFUNCTION()
	void AddToInfoWrapBox(UPokemonSummaryWidget* PokemonSummaryWidget);

	void ClearWrapBox();
	void ClearSummaryBox();

	FButtonClicked BackClicked;
};
