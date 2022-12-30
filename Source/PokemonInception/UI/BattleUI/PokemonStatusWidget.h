// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "PokemonStatusWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UPokemonStatusWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* PokemonName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* PokemonLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* PokemonCurrHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* PokemonMaxHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UProgressBar* HPBar;

public:
	UFUNCTION()
	void SetPokemonName(FText Name);

	UFUNCTION()
	void SetPokemonLevel(int Level);

	UFUNCTION()
	void SetPokemonHP(int CurrHP, int MaxHP);
};
