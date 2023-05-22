// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Camera/CameraActor.h"
#include "../Pokemon/PokemonStruct.h"
#include "../Pokemon/TypeStruct.h"
#include "../Pokemon/MoveBaseStruct.h"
#include "../UI/ItemUI/ItemInfoWidget.h"
#include "../UI/WidgetDelegates.h"
#include "../Items/BallActor.h"
#include "../Interactables/NPC/Trainer.h"
#include "BattleGameMode.generated.h"


UENUM()
enum class EAction {
	UseMove,
	UsePokeball,
	UseHealingItem,
	SwitchOut
};

UENUM()
enum class EBattler {
	Player,
	Opponent
};

UCLASS()
class POKEMONINCEPTION_API ABattleGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	float CurrentBattleTime = 0;
	UPROPERTY()
	int EXPGained = 0;

	UPROPERTY()
	ATrainer* TrainerActor = nullptr;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATrainer> TrainerClass;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* BGM = nullptr;

	bool bIsOpponentTrainer;
	bool bHasDefeatedTrainerBefore;

	bool bHasBattleEnded = false;
	bool bIsBattleVictory = false;
	bool bDoesPlayerHaveToSwitch = false;
	bool bHasTurnEnded;

	UPROPERTY()
	TArray<FPokemonStruct> OpponentTeam;

	int GetCurrentOpponent();
	bool bIsOpponentDefeated();

	UPROPERTY()
	int PlayerPokemonId;
	UPROPERTY()
	int OpponentPokemonId;

	UPROPERTY()
	AStaticOverworldPokemon* PlayerPokemonActor = nullptr;
	UPROPERTY()
	AStaticOverworldPokemon* OpponentPokemonActor = nullptr;

	UPROPERTY()
	ABallActor* ThrownBallActor = nullptr;

	int SelectedMoveID;
	int SelectedPokemonID;
	int SelectedItemID;

	int OpponentSelectedMoveID;

	UFUNCTION()
	void PlacePlayerPokemon();

	UFUNCTION()
	void PlaceOpponentPokemon();

	UFUNCTION()
	void PlaceOpponentTrainer();

	UFUNCTION()
	void UseMove(int MoveId, EBattler MoveCaster);

	UFUNCTION()
	void MoveOutcome(FString MoveMessage, EBattler MoveCaster, EBattler Target, bool bHasMoveSucceeded);

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
	void OpponentFaintsAftermath();

	UFUNCTION()
	void PlayerFaints();

	UFUNCTION()
	int SelectOpponentMove();

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

	UPROPERTY()
	ACameraActor* SceneCamera;
	UPROPERTY()
	ACameraActor* PlayerPokemonCamera;
	UPROPERTY()
	ACameraActor* OpponentCamera;
	UPROPERTY()
	ACameraActor* PlayerCamera;
	UPROPERTY()
	ACameraActor* TrainerCamera;

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
	UPROPERTY(EditDefaultsOnly)
	class UDataTable* AttackMovesDT;

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* StatusMovesDT;

	bool bHasSelectedItem;
	bool bHasSelectedEther;

	UFUNCTION()
	void SwitchCamera(ACameraActor* Camera, float BlendTime);

	UFUNCTION()
	void ShowMoveSplash(EBattler MoveCaster, EBattler Target);

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

	FString GetMoveEffectiveness(ETypes MoveType);

	bool GetbIsOpponentTrainer();
	FPokemonStruct GetCurrentOpponentStruct();
	TArray<FPokemonStruct> GetOpponentTeam();

	UFUNCTION()
	int GetPlayerPokemonId();

	ACameraActor* GetSceneCamera();

	UFUNCTION()
	bool HasPlayerRanOutOfPP();

	UFUNCTION()
	bool bHasToSwitchPokemon();

	UFUNCTION()
	void Run();

	FItemSlotSignature ItemSlotDelegate;
	FPokemonSlotSignature PokemonSlotDelegate;
	FMoveSignature MoveDelegate;
};
