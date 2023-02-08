// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "../BattleUI/PokemonSlotWidget.h"
#include "../BattleUI/PokemonSummaryWidget.h"
#include "../ButtonClick.h"
#include "PokemonWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UPokemonWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UFUNCTION()
	void OnBackClicked();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* Back;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UWrapBox* WrapBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UWrapBox* InfoWrapBox;

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
