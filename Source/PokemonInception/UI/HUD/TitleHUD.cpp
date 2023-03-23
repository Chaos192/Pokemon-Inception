// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleHUD.h"
#include "../../GameModes/TitleGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"


void ATitleHUD::BeginPlay()
{
	Super::BeginPlay();
	ATitleGameMode* GameMode = Cast<ATitleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(GameMode)) {
		return;
	}

	PlayerOwner->SetInputMode(FInputModeUIOnly());
	PlayerOwner->bShowMouseCursor = true;

	TitleWidget = CreateWidget<UTitleWidget>(UGameplayStatics::GetGameInstance(GetWorld()), TitleWidgetClass);
	ControlsWidget = CreateWidget<UControlsWidget>(UGameplayStatics::GetGameInstance(GetWorld()), ControlsWidgetClass);
	PlayerNameWidget = CreateWidget<UPlayerNameWidget>(UGameplayStatics::GetGameInstance(GetWorld()), PlayerNameWidgetClass);

	TitleWidget->PlayClicked.AddDynamic(GameMode, &ATitleGameMode::StartGame);
	TitleWidget->ControllsClicked.AddDynamic(this, &ATitleHUD::ShowControlls);
	TitleWidget->QuitClicked.AddDynamic(GameMode, &ATitleGameMode::QuitGame);
	TitleWidget->ResetClicked.AddDynamic(GameMode, &ATitleGameMode::ResetGame);

	ControlsWidget->BackClicked.AddDynamic(this, &ATitleHUD::ShowTitle);

	PlayerNameWidget->PlayClicked.AddDynamic(GameMode, &ATitleGameMode::SavePlayerName);
	GameMode->ErrorDelegate.AddDynamic(PlayerNameWidget, &UPlayerNameWidget::SetErrorText);

	ShowTitle();
}

void ATitleHUD::ShowTitle()
{
	ControlsWidget->RemoveFromViewport();

	if (PlayerOwner && TitleWidget) {
		TitleWidget->AddToViewport();
	}
}

void ATitleHUD::ShowControlls()
{
	TitleWidget->RemoveFromViewport();

	if (PlayerOwner && ControlsWidget) {
		ControlsWidget->AddToViewport();
	}
}

void ATitleHUD::ShowPlayerNameInput()
{
	TitleWidget->RemoveFromViewport();

	if (PlayerOwner && PlayerNameWidget) {
		PlayerNameWidget->AddToViewport();
	}
}
