// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "../ButtonClick.h"
#include "PokemonIconWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UPokemonIconWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void OnPokemonClicked();

	int PokemonID;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* PokemonButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UImage* PokemonImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* PokemonName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* PokemonLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* PokemonHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UProgressBar* HPBar;

private:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void SetPokemonName(FText Name);

	UFUNCTION()
	void SetPokemonLevel(int Level);

	UFUNCTION()
	void SetPokemonImage(UTexture2D* Image);

	UFUNCTION()
	void SetPokemonHP(int CurrHP, int MaxHP);

	UFUNCTION()
	void SetPokemon(int InPokemonID);

	FElementIDSignature PokemonClick;

};
