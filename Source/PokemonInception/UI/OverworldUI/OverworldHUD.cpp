// Fill out your copyright notice in the Description page of Project Settings.


#include "OverworldHUD.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "../../GameModes/OverworldGameMode.h"
#include "Engine/Engine.h"
#include "../../Player/PokemonInceptionCharacter.h"
#include "../../Player/PlayerCharacterController.h"


void AOverworldHUD::BeginPlay()
{
	Super::BeginPlay();
	
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	MenuWidget = CreateWidget<UMenuWidget>(UGameplayStatics::GetGameInstance(GetWorld()), MenuWidgetClass);
	TextBoxWidget = CreateWidget<UTextBoxWidget>(UGameplayStatics::GetGameInstance(GetWorld()), TextBoxWidgetClass);
	OnScreenMessageWidget = CreateWidget<UTextBoxWidget>(UGameplayStatics::GetGameInstance(GetWorld()), OnScreenMessageWidgetClass);
	PokedexWidget = CreateWidget<UPokedexWidget>(UGameplayStatics::GetGameInstance(GetWorld()), PokedexWidgetClass);
	PokemonWidget = CreateWidget<UPokemonWidget>(UGameplayStatics::GetGameInstance(GetWorld()), PokemonWidgetClass);
	BagWidget = CreateWidget<UBagWidget>(UGameplayStatics::GetGameInstance(GetWorld()), BagWidgetClass);
	TrainerCardWidget = CreateWidget<UTrainerCardWidget>(UGameplayStatics::GetGameInstance(GetWorld()), TrainerCardWidgetClass);
	SaveWidget = CreateWidget<USaveWidget>(UGameplayStatics::GetGameInstance(GetWorld()), SaveWidgetClass);
	SettingsWidget = CreateWidget<USettingsWidget>(UGameplayStatics::GetGameInstance(GetWorld()), SettingsWidgetClass);

	GameMode->MessageUpdate.AddDynamic(TextBoxWidget, &UTextBoxWidget::ReturnMessage);
	GameMode->OnScreenMessageDelegate.AddDynamic(OnScreenMessageWidget, &UTextBoxWidget::ReturnMessage);

	MenuWidget->PokedexClicked.AddDynamic(this, &AOverworldHUD::ShowPokedex);
	MenuWidget->PokemonClicked.AddDynamic(this, &AOverworldHUD::ShowPokemon);
	MenuWidget->BagClicked.AddDynamic(this, &AOverworldHUD::ShowBag);
	MenuWidget->TrainerCardClicked.AddDynamic(this, &AOverworldHUD::ShowTrainerCard);
	MenuWidget->SaveClicked.AddDynamic(this, &AOverworldHUD::ShowSave);
	MenuWidget->SettingsClicked.AddDynamic(this, &AOverworldHUD::ShowSettings);

	PokedexWidget->BackClicked.AddDynamic(this, &AOverworldHUD::ShowMenu);
	PokemonWidget->BackClicked.AddDynamic(this, &AOverworldHUD::ShowMenu);
	BagWidget->BackClicked.AddDynamic(this, &AOverworldHUD::ShowMenu);
	TrainerCardWidget->BackClicked.AddDynamic(this, &AOverworldHUD::ShowMenu);
	SaveWidget->BackClicked.AddDynamic(this, &AOverworldHUD::ShowMenu);
	SettingsWidget->BackClicked.AddDynamic(this, &AOverworldHUD::ShowMenu);

}

void AOverworldHUD::ShowMenu()
{
	Clear();

	if (PlayerOwner && MenuWidget) {
		MenuWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeGameAndUI());
	}
}

void AOverworldHUD::ShowPokedex()
{
	Clear();

	if (PlayerOwner && PokedexWidget) {
		PokedexWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
	}
}

void AOverworldHUD::ShowPokemon()
{
	Clear();

	if (PlayerOwner && PokemonWidget) {
		PokemonWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
	}
}

void AOverworldHUD::ShowBag()
{
	Clear();

	if (PlayerOwner && BagWidget) {
		BagWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
	}
}

void AOverworldHUD::ShowTrainerCard()
{
	Clear();

	if (PlayerOwner && TrainerCardWidget) {
		TrainerCardWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
	}
}

void AOverworldHUD::ShowSave()
{
	Clear();

	if (PlayerOwner && SaveWidget) {
		SaveWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
	}
}

void AOverworldHUD::ShowSettings()
{
	Clear();

	if (PlayerOwner && SettingsWidget) {
		SettingsWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
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

void AOverworldHUD::ClearOnScreenMessage()
{
	OnScreenMessageWidget->RemoveFromViewport();
}
