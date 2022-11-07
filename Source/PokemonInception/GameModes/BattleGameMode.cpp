// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "../UI/BattleHUD.h"


void ABattleGameMode::Run()
{
	APlayerController* cont = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	ABattleHUD* hud = Cast<ABattleHUD>(cont->GetHUD());

	UGameplayStatics::OpenLevel(GetWorld(), "TestingMap");

	hud->PlayerOwner->SetInputMode(FInputModeGameOnly());
	hud->PlayerOwner->bShowMouseCursor = false;
}

