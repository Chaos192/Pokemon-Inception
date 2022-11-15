// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattleGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageSignature, FString, String);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWidgetSignature, UUserWidget*, Widget);

UCLASS()
class POKEMONINCEPTION_API ABattleGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	FString Message = "";
	FString TempMessage = "";
	int Iterator = 0;
	UUserWidget* NextWidget;

protected:
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
	void Run();

	FMessageSignature MessageUpdate;
	FWidgetSignature WidgetUpdate;

};
