// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Camera/CameraActor.h"
#include "Engine/DataTable.h"
#include "../UI/WidgetDelegates.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "OverworldGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTextSignature, FString, String);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGamePauseSignature, bool, bIsPaused);

UENUM()
enum class EPause {
	Pause,
	UnPause,
	Auto
};

UCLASS(minimalapi)
class AOverworldGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	bool bIsPaused = false;

	UPROPERTY()
	TArray<AActor*> ActorsToDestroy;

	UPROPERTY()
	ULevelSequencePlayer* SequencePlayer = nullptr;

	UPROPERTY(EditDefaultsOnly)
	ULevelSequence* EncounterSequence = nullptr;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* BGM = nullptr;

	UFUNCTION()
	void ToggleMainMenu();

	UFUNCTION()
	void TogglePeacefulMode();

	UFUNCTION()
	void UseItem();

	UFUNCTION()
	void SwapPositionWith(int NewPositionId);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AWildPokemonSpawner> SpawnerClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ATrainer> TrainerClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACameraActor> CameraClass;

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

public:
	int SelectedItemID;
	int SelectedPokemonID;
	int SelectedMoveID;

	bool bHasSelectedItem;
	bool bHasSelectedEther;
	bool bIsSwappingPosition;

	bool bIsPeacefulModeOn = false;

	UPROPERTY()
	TArray<AWildPokemon*> PokemonInLevel;

	TArray<FItemBaseStruct> ItemsToSell;
	FString ShopMode;

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* PokemonDT;

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* AttackMovesDT;

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* StatusMovesDT;

	UFUNCTION()
	void SaveGame();

	UFUNCTION()
	void InitiateBattle(TArray<FPokemonStruct> OpponentTeam, bool bIsOpponentTrainer, bool bHasDefeatedTrainerBefore);

	UFUNCTION()
	void SaveLevelData(AWildPokemon* PokemonToIgnore);

	UFUNCTION()
	void LoadLevelData();

	UFUNCTION()
	void SaveAndExit();

	UFUNCTION()
	void PauseGame(EPause PauseType);
	bool bIsGamePaused();

	void PlayEncounterSequence();

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
	void InitItemsToSell(TArray<FName> ItemIDs);

	UFUNCTION()
	void InitBuyShop();

	UFUNCTION()
	void InitSellShop();

	UFUNCTION()
	void BuyItem(int ItemID);

	UFUNCTION()
	void SellItem(int ItemID);

	UFUNCTION()
	void ShowPokemonInMenu();

	UFUNCTION()
	void FillPokedex();

	UFUNCTION()
	void ShowPokemonInStorage();

	FPokemonBaseStruct GetPokemonSpeciesData(FName PokemonID);

	TArray<class UDataTable*> GetItemDT() const;

	TArray<class UDataTable*> GetMoveDT() const;

	FGamePauseSignature OnGamePaused;

	FTextSignature ShopMessageDelegate;

	FItemSlotSignature ItemSlotDelegate;
	FItemSlotSignature ItemShopSlotDelegate;

	FPokedexSlotSignature PokedexSlotDelegate;
	FPokemonSlotSignature PokemonSlotDelegate;

	FPokemonIconSignature PartyPokemonIconDelegate;
	FPokemonIconSignature StoragePokemonIconDelegate;
};



