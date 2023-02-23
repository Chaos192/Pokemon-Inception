// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveTutor.h"
#include "../../GameModes/OverworldGameMode.h"
#include "../../UI/HUD/OverworldHUD.h"

AMoveTutor::AMoveTutor()
{
	Name = "Move Tutor";

	OverheadNameWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Name"));
	OverheadNameWidget->SetupAttachment(RootComponent);
	OverheadNameWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

void AMoveTutor::BeginPlay()
{
	Super::BeginPlay();
	UOverheadNameWidget* Widget = CreateWidget<UOverheadNameWidget>(UGameplayStatics::GetGameInstance(GetWorld()), OverheadNameWidgetClass);

	if (Widget) {
		OverheadNameWidget->SetWidget(Widget);
		OverheadNameWidget->SetRelativeLocation(FVector(10, 0, 125));
		Widget->SetName(Name);
	}
}

void AMoveTutor::Interact(APlayerController* PlayerController)
{
	AOverworldHUD* Hud = Cast<AOverworldHUD>(PlayerController->GetHUD());
	if (Hud == nullptr) {
		return;
	}

	Hud->ShowMoveManager(0);
}
