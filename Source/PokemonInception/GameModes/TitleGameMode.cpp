// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "../UI/HUD/TitleHUD.h"
#include "../SaveGame/PlayerSaveData.h"
#include "../SaveGame/LevelSaveData.h"

void ATitleGameMode::StartGame()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!IsValid(PlayerController)) {
		return;
	}

	PlayerController->SetInputMode(FInputModeGameOnly());
	PlayerController->bShowMouseCursor = false;

	UGameplayStatics::OpenLevel(GetWorld(), "MainMap");
}

void ATitleGameMode::QuitGame()
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0)) {
		PlayerController->ConsoleCommand("quit");
	}
}

void ATitleGameMode::ResetGame()
{
	if (UGameplayStatics::DoesSaveGameExist("WorldSaveSlot", 0)) {
		UGameplayStatics::DeleteGameInSlot("WorldSaveSlot", 0);
	}
	
	if (UGameplayStatics::DoesSaveGameExist("PlayerSaveSlot", 0)) {
		UGameplayStatics::DeleteGameInSlot("PlayerSaveSlot", 0);
	}
}
