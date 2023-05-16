// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "../MenuUI/MenuWidget.h"
#include "../MenuUI/PokedexWidget.h"
#include "../MenuUI/PokemonWidget.h"
#include "../MenuUI/BagWidget.h"
#include "../MenuUI/TrainerCardWidget.h"
#include "../MenuUI/SettingsWidget.h"
#include "../MenuUI/ShopWidget.h"
#include "../MenuUI/PokemonStorageWidget.h"
#include "../MenuUI/MoveManagerWidget.h"
#include "../MenuUI/GuideMenuWidget.h"
#include "../PokedexUI/PokedexSlotWidget.h"
#include "../PokedexUI/PokedexInfoWidget.h"
#include "../ItemUI/ItemSlotWidget.h"
#include "../ItemUI/ItemShopInfoWidget.h"
#include "../PokemonUI/PokemonSlotWidget.h"
#include "../PopupUI/StorageOperationPopup.h"
#include "../PopupUI/MoveSelectionPopupWidget.h"
#include "../PopupUI/PopupSelectionWidget.h"
#include "../MessageUI/TextBoxWidget.h"
#include "../MessageUI/InGameWidget.h"
#include "../GuideUI/GuideTopicWidget.h"

#include "OverworldHUD.generated.h"


UCLASS()
class POKEMONINCEPTION_API AOverworldHUD : public AHUD
{
	GENERATED_BODY()

protected:
	FString SelectedPokemonLocation;

	FVector2D MousePossition;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMenuWidget> MenuWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTextBoxWidget> TextBoxWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTextBoxWidget> OnScreenMessageWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTextBoxWidget> AreaMessageWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPickupMessageWidget> PickupMessageWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInGameWidget> InGameWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPokedexWidget> PokedexWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPokemonWidget> PokemonWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPopupSelectionWidget> SwapPositionWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UBagWidget> BagWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTrainerCardWidget> TrainerCardWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USettingsWidget> SettingsWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UShopWidget> ShopWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UItemSlotWidget> ItemSlotWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UItemInfoWidget> ItemInfoWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UItemShopInfoWidget> ItemShopInfoWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPopupSelectionWidget> UseItemWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMoveSelectionPopupWidget> MoveSelectionPopupWidgetClass;

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

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPokemonStorageWidget> PokemonStorageWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPokemonIconWidget> PokemonIconWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UStorageOperationPopup> StorageOperationPopupClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPopupSelectionWidget> ReleaseConfirmPopupClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMoveManagerWidget> MoveManagerWidgetClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMoveInfoWidget> MoveInfoWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPopupSelectionWidget> MoveManagerOperationWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGuideMenuWidget> GuideMenuWidgetClass;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UGuideTopicWidget>> GuideTopicWidgetClasses;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ButtonBlockerClass;

	UPROPERTY()
	class UMenuWidget* MenuWidget;

	UPROPERTY()
	class UTextBoxWidget* TextBoxWidget;

	UPROPERTY()
	class UTextBoxWidget* OnScreenMessageWidget;

	UPROPERTY()
	class UTextBoxWidget* AreaMessageWidget;

	UPROPERTY()
	class UInGameWidget* InGameWidget;

	UPROPERTY()
	class UPokedexWidget* PokedexWidget;

	UPROPERTY()
	class UPokedexInfoWidget* PokedexInfoWidget;

	UPROPERTY()
	class UPokemonWidget* PokemonWidget;

	UPROPERTY()
	class UPopupSelectionWidget* SwapPositionWidget;

	UPROPERTY()
	class UPokemonSummaryWidget* PokemonSummaryWidget;

	UPROPERTY()
	class UBagWidget* BagWidget;

	UPROPERTY()
	class UItemInfoWidget* ItemInfoWidget;

	UPROPERTY()
	class UItemShopInfoWidget* ItemShopInfoWidget;

	UPROPERTY()
	class UPopupSelectionWidget* UseItemWidget;

	UPROPERTY()
	class UMoveSelectionPopupWidget* MoveSelectionPopupWidget;

