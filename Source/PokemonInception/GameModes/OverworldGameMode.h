// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/DataTable.h"
#include "../UI/OverworldUI/ShopWidget.h"
#include "../UI/OverworldUI/ItemShopSlotWidget.h"
#include "../UI/OverworldUI/ItemInfoWidget.h"
#include "../UI/WidgetDelegates.h"
#include "OverworldGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndTextSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTextSignature, FString, String);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGamePauseSignature, bool, bIsPaused);

UCLASS(minimalapi)
class AOverworldGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	UFUNCTION()
	void TogglePause();

	UFUNCTION()
	void UseItem();

	UFUNCTION()
	void SwapPositionWith(int NewPositionId);

	int SelectedItemID;
	int SelectedPokemonID;
	int SelectedMoveID;

	bool bIsPaused = false;

	TArray<AActor*> ActorsToDestroy;

	TArray<UItemShopSlotWidget*> ShopSlots;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void BuyItem(FItemBaseStruct Item);

	UFUNCTION()
	void SellItem(FItemBaseStruct Item);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AWildPokemonSpawner> SpawnerClass;

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* BallsDT;

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* PotionsDT;

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* ValuablesDT;

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* EthersDT;

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* RevivesDT;

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* CandyDT;

	UPROPERTY(EditDefaultsOnly)
	TArray<class UDataTable*> MoveDT;

public:
	bool bHasSelectedItem;
	bool bHasSelectedEther;
	bool bIsSwappingPosition;

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* PokemonDT;

	UFUNCTION()
	FString ETypeToString(ETypes Type);

	UFUNCTION()
	void SaveGame();

	UFUNCTION()
	void SaveOpponent(FPokemonStruct Opponent);

	UFUNCTION()
	void SaveWildPokemon(AWildPokemon* PokemonToIgnore);

	UFUNCTION()
	void LoadWildPokemon();

	UFUNCTION()
	void SaveAndExit();

	UFUNCTION()
	void MarkActorAsDestroyed(AActor* Actor);

	UFUNCTION()
	void SelectMove(int InId);

	UFUNCTION()
	void SelectPokemon(int InId);

	UFUNCTION()
	void SelectItem(int InId);

	UFUNCTION()
	void FillBagWidget();

	UFUNCTION()
	void InitShop(TArray<FName> ItemsToSell);

	UFUNCTION()
	void RefreshShop();

	UFUNCTION()
	void ShowPokemonInMenu();

	UFUNCTION()
	void FillPokedex();

	UFUNCTION()
	void ShowPokemonPartyInStorage();

	UFUNCTION()
	void ShowPokemonInStorage();

	FPokemonBaseStruct GetPokemonSpeciesData(FName PokemonID);

	TArray<class UDataTable*> GetItemDT() const;

	TArray<class UDataTable*> GetMoveDT() const;

	FGamePauseSignature OnGamePaused;

	FTextSignature ShopMessageDelegate;

	FItemSlotSignature ItemSlotDelegate;
	FShopSlotSignature ItemShopSlotDelegate;

	FPokedexSlotSignature PokedexSlotDelegate;
	FPokemonSlotSignature PokemonSlotDelegate;
};



