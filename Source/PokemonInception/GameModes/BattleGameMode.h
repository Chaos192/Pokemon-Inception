// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../Player/Camera/BattleCamera.h"
#include "../Pokemon/PokemonStruct.h"
#include "../Pokemon/TypeStruct.h"
#include "../Pokemon/MoveBaseStruct.h"
#include "../SaveGame/WorldSaveData.h"
#include "../UI/OverworldUI/ItemInfoWidget.h"
#include "../UI/WidgetDelegates.h"
#include "BattleGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageSignature, FString, String);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWidgetSignature, UUserWidget*, Widget);

UENUM()
enum class EAction {
	UseMove,
	UsePokeball,
	UseHealingItem,
	SwitchOut
};

UCLASS()
class POKEMONINCEPTION_API ABattleGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	int CurrentAction = 0;
	bool bHasBattleEnded = false;
	bool bIsBattleVictory = false;
	bool bDoesPlayerHaveToSwitch = false;
	bool bHasTurnEnded;

	TArray<FPokemonStruct> OpponentTeam;
	int GetCurrentOpponent();
	bool bIsOpponentDefeated();

	int PlayerPokemonId;
	int OpponentPokemonId;

	AStaticOverworldPokemon* PlayerPokemonActor = nullptr;
	AStaticOverworldPokemon* OpponentPokemonActor = nullptr;

	UFUNCTION()
	void PlacePlayerPokemon(int PokemonId);

	UFUNCTION()
	void PlaceOpponentPokemon(int PokemonId);

	int SelectedMoveID;
	int SwitchedPokemonID;

	UFUNCTION()
	void UseMove(int MoveId, FString AttackerContextString);

	UFUNCTION()
	void MoveOutcome(FString MoveMessage);

	UFUNCTION()
	void OpponentFaints();

	UFUNCTION()
	void PlayerFaints();

	UFUNCTION()
	int SelectOpponentMove();

	FGameMapData SavedGameMapData;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void BattleStart();

	UFUNCTION()
	void BattleEnd();

	UFUNCTION()
	void BattleTurn(EAction PlayerAction);

	UFUNCTION()
	void EndTurn();

	UFUNCTION()
	void ExitBattleMap();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABattleCamera> Camera;

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* AttackMovesDT;

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* StatusMovesDT;

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* TypesDT;

	FTimerHandle MessageTimer;
	FTimerHandle MessageTimer2;
	FTimerHandle MessageTimer3;
	FTimerHandle MessageTimer4;
	FTimerHandle MessageTimer5;
	FTimerHandle WidgetDelay;

public:
	UFUNCTION()
	FString ETypeToString(ETypes Type);

	UFUNCTION()
	void ShowPokemonInMenu();

	UFUNCTION()
	void SelectMove(int MoveId);

	UFUNCTION()
	void SelectPokemon(int InId);

	UFUNCTION()
	void FillBagWidget();

	UFUNCTION()
	void ShowPokemonMoves();

	UFUNCTION()
	void ShowItemInfo(FItemBaseStruct InventoryItem);

	UFUNCTION()
	FPokemonStruct GetCurrentOpponentStruct();

	UFUNCTION()
	int GetPlayerPokemonId();

	UFUNCTION()
	bool HasPlayerRanOutOfPP();

	UFUNCTION()
	bool bHasToSwitchPokemon();

	UFUNCTION()
	void Run();

	FMessageSignature MessageUpdate;
	FWidgetSignature WidgetUpdate;

	FItemSlotSignature ItemSlotDelegate;
	FItemInfoSignature ItemInfoDelegate;

	FPokemonSlotSignature PokemonSlotDelegate;

	FMoveSignature MoveDelegate;
};
