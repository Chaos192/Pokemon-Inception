// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleHUD.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "../../GameModes/BattleGameMode.h"
#include "Engine/Engine.h"

void ABattleHUD::BeginPlay()
{
	Super::BeginPlay();
	
	ABattleGameMode* GameMode = Cast<ABattleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	BattleStartWidget = CreateWidget<UBattleStartWidget>(UGameplayStatics::GetGameInstance(GetWorld()), BattleStartWidgetClass);
	FightWidget = CreateWidget<UFightWidget>(UGameplayStatics::GetGameInstance(GetWorld()), FightWidgetClass);
	TextBoxWidget = CreateWidget<UTextBoxWidget>(UGameplayStatics::GetGameInstance(GetWorld()), TextBoxWidgetClass);

	BattleStartWidget->RunClicked.AddDynamic(GameMode, &ABattleGameMode::Run);
	BattleStartWidget->FightClicked.AddDynamic(this, &ABattleHUD::ShowFightWidget);
	FightWidget->BackClicked.AddDynamic(this, &ABattleHUD::ShowBattleStartWidget);

	GameMode->MessageUpdate.AddDynamic(TextBoxWidget, &UTextBoxWidget::ReturnMessage);
	GameMode->WidgetUpdate.AddDynamic(this, &ABattleHUD::ShowWidget);

	ShowText("A wild Pokemon appeared!", Cast<UUserWidget>(BattleStartWidget));
}

void ABattleHUD::Clear()
{
	UWidgetLayoutLibrary::RemoveAllWidgets(this);
}

void ABattleHUD::ShowWidget(class UUserWidget* Widget)
{
	Clear();

	if (PlayerOwner && Widget) {
		Widget->AddToViewport();
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
	}
}

void ABattleHUD::ShowText(FString Message, class UUserWidget* NextWidget)
{
	Clear();
	ABattleGameMode* GameMode = Cast<ABattleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (PlayerOwner && TextBoxWidget) {
		TextBoxWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = false;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
		GameMode->DisplayMessage(Message, NextWidget);
	}
}

void ABattleHUD::ShowFightWidget()
{
	Clear();

	if (PlayerOwner && FightWidget) {
		FightWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
	}
}

void ABattleHUD::ShowBattleStartWidget()
{
	Clear();

	if (PlayerOwner && BattleStartWidget) {
		BattleStartWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
	}
}
