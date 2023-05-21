// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "../ButtonClick.h"
#include "PokemonSlotWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UPokemonSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	int PokemonID;

	UFUNCTION()
	void OnPokemonClicked();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* PokemonButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UImage* PokemonImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* PokemonName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* PokemonLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* PokemonHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* HPBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* EXPBar;

public:
	virtual void NativeOnInitialized() override;

	void SetPokemon(int InPokemonID);

	void SetPokemonName(FText Name);
	void SetPokemonLevel(int Level);
	void SetPokemonImage(UTexture2D* Image);
	void SetPokemonHP(int CurrHP, int MaxHP);
	void SetPokemonEXP(int CurrExp, int MaxExp);

	FElementIDSignature PokemonClick;
};
