// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MenuWidget.h"
#include "PokedexWidget.h"
#include "PokemonWidget.h"
#include "BagWidget.h"
#include "TrainerCardWidget.h"
#include "SaveWidget.h"
#include "SettingsWidget.h"
#include "ShopWidget.h"
#include "ItemSlotWidget.h"
#include "ItemShopSlotWidget.h"
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
	TSubclassOf<USaveWidget> SaveWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USettingsWidget> SettingsWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UShopWidget> ShopWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UItemSlotWidget> ItemSlotWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UItemShopSlotWidget> ItemShopSlotWidgetClass;

	UPROPERTY()
	class UMenuWidget* MenuWidget;

	UPROPERTY()
	class UTextBoxWidget* TextBoxWidget;

	UPROPERTY()
	class UTextBoxWidget* OnScreenMessageWidget;

	UPROPERTY()
	class UPokedexWidget* PokedexWidget;

	UPROPERTY()
	class UPokemonWidget* PokemonWidget;

	UPROPERTY()
	class UBagWidget* BagWidget;

	UPROPERTY()
	class UTrainerCardWidget* TrainerCardWidget;

	UPROPERTY()
	class USaveWidget* SaveWidget;

	UPROPERTY()
	class USettingsWidget* SettingsWidget;

	UPROPERTY()
	class UShopWidget* ShopWidget;

	virtual void BeginPlay()override;
	
public:
	void Clear();

	void ClearOnScreenMessage();

	UFUNCTION()
	void ClearShop();

	TSubclassOf<UItemSlotWidget> GetItemSlotWidgetClass();

	TSubclassOf<UItemShopSlotWidget> GetItemShopSlotWidgetClass();

	UFUNCTION()
	void ShowMenu();

	UFUNCTION()
	void ShowPokedex();

	UFUNCTION()
	void ShowPokemon();

	UFUNCTION()
	void ShowBag();

	UFUNCTION()
	void ShowTrainerCard();

	UFUNCTION()
	void ShowSave();

	UFUNCTION()
	void ShowSettings();

	UFUNCTION()
	void ShowShop();

	UFUNCTION()
	void OnScreenMessage(FString Message);

	UFUNCTION()
	void ShowText(FString Message);

	UFUNCTION()
	void TogglePause(bool IsPaused);

};
