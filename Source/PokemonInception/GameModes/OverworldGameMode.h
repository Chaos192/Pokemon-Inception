// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/DataTable.h"
#include "../UI/OverworldUI/ShopWidget.h"
#include "../UI/OverworldUI/ItemShopSlotWidget.h"
#include "OverworldGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndTextSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTextSignature, FString, String);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGamePauseSignature, bool, bIsPaused);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemSlotSignature, UItemSlotWidget*, ItemSlot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShopSlotSignature, UItemShopSlotWidget*, ItemShopSlot);

UCLASS(minimalapi)
class AOverworldGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnScreenMessage(FString MessageToDisplay);

	UFUNCTION()
	void DisplayMessage(FString MessageToDisplay);

	UFUNCTION()
	void IterateMessage();

	UFUNCTION()
	void EndMessage();

	UFUNCTION()
	void EndOnScreenMessage();

	UFUNCTION()
	void FillBagWidget();

	UFUNCTION()
	void InitShop(TArray<FName> ItemsToSell);

	UFUNCTION()
	void RefreshShop();

	TArray<class UDataTable*> GetItemDT() const;

	FEndTextSignature RemoveText;

	FGamePauseSignature OnGamePaused;

	FTextSignature MessageUpdate;
	FTextSignature OnScreenMessageDelegate;
	FTextSignature ShopMessageDelegate;

	FItemSlotSignature ItemSlotDelegate;
	FShopSlotSignature ItemShopSlotDelegate;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void BuyItem(FItemBaseStruct Item);

	UFUNCTION()
	void SellItem(FItemBaseStruct Item);

	UPROPERTY(EditDefaultsOnly)
	TArray<class UDataTable*> ItemDT;
	
	FTimerHandle MessageTimer;
	FTimerHandle ScreenMessageTimer;

private:
	UFUNCTION()
	void TogglePause();

	bool bIsPaused = false;

	FString Message = "";
	FString TempMessage = "";
	int Iterator = 0;

	TArray<UItemShopSlotWidget*> ShopSlots;
};



