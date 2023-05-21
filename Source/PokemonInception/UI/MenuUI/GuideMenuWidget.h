// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "../ButtonClick.h"
#include "GuideMenuWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UGuideMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UFUNCTION()
	void OnExitClicked();

	UFUNCTION()
	void OnIslandClicked();

	UFUNCTION()
	void OnNPCsClicked();

	UFUNCTION()
	void OnItemsClicked();

	UFUNCTION()
	void OnWildPokemonClicked();

	UFUNCTION()
	void OnBattlesClicked();

	UFUNCTION()
	void OnTypesClicked();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* Island;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* NPCs;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* Items;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* WildPokemon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* Battles;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* Types;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* Exit;

public:
	virtual void NativeOnInitialized() override;

	FButtonClicked ExitClicked;
	FElementIDSignature IslandClicked;
	FElementIDSignature NPCsClicked;
	FElementIDSignature ItemsClicked;
	FElementIDSignature WildPokemonClicked;
	FElementIDSignature BattlesClicked;
	FElementIDSignature TypesClicked;
};
