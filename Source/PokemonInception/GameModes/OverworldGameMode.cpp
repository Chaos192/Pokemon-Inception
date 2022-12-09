// Copyright Epic Games, Inc. All Rights Reserved.

#include "OverworldGameMode.h"
#include "../Player/PokemonInceptionCharacter.h"
#include "../UI/OverworldUI/OverworldHUD.h"
#include "GameFramework/Controller.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/WorldSettings.h"
#include "../Player/PlayerCharacterController.h"


void AOverworldGameMode::BeginPlay()
{
	APokemonInceptionCharacter* Player = Cast<APokemonInceptionCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Player == nullptr) {
		return;
	}

	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(Player->Controller);
	if (PlayerController == nullptr) {
		return;
	}
	
	PlayerController->PauseDelegate.AddDynamic(this, &AOverworldGameMode::TogglePause);

	OnGamePaused.AddDynamic(Cast<AOverworldHUD>(PlayerController->GetHUD()), &AOverworldHUD::TogglePause);
}

void AOverworldGameMode::OnScreenMessage(FString MessageToDisplay)
{
	OnScreenMessageDelegate.Broadcast(MessageToDisplay);
	GetWorldTimerManager().SetTimer(ScreenMessageTimer, this, &AOverworldGameMode::EndOnScreenMessage, 1, false);
}

void AOverworldGameMode::DisplayMessage(FString MessageToDisplay)
{
	UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->CustomTimeDilation = 0;
	TempMessage = "";
	Message = MessageToDisplay;
	Iterator = 0;

	GetWorldTimerManager().SetTimer(MessageTimer, this, &AOverworldGameMode::IterateMessage, 0.05, true);
}

void AOverworldGameMode::IterateMessage()
{
	TempMessage = Message.Mid(0, Iterator);
	Iterator++;
	MessageUpdate.Broadcast(TempMessage);

	if (TempMessage == Message) {
		GetWorldTimerManager().ClearTimer(MessageTimer);
		GetWorldTimerManager().SetTimer(MessageTimer, this, &AOverworldGameMode::EndMessage, 1, false);
	}
}

void AOverworldGameMode::EndMessage()
{
	AOverworldHUD* Hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	Hud->PlayerOwner->SetInputMode(FInputModeGameOnly());
	Hud->Clear();

	APokemonInceptionCharacter* Player = Cast<APokemonInceptionCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	Player->CustomTimeDilation = 1;
}

void AOverworldGameMode::EndOnScreenMessage()
{
	AOverworldHUD* Hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (Hud != nullptr) {
		Hud->ClearOnScreenMessage();
	}
}

void AOverworldGameMode::FillBagWidget()
{
	APokemonInceptionCharacter* Player = Cast<APokemonInceptionCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(Player->Controller);

	TArray<FItemBaseStruct> Inventory = PlayerController->GetInventory();

	TArray<FItemBaseStruct> UniqueItems;
	TArray<int> ItemCount;

	for (FItemBaseStruct Item : Inventory) {
		if (UniqueItems.Contains(Item) == false) {
			UniqueItems.Add(Item);
		}
	}

	for (FItemBaseStruct Item : UniqueItems) {
		int Count = 0;

		for (FItemBaseStruct ItemToSearch : Inventory) {
			if (Item == ItemToSearch) {
				Count++;
			}
		}

		ItemCount.Add(Count);
	}

	for (int i = 0; i < UniqueItems.Num(); i++) {
		AOverworldHUD* Hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
		UItemSlotWidget* ItemSlotWidget = CreateWidget<UItemSlotWidget>(UGameplayStatics::GetGameInstance(GetWorld()), Hud->GetItemSlotWidgetClass());

		ItemSlotWidget->SetItemName(UniqueItems[i].Name);
		ItemSlotWidget->SetItemImage(UniqueItems[i].Image);
		ItemSlotWidget->SetItemCount(ItemCount[i]);

		ItemSlotDelegate.Broadcast(ItemSlotWidget);
	}
}

TArray<class UDataTable*> AOverworldGameMode::GetItemDT() const
{
	return ItemDT;
}

void AOverworldGameMode::TogglePause()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	bIsPaused = !bIsPaused;
	PlayerController->SetPause(bIsPaused);
	OnGamePaused.Broadcast(bIsPaused);
}
