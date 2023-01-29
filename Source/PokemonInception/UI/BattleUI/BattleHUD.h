// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BattleStartWidget.h"
#include "FightWidget.h"
#include "MoveButtonWidget.h"
#include "PokemonSlotWidget.h"
#include "PokemonStatusWidget.h"
#include "PlayerPokemonStatusWidget.h"
#include "PokemonSummaryWidget.h"
#include "PopupSelectionWidget.h"
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
	TSubclassOf<UPopupSelectionWidget> SwitchOutWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPokemonSummaryWidget> PokemonSummaryWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPokemonStatusWidget> PokemonStatusWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPlayerPokemonStatusWidget> PlayerPokemonStatusWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPokemonStatusWidget> OpponentStatusWidgetClass;

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
	class UPlayerPokemonStatusWidget* PlayerPokemonStatusWidget;

	UPROPERTY()
	class UPokemonStatusWidget* OpponentPokemonStatusWidget;

	UPROPERTY()
	class UPokemonWidget* PokemonWidget;

	UPROPERTY()
	class UPokemonSummaryWidget* PokemonSummaryWidget;

	UPROPERTY()
	class UPopupSelectionWidget* SwitchOutWidget;

	UPROPERTY()
	class UBagWidget* BagWidget;

	UPROPERTY()
	class UTextBoxWidget* TextBoxWidget;

	virtual void BeginPlay() override;
	
public:
	TSubclassOf<UItemSlotWidget> GetItemSlotWidgetClass();

	TSubclassOf<UItemInfoWidget> GetItemInfoWidgetClass();

	TSubclassOf<UPokemonSlotWidget> GetPokemonSlotWidgetClass();

	TSubclassOf<UMoveButtonWidget> GetMoveButtonWidgetClass();

	void Clear();

	UFUNCTION()
	void ClearPopup();

	UFUNCTION()
	void ShowFightWidget();

	UFUNCTION()
	void ShowBag();

	UFUNCTION()
	void ShowPokemon();

	UFUNCTION()
	void ShowPokemonSummary(int PokemonID);

	UFUNCTION()
	void ShowSwitchOutPopup(int PokemonId);

	UFUNCTION()
	void ShowPlayerPokemonStatus();

	UFUNCTION()
	void ShowOpponentPokemonStatus();

	UFUNCTION()
	void RefreshPlayerPokemonStatus();

	UFUNCTION()
	void RefreshOpponentPokemonStatus();

	UFUNCTION()
	void ShowBattleStartWidget();

	UFUNCTION()
	void ShowText(FString Message);

};
