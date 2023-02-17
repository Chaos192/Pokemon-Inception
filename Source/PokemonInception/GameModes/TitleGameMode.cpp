// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "../UI/HUD/TitleHUD.h"

void ATitleGameMode::StartGame()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	ATitleHUD* Hud = Cast<ATitleHUD>(PlayerController->GetHUD());

	Hud->PlayerOwner->SetInputMode(FInputModeGameOnly());
	Hud->PlayerOwner->bShowMouseCursor = false;

	UGameplayStatics::OpenLevel(GetWorld(), "TestingMap");
}

void ATitleGameMode::QuitGame()
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0)) {
		PlayerController->ConsoleCommand("quit");
	}
}
