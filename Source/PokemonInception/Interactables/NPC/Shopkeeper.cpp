// Fill out your copyright notice in the Description page of Project Settings.


#include "Shopkeeper.h"
#include "../../Player/PlayerCharacterController.h"
#include "../../GameModes/OverworldGameMode.h"
#include "../../UI/HUD/OverworldHUD.h"

AShopkeeper::AShopkeeper()
{
	Name = "Shopkeeper";

	OverheadNameWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Name"));
	OverheadNameWidget->SetupAttachment(RootComponent);
	OverheadNameWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

void AShopkeeper::BeginPlay()
{
	Super::BeginPlay();
	UOverheadNameWidget* Widget = CreateWidget<UOverheadNameWidget>(UGameplayStatics::GetGameInstance(GetWorld()), OverheadNameWidgetClass);

	if (Widget) {
		OverheadNameWidget->SetWidget(Widget);
		OverheadNameWidget->SetRelativeLocation(FVector(10, 0, 125));
		Widget->SetName(Name);
	}
}

void AShopkeeper::Interact(APlayerController* PlayerController)
{
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(GetWorld()->GetAuthGameMode());
	if (!IsValid(GameMode)) {
		return;
	}

	AOverworldHUD* Hud = Cast<AOverworldHUD>(PlayerController->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	GameMode->InitItemsToSell(ItemsToSell);
	Hud->ShowBuyShop();
}
