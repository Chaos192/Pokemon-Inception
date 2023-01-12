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
	
private:
	UFUNCTION()
	void OnSlotClicked();

	FPokemonBaseStruct Data;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* SlotButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UImage* PokemonImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* PokemonID;

public:
	UFUNCTION()
	void InitEmptySlot(FName ID);

	UFUNCTION()
	void InitFilledSlot(FPokemonBaseStruct PokedexData);
	
	FPokedexSignature SlotClicked;
};
