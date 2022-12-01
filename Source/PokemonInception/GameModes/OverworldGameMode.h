// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/DataTable.h"
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

	class UDataTable* GetItemDT();

	FEndTextSignature RemoveText;
	FGamePauseSignature OnGamePaused;
	FTextSignature MessageUpdate;
	FTextSignature OnScreenMessageDelegate;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	class UDataTable* ItemDT;
	
	FTimerHandle MessageTimer;

private:
	UFUNCTION()
	void TogglePause();

	bool bIsPaused = false;

	FString Message = "";
	FString TempMessage = "";
	int Iterator = 0;
};



