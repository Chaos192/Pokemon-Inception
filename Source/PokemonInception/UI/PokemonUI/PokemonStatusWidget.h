// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/WrapBox.h"
#include "../../Pokemon/PokemonStruct.h"
#include "PokemonStatusWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UPokemonStatusWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* PokemonName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* PokemonLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* PokemonHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* HPBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UImage* AttackStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UImage* DefenceStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UImage* SpeedStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UWrapBox* PokemonTeamBox;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* AlivePokemonIcon;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* FaintedPokemonIcon;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* EmptyPokemonIcon;

public:
	void SetPokemonName(FText Name);
	void SetPokemonLevel(int Level);
	void SetPokemonHP(int CurrHP, int MaxHP);
	void SetPokemonStatus(TArray<EEffect> Effects);

	void SetPokemonTeamBox(TArray<EPokemonStatus> PokemonStatuses);
	void ClearPokemonTeamBox();
};
