// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MenuWidget.h"
#include "PokedexWidget.h"
#include "PokedexSlotWidget.h"
#include "PokedexInfoWidget.h"
#include "PokemonWidget.h"
#include "BagWidget.h"
#include "TrainerCardWidget.h"
#include "ShopWidget.h"
#include "ItemSlotWidget.h"
#include "ItemShopSlotWidget.h"
#include "../BattleUI/PokemonSlotWidget.h"
#include "../BattleUI/MoveSelectionPopupWidget.h"
#include "../BattleUI/PopupSelectionWidget.h"
#include "../TextBoxWidget.h"
#include "OverworldHUD.generated.h"


UCLASS()
class POKEMONINCEPTION_API AOverworldHUD : public AHUD
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMenuWidget> MenuWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTextBoxWidget> TextBoxWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTextBoxWidget> OnScreenMessageWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPokedexWidget> PokedexWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPokemonWidget> PokemonWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UBagWidget> BagWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTrainerCardWidget> TrainerCardWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UShopWidget> ShopWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UItemSlotWidget> ItemSlotWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UItemInfoWidget> ItemInfoWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPopupSelectionWidget> UseItemWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMoveSelectionPopupWidget> MoveSelectionPopupWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UItemShopSlotWidget> ItemShopSlotWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPokedexSlotWidget> PokedexSlotWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPokedexInfoWidget> PokedexInfoWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPokemonSlotWidget> PokemonSlotWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPokemonSummaryWidget> PokemonSummaryWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMoveButtonWidget> MoveButtonWidgetClass;

	UPROPERTY()
	class UMenuWidget* MenuWidget;

	UPROPERTY()
	class UTextBoxWidget* TextBoxWidget;

	UPROPERTY()
	class UTextBoxWidget* OnScreenMessageWidget;

	UPROPERTY()
	class UPokedexWidget* PokedexWidget;

	UPROPERTY()
	class UPokedexInfoWidget* PokedexInfoWidget;

	UPROPERTY()
	class UPokemonWidget* PokemonWidget;

	UPROPERTY()
	class UPokemonSummaryWidget* PokemonSummaryWidget;

	UPROPERTY()
	class UBagWidget* BagWidget;

	UPROPERTY()
	class UItemInfoWidget* ItemInfoWidget;

	UPROPERTY()
	class UPopupSelectionWidget* UseItemWidget;

	UPROPERTY()
	class UMoveSelectionPopupWidget* MoveSelectionPopupWidget;

	UPROPERTY()
	class UTrainerCardWidget* TrainerCardWidget;

	UPROPERTY()
	class UShopWidget* ShopWidget;

	virtual void BeginPlay()override;
	
public:
	void Clear();

	UFUNCTION()
	void ClearOnScreenMessage();

	UFUNCTION()
	void ClearShop();

	UFUNCTION()
	void ClearPopup();

	UFUNCTION()
	void ClearMovePopup();

	bool BIsMovePopupInViewport();

	TSubclassOf<UItemSlotWidget> GetItemSlotWidgetClass();

	TSubclassOf<UItemShopSlotWidget> GetItemShopSlotWidgetClass();

	TSubclassOf<UPokedexSlotWidget> GetPokedexSlotWidgetClass();

	TSubclassOf<UPokemonSlotWidget> GetPokemonSlotWidgetClass();

	TSubclassOf<UMoveButtonWidget> GetMoveButtonWidgetClass();

	UFUNCTION()
	void ShowMenu();

	UFUNCTION()
	void ShowPokedex();

	UFUNCTION()
	void ShowPokedexInfo(FName PokemonID);

	UFUNCTION()
	void ShowPokemon();

	UFUNCTION()
	void ShowPokemonSummary(int PokemonID);

	UFUNCTION()
	void ShowBag();

	UFUNCTION()
	void ShowItemInfo(int ItemID);

	UFUNCTION()
	void ShowUseItemPopup(int PokemonId);

	UFUNCTION()
	void ShowMoveSelectionPopup(int PokemonId);

	UFUNCTION()
	void ShowTrainerCard();

	UFUNCTION()
	void ShowShop(TArray<FName> ItemsToSell);

	UFUNCTION()
	void OnScreenMessage(FString Message);

	UFUNCTION()
	void ShowText(FString Message);

	UFUNCTION()
	void TogglePause(bool IsPaused);

};