	UPROPERTY()
	class UTrainerCardWidget* TrainerCardWidget;

	UPROPERTY()
	class USettingsWidget* SettingsWidget;

	UPROPERTY()
	class UShopWidget* ShopWidget;

	UPROPERTY()
	class UPokemonStorageWidget* PokemonStorageWidget;

	UPROPERTY()
	class UStorageOperationPopup* StorageOperationPopupWidget;

	UPROPERTY()
	class UPopupSelectionWidget* ReleaseConfirmPopup;

	UPROPERTY()
	class UMoveManagerWidget* MoveManagerWidget;

	UPROPERTY()
	class UMoveInfoWidget* MoveInfoWidget;

	UPROPERTY()
	class UPopupSelectionWidget* MoveManagerOperationWidget;

	UPROPERTY()
	class UGuideMenuWidget* GuideMenuWidget;

	UPROPERTY()
	TArray<class UGuideTopicWidget*> GuideTopicWidgets;

	UPROPERTY()
	class UUserWidget* ButtonBlocker;

	virtual void BeginPlay()override;
	
public:
	void Clear();

	UFUNCTION()
	void ClearOnScreenMessage();

	UFUNCTION()
	void ClearPokemonSlots();

	UFUNCTION()
	void ClearPokemonIcons();

	UFUNCTION()
	void ClearAndUnpause();

	UFUNCTION()
	void ClearPopup();

	UFUNCTION()
	void ClearMovePopup();

	bool BIsMovePopupInViewport();

	TSubclassOf<UItemSlotWidget> GetItemSlotWidgetClass();

	TSubclassOf<UPokedexSlotWidget> GetPokedexSlotWidgetClass();

	TSubclassOf<UPokemonSlotWidget> GetPokemonSlotWidgetClass();

	TSubclassOf<UMoveButtonWidget> GetMoveButtonWidgetClass();

	TSubclassOf<UPokemonIconWidget> GetPokemonIconWidgetClass();

	UFUNCTION()
	void ShowInGameWidget();

	UFUNCTION()
	void ShowMenu();

	UFUNCTION()
	void ShowPokedex();

	UFUNCTION()
	void ShowPokedexInfo(FName PokemonID);

	UFUNCTION()
	void ShowPokemon();

	UFUNCTION()
	void ShowSwapPositionPopup(int PokemonId);

	UFUNCTION()
	void ShowPokemonSummary(int PokemonID);

	UFUNCTION()
	void ShowBag();

	UFUNCTION()
	void ShowItemInfo(int ItemID);

	UFUNCTION()
	void ShowItemShopInfo(int ItemID);

	UFUNCTION()
	void ShowUseItemPopup(int PokemonId);

	UFUNCTION()
	void ShowMoveSelectionPopup(int PokemonId);

	UFUNCTION()
	void ShowTrainerCard();

	UFUNCTION()
	void ShowSettings();

	UFUNCTION()
	void ShowBuyShop();

	UFUNCTION()
	void ShowSellShop();

	UFUNCTION()
	void ShowPokemonStorage();

	UFUNCTION()
	void ShowWithdrawPopup(int PokemonID);

	UFUNCTION()
	void ShowDepositPopup(int PokemonID);

	UFUNCTION()
	void ShowReleaseConfirmPopup(int PokemonID);

	UFUNCTION()
	void ShowMoveManager(int PokemonID);

	UFUNCTION()
	void ShowMoveInfo(int MoveID);

	UFUNCTION()
	void ShowLearnPopup(int MoveID);

	UFUNCTION()
	void ShowForgetPopup(int MoveID);

	UFUNCTION()
	void ShowGuideMenu();

	UFUNCTION()
	void ShowGuideTopic(int TopicID);

	UFUNCTION()
	void OnScreenMessage(FString Message);

	UFUNCTION()
	void ShowText(FString Message);

	UFUNCTION()
	void ShowAreaName(FString AreaName);

	UFUNCTION()
	void ShowPickupMessage(FString Message, UTexture2D* Image);

	UFUNCTION()
	void ToggleMainMenu(bool IsPaused);

};
