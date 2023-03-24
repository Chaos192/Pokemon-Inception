// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleHUD.h"
#include "../../GameModes/TitleGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/WidgetLayoutLibrary.h"


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
	ResetProgressPopup = CreateWidget<UPopupSelectionWidget>(UGameplayStatics::GetGameInstance(GetWorld()), ResetProgressPopupClass);

	TitleWidget->PlayClicked.AddDynamic(GameMode, &ATitleGameMode::StartGame);
	TitleWidget->ControllsClicked.AddDynamic(this, &ATitleHUD::ShowControlls);
	TitleWidget->QuitClicked.AddDynamic(GameMode, &ATitleGameMode::QuitGame);
	TitleWidget->ResetClicked.AddDynamic(this, &ATitleHUD::ShowResetProgressPopup);

	ControlsWidget->BackClicked.AddDynamic(this, &ATitleHUD::ShowTitle);

	ResetProgressPopup->ActionClicked.AddDynamic(GameMode, &ATitleGameMode::ResetGame);
	ResetProgressPopup->CancelClicked.AddDynamic(this, &ATitleHUD::ShowTitle);

	PlayerNameWidget->PlayClicked.AddDynamic(GameMode, &ATitleGameMode::SavePlayerName);
	GameMode->ErrorDelegate.AddDynamic(PlayerNameWidget, &UPlayerNameWidget::SetErrorText);

	ShowTitle();
}

void ATitleHUD::Clear()
{
	UWidgetLayoutLibrary::RemoveAllWidgets(this);
}

void ATitleHUD::ShowTitle()
{
	Clear();

	if (PlayerOwner && TitleWidget) {
		TitleWidget->AddToViewport();
	}
}

void ATitleHUD::ShowControlls()
{
	Clear();

	if (PlayerOwner && ControlsWidget) {
		ControlsWidget->AddToViewport();
	}
}

void ATitleHUD::ShowPlayerNameInput()
{
	Clear();

	if (PlayerOwner && PlayerNameWidget) {
		PlayerNameWidget->AddToViewport();
	}
}

void ATitleHUD::ShowResetProgressPopup()
{
	if (PlayerOwner && ResetProgressPopup) {
		ResetProgressPopup->AddToViewport();
	}
}
