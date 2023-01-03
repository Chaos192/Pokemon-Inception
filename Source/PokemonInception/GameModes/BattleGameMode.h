// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../Pokemon/PokemonStruct.h"
#include "../SaveGame/WorldSaveData.h"
#include "../UI/OverworldUI/ItemInfoWidget.h"
#include "../UI/WidgetDelegates.h"
#include "BattleGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageSignature, FString, String);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWidgetSignature, UUserWidget*, Widget);

UCLASS()
class POKEMONINCEPTION_API ABattleGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	TArray<FPokemonStruct> OpponentTeam;

	FString Message = "";
	FString TempMessage = "";
	int Iterator = 0;
	UUserWidget* NextWidget;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void BattleEnd();

	FTimerHandle MessageTimer;
	FTimerHandle WidgetDelay;

public:
	UFUNCTION()
	void DisplayMessage(FString MessageToDisplay, UUserWidget* Widget);

	UFUNCTION()
	void IterateMessage();

	UFUNCTION()
	void DisplayNextWidget();

	UFUNCTION()
	void ShowPokemonInMenu();

	UFUNCTION()
	void FillBagWidget();

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
};
