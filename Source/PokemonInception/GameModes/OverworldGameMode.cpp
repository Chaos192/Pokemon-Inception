// Copyright Epic Games, Inc. All Rights Reserved.

#include "OverworldGameMode.h"
#include "../Player/PokemonInceptionCharacter.h"
#include "../UI/OverworldHUD.h"
#include "GameFramework/Controller.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AOverworldGameMode::AOverworldGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AOverworldGameMode::BeginPlay()
{
	APokemonInceptionCharacter* Player = Cast<APokemonInceptionCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	Player->PauseDelegate.AddDynamic(this, &AOverworldGameMode::TogglePause);
	
	APlayerController* PlayerController = Cast<APlayerController>(Player->Controller);
	OnGamePaused.AddDynamic(Cast<AOverworldHUD>(PlayerController->GetHUD()), &AOverworldHUD::TogglePause);
}

void AOverworldGameMode::Pokedex()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Opened pokedex"));
}

void AOverworldGameMode::Pokemon()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Opened pokemon"));
}

void AOverworldGameMode::Bag()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Opened bag"));
}

void AOverworldGameMode::TrainerCard()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Opened trainer card"));
}

void AOverworldGameMode::Save()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Opened save"));
}

void AOverworldGameMode::Settings()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Opened settings"));
}

void AOverworldGameMode::TogglePause()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	bIsPaused = !bIsPaused;
	PlayerController->SetPause(bIsPaused);
	OnGamePaused.Broadcast(bIsPaused);
}
