// Fill out your copyright notice in the Description page of Project Settings.


#include "PokemonStorageManager.h"
#include "../../Player/PlayerCharacterController.h"
#include "../../GameModes/OverworldGameMode.h"
#include "../../UI/HUD/OverworldHUD.h"

APokemonStorageManager::APokemonStorageManager()
{
	Name = "Pokemon Storage Manager";

	OverheadNameWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Name"));
	OverheadNameWidget->SetupAttachment(RootComponent);
	OverheadNameWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

void APokemonStorageManager::BeginPlay()
{
	Super::BeginPlay();
	UOverheadNameWidget* Widget = CreateWidget<UOverheadNameWidget>(UGameplayStatics::GetGameInstance(GetWorld()), OverheadNameWidgetClass);

	if (Widget) {
		OverheadNameWidget->SetWidget(Widget);
		OverheadNameWidget->SetRelativeLocation(FVector(10, 0, 125));
		Widget->SetName(Name);
	}
}

void APokemonStorageManager::Interact(APlayerController* PlayerController)
{
	AOverworldHUD* Hud = Cast<AOverworldHUD>(PlayerController->GetHUD());
	if (Hud == nullptr) {
		return;
	}

	Hud->ShowPokemonStorage();
}
