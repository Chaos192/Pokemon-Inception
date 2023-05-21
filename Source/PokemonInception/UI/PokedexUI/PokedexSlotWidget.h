// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "../../Pokemon/PokemonBaseStruct.h"
#include "../ButtonClick.h"
#include "PokedexSlotWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UPokedexSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	FName ID;

	UFUNCTION()
	void OnSlotClicked();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* SlotButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UImage* PokemonImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* PokemonID;

public:
	void InitEmptySlot(FName InID);
	void InitFilledSlot(FPokemonBaseStruct PokedexData);
	
	FPokedexIDSignature SlotClicked;
};
