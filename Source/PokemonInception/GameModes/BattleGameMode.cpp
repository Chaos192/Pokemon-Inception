// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "../UI/BattleHUD.h"


void ABattleGameMode::DisplayMessage(FString MessageToDisplay, UUserWidget* Widget)
{
	TempMessage = "";
	Message = MessageToDisplay;
	Iterator = 0;
	NextWidget = Widget;

	GetWorldTimerManager().SetTimer(MessageTimer, this, &ABattleGameMode::IterateMessage, 0.05, true);
}

void ABattleGameMode::IterateMessage()
{
	TempMessage = Message.Mid(0, Iterator);
	Iterator++;
	MessageUpdate.Broadcast(TempMessage);

	if (TempMessage == Message) {
		GetWorldTimerManager().ClearTimer(MessageTimer);
		GetWorldTimerManager().SetTimer(WidgetDelay, this, &ABattleGameMode::DisplayNextWidget, 1, false);
	}
}

void ABattleGameMode::DisplayNextWidget()
{
	WidgetUpdate.Broadcast(NextWidget);
}

void ABattleGameMode::Run()
{
	APlayerController* cont = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	ABattleHUD* hud = Cast<ABattleHUD>(cont->GetHUD());

	UGameplayStatics::OpenLevel(GetWorld(), "TestingMap");

	hud->PlayerOwner->SetInputMode(FInputModeGameOnly());
	hud->PlayerOwner->bShowMouseCursor = false;
}

