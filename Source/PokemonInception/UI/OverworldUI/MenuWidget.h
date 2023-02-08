// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "../ButtonClick.h"
#include "MenuWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UFUNCTION()
	void OnPokedexClicked();

	UFUNCTION()
	void OnPokemonClicked();

	UFUNCTION()
	void OnBagClicked();

	UFUNCTION()
	void OnTrainerCardClicked();

	UFUNCTION()
	void OnSaveClicked();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* Pokedex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* Pokemon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* Bag;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* TrainerCard;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* Save;

public:
	virtual void NativeOnInitialized() override;

	FButtonClicked PokedexClicked;
	FButtonClicked PokemonClicked;
	FButtonClicked BagClicked;
	FButtonClicked TrainerCardClicked;
	FButtonClicked SaveClicked;
};
