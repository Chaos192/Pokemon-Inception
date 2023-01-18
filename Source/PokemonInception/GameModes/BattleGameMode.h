// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../Player/Camera/BattleCamera.h"
#include "../Pokemon/PokemonStruct.h"
#include "../Pokemon/TypeStruct.h"
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
	TArray<FPokemonStruct> OpponentTeam;

	AStaticOverworldPokemon* PlayerPokemonActor = nullptr;
	AStaticOverworldPokemon* OpponentPokemonActor = nullptr;

	FPokemonStruct PlayerPokemon;
	FPokemonStruct OpponentPokemon;

	void PlacePlayerPokemon(FPokemonStruct Pokemon);

	void PlaceOpponentPokemon(FPokemonStruct Pokemon);

	bool bIsOpponentDefeated();

	void UseMove(FPokemonStruct Attacker, FPokemonStruct Opponent, FMoveBaseStruct Move);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void BattleStart();

	UFUNCTION()
	void BattleEnd();

	UFUNCTION()
	void BattleTurn(EAction PlayerAction);

	UFUNCTION()
	FString ETypeToString(ETypes Type);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABattleCamera> Camera;

	FTimerHandle MessageTimer;
	FTimerHandle MessageTimer2;
	FTimerHandle WidgetDelay;

public:
	UFUNCTION()
	void ShowPokemonInMenu();

	UFUNCTION()
	void ShowPokemonSummary(FPokemonStruct Pokemon);

	UFUNCTION()
	void FillBagWidget();

	UFUNCTION()
	void ShowPokemonMoves();

	UFUNCTION()
	void ShowItemInfo(FItemBaseStruct InventoryItem);

	UFUNCTION()
	FPokemonStruct GetCurrentOpponent();

	UFUNCTION()
	void Run();

	FMessageSignature MessageUpdate;
	FWidgetSignature WidgetUpdate;

	FItemSlotSignature ItemSlotDelegate;
	FItemInfoSignature ItemInfoDelegate;

	FPokemonSlotSignature PokemonSlotDelegate;
	FPokemonSummarySignature PokemonSummaryDelegate;

	FMoveSignature MoveDelegate;
};
