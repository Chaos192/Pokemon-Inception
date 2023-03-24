// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TitleGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FErrorSignature, FString, ErrorString);

UCLASS()
class POKEMONINCEPTION_API ATitleGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	bool bIsNameValid(FString PlayerName);

public:
	FErrorSignature ErrorDelegate;
	FString ErrorMessage;

	UFUNCTION()
	void StartGame();

	UFUNCTION()
	void QuitGame();

	UFUNCTION()
	void ResetGame(int PlaceHolder);

	UFUNCTION()
	void SavePlayerName(FString PlayerName);
};
