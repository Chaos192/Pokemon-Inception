// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "../BattleUI/PokemonSlotWidget.h"
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

	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void AddToWrapBox(UPokemonSlotWidget* PokemonWidget);

	void ClearWrapBox();

	FButtonClicked BackClicked;
};
