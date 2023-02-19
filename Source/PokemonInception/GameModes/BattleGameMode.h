// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../Player/Camera/BattleCamera.h"
#include "../Pokemon/PokemonStruct.h"
#include "../Pokemon/TypeStruct.h"
#include "../Pokemon/MoveBaseStruct.h"
#include "../SaveGame/WorldSaveData.h"
#include "../UI/ItemUI/ItemInfoWidget.h"
#include "../UI/WidgetDelegates.h"
#include "../Items/BallActor.h"
#include "../Interactables/Trainer.h"
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
	float CurrentBattleTime = 0;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATrainer> TrainerClass;
	ATrainer* TrainerActor = nullptr;

	bool bIsOpponentTrainer;

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

	ABallActor* ThrownBallActor = nullptr;

	UFUNCTION()
	void PlacePlayerPokemon();

	UFUNCTION()
	void PlaceOpponentPokemon();

	int SelectedMoveID;
	int SelectedPokemonID;
	int SelectedItemID;

	UFUNCTION()
	void UseMove(int MoveId, FString AttackerContextString);

	UFUNCTION()
	void MoveOutcome(FString MoveMessage, FString CameraContextString);

	UFUNCTION()
	void UseItem();

	UFUNCTION()
	void UseBall();

	UFUNCTION()
	void CatchSuccess();

	UFUNCTION()
	void CatchFail();

	UFUNCTION()
	void OpponentFaints();

	UFUNCTION()
	void PlayerFaints();

	UFUNCTION()
	int SelectOpponentMove();

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

	ABattleCamera* SceneCamera;
	ABattleCamera* PlayerCamera;
	ABattleCamera* OpponentCamera;

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* AttackMovesDT;

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* StatusMovesDT;

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* TypesDT;

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* BallsDT;

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* PotionsDT;

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* EthersDT;

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* RevivesDT;

public:
	bool bHasSelectedItem;
	bool bHasSelectedEther;

	UFUNCTION()
	void ResetCamera();

	UFUNCTION()
	void SwitchToPlayerCamera();

	UFUNCTION()
	void SwitchToOpponentCamera();

	UFUNCTION()
	FString ETypeToString(ETypes Type);

	UFUNCTION()
	void ShowPokemonInMenu();

	UFUNCTION()
	void SelectMove(int MoveId);

	UFUNCTION()
	void SelectPokemon(int InId);

	UFUNCTION()
	void SelectItem(int InId);

	UFUNCTION()
	void SelectPokemonToUseItem(int InId);

	UFUNCTION()
	void SelectMoveToUseItem(int InId);

	UFUNCTION()
	void FillBagWidget();

	UFUNCTION()
	void ShowPokemonMoves();

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

	FPokemonSlotSignature PokemonSlotDelegate;

	FMoveSignature MoveDelegate;
};
