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
	
private:
	UFUNCTION()
	void OnBackClicked();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* Back;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* PlayerName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* SpeciesRegistered;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* PokemonCaught;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* MoneyObtained;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* TrainersDefeated;

public:
	virtual void NativeOnInitialized() override;

	void SetPlayerName(FString Name);

	void SetRegisteredSpecies(int Num);

	void SetPokemonCaught(int Num);

	void SetMoneyObtained(int Num);

	void SetTrainersDefeated(int Num);

	FButtonClicked BackClicked;
};
