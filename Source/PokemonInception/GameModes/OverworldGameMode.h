// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OverworldGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndTextSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTextSignature, FString, String);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGamePauseSignature, bool, bIsPaused);

UCLASS(minimalapi)
class AOverworldGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOverworldGameMode();

	UFUNCTION()
	void OnScreenMessage(FString MessageToDisplay);

	UFUNCTION()
	void DisplayMessage(FString MessageToDisplay);

	UFUNCTION()
	void IterateMessage();

	UFUNCTION()
	void EndMessage();

	UFUNCTION()
	void Pokedex();

	UFUNCTION()
	void Pokemon();

	UFUNCTION()
	void Bag();

	UFUNCTION()
	void TrainerCard();

	UFUNCTION()
	void Save();

	UFUNCTION()
	void Settings();

	FEndTextSignature RemoveText;
	FGamePauseSignature OnGamePaused;
	FTextSignature MessageUpdate;
	FTextSignature OnScreenMessageDelegate;

protected:
	virtual void BeginPlay() override;
	
	FTimerHandle MessageTimer;

private:
	UFUNCTION()
	void TogglePause();

	bool bIsPaused = false;

	FString Message = "";
	FString TempMessage = "";
	int Iterator = 0;
};



