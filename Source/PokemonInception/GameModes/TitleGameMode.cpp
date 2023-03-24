// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "../UI/HUD/TitleHUD.h"
#include "../SaveGame/PlayerSaveData.h"
#include "../SaveGame/LevelSaveData.h"

bool ATitleGameMode::bIsNameValid(FString PlayerName)
{
	if (PlayerName.Len() < 1 || PlayerName.Len() > 10) {
		ErrorMessage = "Name must be between 1 and 10 characters!";
		ErrorDelegate.Broadcast(ErrorMessage);
		return false;
	}
	
	if ((PlayerName[0] < 65 || PlayerName[0] > 90) && (PlayerName[0] < 97 || PlayerName[0] > 122)) {
		ErrorMessage = "Name must begin with a letter!";
		ErrorDelegate.Broadcast(ErrorMessage);
		return false;
	}

	return true;
}

void ATitleGameMode::StartGame()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!IsValid(PlayerController)) {
		return;
	}

	if (!UGameplayStatics::DoesSaveGameExist("PlayerSaveSlot", 0)) {
		ATitleHUD* Hud = Cast<ATitleHUD>(PlayerController->GetHUD());
		if (!IsValid(Hud)) {
			return;
		}

		Hud->ShowPlayerNameInput();
	}

	else {
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;

		UGameplayStatics::OpenLevel(GetWorld(), "MainMap");
	}
}

void ATitleGameMode::QuitGame()
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0)) {
		PlayerController->ConsoleCommand("quit");
	}
}

void ATitleGameMode::ResetGame(int PlaceHolder)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!IsValid(PlayerController)) {
		return;
	}

	ATitleHUD* Hud = Cast<ATitleHUD>(PlayerController->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	if (UGameplayStatics::DoesSaveGameExist("WorldSaveSlot", 0)) {
		UGameplayStatics::DeleteGameInSlot("WorldSaveSlot", 0);
	}
	
	if (UGameplayStatics::DoesSaveGameExist("PlayerSaveSlot", 0)) {
		UGameplayStatics::DeleteGameInSlot("PlayerSaveSlot", 0);
	}

	Hud->ShowTitle();
}

void ATitleGameMode::SavePlayerName(FString PlayerName)
{
	if (!bIsNameValid(PlayerName)) {
		return;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!IsValid(PlayerController)) {
		return;
	}

	UPlayerSaveData* SaveData = Cast<UPlayerSaveData>(UGameplayStatics::CreateSaveGameObject(UPlayerSaveData::StaticClass()));
	SaveData->PlayerName = PlayerName;
	UGameplayStatics::SaveGameToSlot(SaveData, "PlayerSaveSlot", 0);

	PlayerController->SetInputMode(FInputModeGameOnly());
	PlayerController->bShowMouseCursor = false;

	UGameplayStatics::OpenLevel(GetWorld(), "MainMap");
}
