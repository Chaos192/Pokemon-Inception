// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OverworldGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGamePauseSignature, bool, bIsPaused);

UCLASS(minimalapi)
class AOverworldGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOverworldGameMode();

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

	FGamePauseSignature OnGamePaused;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void TogglePause();

	bool bIsPaused = false;
};



