// Fill out your copyright notice in the Description page of Project Settings.


#include "Healer.h"
#include "../../Player/PlayerCharacterController.h"
#include "../../GameModes/OverworldGameMode.h"
#include "../../UI/HUD/OverworldHUD.h"

AHealer::AHealer()
{
	Name = "Healer";

	OverheadNameWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Name"));
	OverheadNameWidget->SetupAttachment(RootComponent);
	OverheadNameWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

void AHealer::BeginPlay()
{
	Super::BeginPlay();
	UOverheadNameWidget* Widget = CreateWidget<UOverheadNameWidget>(UGameplayStatics::GetGameInstance(GetWorld()), OverheadNameWidgetClass);

	if (Widget) {
		OverheadNameWidget->SetWidget(Widget);
		OverheadNameWidget->SetRelativeLocation(FVector(10, 0, 125));
		Widget->SetName(Name);
	}
}

void AHealer::Interact(APlayerController* PlayerController)
{
	AOverworldHUD* Hud = Cast<AOverworldHUD>(PlayerController->GetHUD());
	if (Hud == nullptr) {
		return;
	}

	APlayerCharacterController* ThePlayerController = Cast<APlayerCharacterController>(PlayerController);

	ThePlayerController->FullRestoreAllPokemon();
	Hud->OnScreenMessage("Your Pokemon were fully healed!");
}
