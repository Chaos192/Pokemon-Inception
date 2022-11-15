// Fill out your copyright notice in the Description page of Project Settings.


#include "OverworldHUD.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "../GameModes/OverworldGameMode.h"
#include "Engine/Engine.h"

void AOverworldHUD::BeginPlay()
{
	Super::BeginPlay();
	
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	MenuWidget = CreateWidget<UMenuWidget>(UGameplayStatics::GetGameInstance(GetWorld()), MenuWidgetClass);

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
