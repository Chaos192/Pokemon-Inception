// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "../ButtonClick.h"
#include "TrainerCardWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UTrainerCardWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UFUNCTION()
	void OnBackClicked();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* Back;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* PlayerName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* SpeciesRegistered;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* PokemonCaught;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* MoneyObtained;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* TrainersDefeated;

public:
	virtual void NativeOnInitialized() override;

	void SetPlayerName(FString Name);
	void SetRegisteredSpecies(int Num);
	void SetPokemonCaught(int Num);
	void SetMoneyObtained(int Num);
	void SetTrainersDefeated(int Num);

	FButtonClicked BackClicked;
};
