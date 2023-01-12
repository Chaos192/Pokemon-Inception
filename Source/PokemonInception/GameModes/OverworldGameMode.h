// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/DataTable.h"
#include "../UI/OverworldUI/ShopWidget.h"
#include "../UI/OverworldUI/ItemShopSlotWidget.h"
#include "../UI/OverworldUI/ItemInfoWidget.h"
#include "../UI/WidgetDelegates.h"
#include "../SaveGame/WorldSaveData.h"
#include "OverworldGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndTextSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTextSignature, FString, String);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGamePauseSignature, bool, bIsPaused);

UCLASS(minimalapi)
class AOverworldGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SaveGame();

	UFUNCTION()
	void MarkActorAsDestroyed(AActor* Actor);

	UFUNCTION()
	void SaveOpponent(FPokemonStruct Opponent);

	UFUNCTION()
	void SaveAndExit();

	UFUNCTION()
	void OnScreenMessage(FString MessageToDisplay);

	UFUNCTION()
	void EndOnScreenMessage();

	UFUNCTION()
	void FillBagWidget();

	UFUNCTION()
	void ShowItemInfo(FItemBaseStruct InventoryItem);

	UFUNCTION()
	void InitShop(TArray<FName> ItemsToSell);

	UFUNCTION()
	void RefreshShop();

	UFUNCTION()
	void ShowPokemonInMenu();

	UFUNCTION()
	void ShowPokemonSummary(FPokemonStruct Pokemon);

	UFUNCTION()
	void FillPokedex();

	TArray<class UDataTable*> GetItemDT() const;

	TArray<class UDataTable*> GetMoveDT() const;

	FEndTextSignature RemoveText;

	FGamePauseSignature OnGamePaused;

	FTextSignature MessageUpdate;
	FTextSignature OnScreenMessageDelegate;
	FTextSignature ShopMessageDelegate;

	FItemSlotSignature ItemSlotDelegate;
	FShopSlotSignature ItemShopSlotDelegate;
	FItemInfoSignature ItemInfoDelegate;

	FPokedexSlotSignature PokedexSlotDelegate;

	FPokemonSlotSignature PokemonSlotDelegate;
	FPokemonSummarySignature PokemonSummaryDelegate;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	FString ETypeToString(ETypes Type);

	UFUNCTION()
	void BuyItem(FItemBaseStruct Item);

	UFUNCTION()
	void SellItem(FItemBaseStruct Item);

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* PokemonDT;

	UPROPERTY(EditDefaultsOnly)
	TArray<class UDataTable*> ItemDT;

	UPROPERTY(EditDefaultsOnly)
	TArray<class UDataTable*> MoveDT;
	
	FTimerHandle MessageTimer;
	FTimerHandle ScreenMessageTimer;

private:
	UFUNCTION()
	void TogglePause();

	bool bIsPaused = false;

	TArray<AActor*> ActorsToDestroy;

	TArray<UItemShopSlotWidget*> ShopSlots;
};



