// Fill out your copyright notice in the Description page of Project Settings.


#include "OverworldHUD.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "../GameModes/OverworldGameMode.h"
#include "Engine/Engine.h"
#include "../Player/PokemonInceptionCharacter.h"


void AOverworldHUD::BeginPlay()
{
	Super::BeginPlay();
	
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	MenuWidget = CreateWidget<UMenuWidget>(UGameplayStatics::GetGameInstance(GetWorld()), MenuWidgetClass);
	TextBoxWidget = CreateWidget<UTextBoxWidget>(UGameplayStatics::GetGameInstance(GetWorld()), TextBoxWidgetClass);
	OnScreenMessageWidget = CreateWidget<UTextBoxWidget>(UGameplayStatics::GetGameInstance(GetWorld()), OnScreenMessageWidgetClass);

	GameMode->MessageUpdate.AddDynamic(TextBoxWidget, &UTextBoxWidget::ReturnMessage);
	GameMode->OnScreenMessageDelegate.AddDynamic(OnScreenMessageWidget, &UTextBoxWidget::ReturnMessage);

	MenuWidget->PokedexClicked.AddDynamic(GameMode, &AOverworldGameMode::Pokedex);
	MenuWidget->PokemonClicked.AddDynamic(GameMode, &AOverworldGameMode::Pokemon);
	MenuWidget->BagClicked.AddDynamic(GameMode, &AOverworldGameMode::Bag);
	MenuWidget->TrainerCardClicked.AddDynamic(GameMode, &AOverworldGameMode::TrainerCard);
	MenuWidget->SaveClicked.AddDynamic(GameMode, &AOverworldGameMode::Save);
	MenuWidget->SettingsClicked.AddDynamic(GameMode, &AOverworldGameMode::Settings);

}

void AOverworldHUD::ShowMenu()
{
	if (PlayerOwner && MenuWidget) {
		MenuWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeGameAndUI());
	}
}

void AOverworldHUD::OnScreenMessage(FString Message)
{
	Clear();
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (PlayerOwner && OnScreenMessageWidget) {
		OnScreenMessageWidget->AddToViewport();
		PlayerOwner->SetInputMode(FInputModeGameOnly());
		GameMode->OnScreenMessage(Message);
	}
}

void AOverworldHUD::ShowText(FString Message)
{
	Clear();
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (PlayerOwner && TextBoxWidget) {
		TextBoxWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = false;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
		GameMode->DisplayMessage(Message);
	}
}


void AOverworldHUD::TogglePause(bool IsPaused)
{
	if (IsPaused) {
		ShowMenu();
	}
	else {
		Clear();
		PlayerOwner->SetInputMode(FInputModeGameOnly());
		PlayerOwner->bShowMouseCursor = false;
	}
	
}

void AOverworldHUD::Clear()
{
	UWidgetLayoutLibrary::RemoveAllWidgets(this);
}
