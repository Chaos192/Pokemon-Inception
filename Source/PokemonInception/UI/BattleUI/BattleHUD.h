// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BattleStartWidget.h"
#include "FightWidget.h"
#include "MoveButtonWidget.h"
#include "PokemonSlotWidget.h"
#include "PokemonStatusWidget.h"
#include "../TextBoxWidget.h"
#include "../OverworldUI/PokemonWidget.h"
#include "../OverworldUI/BagWidget.h"
#include "../OverworldUI/ItemSlotWidget.h"
#include "../OverworldUI/ItemInfoWidget.h"
#include "BattleHUD.generated.h"


UCLASS()
class POKEMONINCEPTION_API ABattleHUD : public AHUD
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UBattleStartWidget> BattleStartWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UFightWidget> FightWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPokemonWidget> PokemonWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPokemonSlotWidget> PokemonSlotWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPokemonStatusWidget> PokemonStatusWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UBagWidget> BagWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UItemSlotWidget> ItemSlotWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UItemInfoWidget> ItemInfoWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMoveButtonWidget> MoveButtonWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTextBoxWidget> TextBoxWidgetClass;

	UPROPERTY()
	class UBattleStartWidget* BattleStartWidget;

	UPROPERTY()
	class UFightWidget* FightWidget;

	UPROPERTY()
	class UPokemonStatusWidget* PlayerPokemonStatusWidget;

	UPROPERTY()
	class UPokemonWidget* PokemonWidget;

	UPROPERTY()
	class UBagWidget* BagWidget;

	UPROPERTY()
	class UTextBoxWidget* TextBoxWidget;

	virtual void BeginPlay() override;
	
public:
	TSubclassOf<UItemSlotWidget> GetItemSlotWidgetClass();

	TSubclassOf<UItemInfoWidget> GetItemInfoWidgetClass();

	TSubclassOf<UPokemonSlotWidget> GetPokemonSlotWidgetClass();

	void Clear();

	UFUNCTION()
	void ShowFightWidget();

	UFUNCTION()
	void ShowBag();

	UFUNCTION()
	void ShowPokemon();

	UFUNCTION()
	void ShowPlayerPokemonStatus();

	UFUNCTION()
	void RefreshPlayerPokemonStatus();

	UFUNCTION()
	void ShowBattleStartWidget();

	UFUNCTION()
	void ShowWidget(class UUserWidget* Widget);

	UFUNCTION()
	void ShowText(FString Message, class UUserWidget* NextWidget);

};
