// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/WrapBox.h"
#include "MoveButtonWidget.h"
#include "../../Pokemon/TypeStruct.h"
#include "PokemonSummaryWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UPokemonSummaryWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UImage* PokemonImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* PokemonName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* PokemonID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* PokemonType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* PokemonLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* EXPRequired;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* PokemonHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* PokemonAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* PokemonDefence;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* PokemonSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UWrapBox* MoveWrapBox;

public:
	UFUNCTION()
	void SetImage(UTexture2D* Image);

	UFUNCTION()
	void SetGeneralInfo(FText Name, FName Id, FString Type, int Level, int RequiredEXP);

	UFUNCTION()
	void SetStats(FString HP, int Attack, int Defence, int Speed);

	UFUNCTION()
	void AddMove(UMoveButtonWidget* Move);

	UFUNCTION()
	void ClearMoves();
};
