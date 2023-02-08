 // Fill out your copyright notice in the Description page of Project Settings.


#include "BattleHUD.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "../../GameModes/BattleGameMode.h"
#include "../../Player/BattleController.h"
#include "Engine/Engine.h"

void ABattleHUD::BeginPlay()
{
	Super::BeginPlay();
	
	ABattleGameMode* GameMode = Cast<ABattleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	BattleStartWidget = CreateWidget<UBattleStartWidget>(UGameplayStatics::GetGameInstance(GetWorld()), BattleStartWidgetClass);
	FightWidget = CreateWidget<UFightWidget>(UGameplayStatics::GetGameInstance(GetWorld()), FightWidgetClass);
	PokemonWidget = CreateWidget<UPokemonWidget>(UGameplayStatics::GetGameInstance(GetWorld()), PokemonWidgetClass);
	PokemonSummaryWidget = CreateWidget<UPokemonSummaryWidget>(UGameplayStatics::GetGameInstance(GetWorld()), PokemonSummaryWidgetClass);
	SwitchOutWidget = CreateWidget<UPopupSelectionWidget>(UGameplayStatics::GetGameInstance(GetWorld()), SwitchOutWidgetClass);
	UseItemWidget = CreateWidget<UPopupSelectionWidget>(UGameplayStatics::GetGameInstance(GetWorld()), UseItemWidgetClass);
	MoveSelectionPopupWidget = CreateWidget<UMoveSelectionPopupWidget>(UGameplayStatics::GetGameInstance(GetWorld()), MoveSelectionPopupWidgetClass);
	PlayerPokemonStatusWidget = CreateWidget<UPlayerPokemonStatusWidget>(UGameplayStatics::GetGameInstance(GetWorld()), PlayerPokemonStatusWidgetClass);
	OpponentPokemonStatusWidget = CreateWidget<UPokemonStatusWidget>(UGameplayStatics::GetGameInstance(GetWorld()), OpponentStatusWidgetClass);
	BagWidget = CreateWidget<UBagWidget>(UGameplayStatics::GetGameInstance(GetWorld()), BagWidgetClass);
	ItemInfoWidget = CreateWidget<UItemInfoWidget>(UGameplayStatics::GetGameInstance(GetWorld()), ItemInfoWidgetClass);
	TextBoxWidget = CreateWidget<UTextBoxWidget>(UGameplayStatics::GetGameInstance(GetWorld()), TextBoxWidgetClass);

	BattleStartWidget->FightClicked.AddDynamic(this, &ABattleHUD::ShowFightWidget);
	BattleStartWidget->PokemonClicked.AddDynamic(this, &ABattleHUD::ShowPokemon);
	BattleStartWidget->BagClicked.AddDynamic(this, &ABattleHUD::ShowBag);
	BattleStartWidget->RunClicked.AddDynamic(GameMode, &ABattleGameMode::Run);

	FightWidget->BackClicked.AddDynamic(this, &ABattleHUD::ShowBattleStartWidget);
	PokemonWidget->BackClicked.AddDynamic(this, &ABattleHUD::ClearPokemon);
	BagWidget->BackClicked.AddDynamic(this, &ABattleHUD::ShowBattleStartWidget);

	ItemInfoWidget->UseClicked.AddDynamic(GameMode, &ABattleGameMode::SelectItem);

	SwitchOutWidget->ActionClicked.AddDynamic(GameMode, &ABattleGameMode::SelectPokemon);
	SwitchOutWidget->CancelClicked.AddDynamic(this, &ABattleHUD::ClearPopup);

	UseItemWidget->ActionClicked.AddDynamic(GameMode, &ABattleGameMode::SelectPokemonToUseItem);
	UseItemWidget->CancelClicked.AddDynamic(this, &ABattleHUD::ClearPopup);

	MoveSelectionPopupWidget->BackClicked.AddDynamic(this, &ABattleHUD::ClearMovePopup);

	GameMode->MessageUpdate.AddDynamic(TextBoxWidget, &UTextBoxWidget::ReturnMessage);
	GameMode->ItemSlotDelegate.AddDynamic(BagWidget, &UBagWidget::AddToWrapBox);
	GameMode->PokemonSlotDelegate.AddDynamic(PokemonWidget, &UPokemonWidget::AddToWrapBox);
	GameMode->MoveDelegate.AddDynamic(FightWidget, &UFightWidget::AddToWrapBox);
}

TSubclassOf<UItemSlotWidget> ABattleHUD::GetItemSlotWidgetClass()
{
	return ItemSlotWidgetClass; 
}

TSubclassOf<UPokemonSlotWidget> ABattleHUD::GetPokemonSlotWidgetClass()
{
	return PokemonSlotWidgetClass;
}

TSubclassOf<UMoveButtonWidget> ABattleHUD::GetMoveButtonWidgetClass()
{
	return MoveButtonWidgetClass;
}

void ABattleHUD::Clear()
{
	UWidgetLayoutLibrary::RemoveAllWidgets(this);
}

void ABattleHUD::ClearPokemon()
{
	ABattleGameMode* GameMode = Cast<ABattleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode == nullptr) {
		return;
	}

	if (GameMode->bHasToSwitchPokemon() == true) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("You have to select a pokemon"));
		return;
	}
	ShowBattleStartWidget();
}

void ABattleHUD::ClearPopup()
{
	SwitchOutWidget->RemoveFromViewport();
	UseItemWidget->RemoveFromViewport();
}

void ABattleHUD::ClearMovePopup()
{
	MoveSelectionPopupWidget->RemoveFromViewport();
}

void ABattleHUD::ShowText(FString Message)
{
	Clear();

	if (PlayerOwner && TextBoxWidget) {
		TextBoxWidget->ReturnMessage(Message);
		TextBoxWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = false;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
	}
}

void ABattleHUD::ShowFightWidget()
{
	Clear();
	ABattleGameMode* GameMode = Cast<ABattleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode == nullptr) {
		return;
	}

	if (GameMode->HasPlayerRanOutOfPP() == true) {
		GameMode->SelectMove(-1);
		return;
	}

	if (PlayerOwner && FightWidget) {
		ShowPlayerPokemonStatus();
		ShowOpponentPokemonStatus();
		FightWidget->ClearWrapBox();
		GameMode->ShowPokemonMoves();
		FightWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
	}
}

void ABattleHUD::ShowBag()
{
	Clear();
	ABattleGameMode* GameMode = Cast<ABattleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode == nullptr) {
		return;
	}

	if (PlayerOwner && BagWidget) {
		BagWidget->ClearWrapBox();
		BagWidget->ClearInfoBox();
		GameMode->FillBagWidget();
		BagWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
	}
}

void ABattleHUD::ShowItemInfo(int ItemID)
{
	if (PlayerOwner && ItemInfoWidget) {
		ABattleController* Controller = Cast<ABattleController>(PlayerOwner);

		ItemInfoWidget->SetDescription(Controller->Inventory[ItemID].Description);
		ItemInfoWidget->SetID(ItemID);

		BagWidget->ShowInfo(ItemInfoWidget);
	}
}

void ABattleHUD::ShowPokemon()
{
	Clear();
	ABattleGameMode* GameMode = Cast<ABattleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode == nullptr) {
		return;
	}

	if (PlayerOwner && PokemonWidget) {
		PokemonWidget->ClearWrapBox();
		PokemonWidget->ClearSummaryBox();
		GameMode->ShowPokemonInMenu();
		PokemonWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
	}
}

void ABattleHUD::ShowPokemonSummary(int PokemonID)
{
	if (SwitchOutWidget->IsInViewport() == true || UseItemWidget->IsInViewport() == true || MoveSelectionPopupWidget->IsInViewport() == true) {
		return;
	}

	PokemonSummaryWidget->ClearMoves();
	ABattleGameMode* GameMode = Cast<ABattleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode == nullptr) {
		return;
	}

	if (PlayerOwner && PokemonSummaryWidget) {
		ABattleController* Controller = Cast<ABattleController>(PlayerOwner);
		FPokemonStruct Pokemon = Controller->PokemonParty[PokemonID];

		FString PokemonType;
		if (Pokemon.SpeciesData.Type2 == ETypes::None) {
			PokemonType = GameMode->ETypeToString(Pokemon.SpeciesData.Type1);
		}
		else {
			PokemonType = GameMode->ETypeToString(Pokemon.SpeciesData.Type1) + " " + GameMode->ETypeToString(Pokemon.SpeciesData.Type2);
		}

		FString PokemonHP = FString::FromInt(Pokemon.CurrHP) + "/" + FString::FromInt(Pokemon.MaxHP);

		PokemonSummaryWidget->SetImage(Pokemon.SpeciesData.Image);
		PokemonSummaryWidget->SetGeneralInfo(Pokemon.SpeciesData.Name, Pokemon.SpeciesData.PokemonID, PokemonType, Pokemon.Level, (Pokemon.RequiredExp - Pokemon.CurrExp));
		PokemonSummaryWidget->SetStats(PokemonHP, Pokemon.Attack, Pokemon.Defence, Pokemon.Speed);

		for (int i = 0; i < Pokemon.CurrentMoves.Num(); i++) {
			UMoveButtonWidget* MoveButton = CreateWidget<UMoveButtonWidget>(UGameplayStatics::GetGameInstance(GetWorld()), MoveButtonWidgetClass);

			MoveButton->InitButton(Pokemon.Moves[Pokemon.CurrentMoves[i]].Name, Pokemon.Moves[Pokemon.CurrentMoves[i]].CurrPowerPoints, 
				Pokemon.Moves[Pokemon.CurrentMoves[i]].PowerPoints, Pokemon.Moves[Pokemon.CurrentMoves[i]].MoveType);
			PokemonSummaryWidget->AddMove(MoveButton);
		}

		PokemonWidget->AddToInfoWrapBox(PokemonSummaryWidget);
	}
}

void ABattleHUD::ShowSwitchOutPopup(int PokemonId)
{
	if (SwitchOutWidget->IsInViewport() == true) {
		return;
	}

	if (PlayerOwner && SwitchOutWidget) {
		float MouseX;
		float MouseY;

		PlayerOwner->GetMousePosition(MouseX, MouseY);

		SwitchOutWidget->SetId(PokemonId);
		SwitchOutWidget->AddToViewport();
		SwitchOutWidget->SetPositionInViewport(FVector2D(MouseX, MouseY), false);
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
	}
}

void ABattleHUD::ShowUseItemPopup(int PokemonId)
{
	if (UseItemWidget->IsInViewport() == true) {
		return;
	}

	if (PlayerOwner && UseItemWidget) {
		float MouseX;
		float MouseY;

		PlayerOwner->GetMousePosition(MouseX, MouseY);

		UseItemWidget->SetId(PokemonId);
		UseItemWidget->AddToViewport();
		UseItemWidget->SetPositionInViewport(FVector2D(MouseX, MouseY), false);
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
	}
}

void ABattleHUD::ShowMoveSelectionPopup(int PokemonId)
{
	if (MoveSelectionPopupWidget->IsInViewport() == true) {
		return;
	}

	ABattleGameMode* GameMode = Cast<ABattleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode == nullptr) {
		return;
	}

	if (PlayerOwner && MoveSelectionPopupWidget) {
		MoveSelectionPopupWidget->ClearWrapBox();
		ABattleController* Controller = Cast<ABattleController>(PlayerOwner);

		FPokemonStruct Pokemon = Controller->PokemonParty[PokemonId];
		for (int i = 0; i < Pokemon.CurrentMoves.Num(); i++) {
			UMoveButtonWidget* MoveButton = CreateWidget<UMoveButtonWidget>(UGameplayStatics::GetGameInstance(GetWorld()), MoveButtonWidgetClass);

			MoveButton->InitButton(Pokemon.Moves[Pokemon.CurrentMoves[i]].Name, Pokemon.Moves[Pokemon.CurrentMoves[i]].CurrPowerPoints,
				Pokemon.Moves[Pokemon.CurrentMoves[i]].PowerPoints, Pokemon.Moves[Pokemon.CurrentMoves[i]].MoveType);
			MoveButton->SetMove(Pokemon.CurrentMoves[i]);
			MoveButton->ButtonClicked.AddDynamic(GameMode, &ABattleGameMode::SelectMoveToUseItem);
			MoveSelectionPopupWidget->AddToWrapBox(MoveButton);
		}

		MoveSelectionPopupWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
	}
}

void ABattleHUD::ShowPlayerPokemonStatus()
{
	ABattleGameMode* GameMode = Cast<ABattleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode == nullptr) {
		return;
	}

	if (PlayerOwner && PlayerPokemonStatusWidget) {
		RefreshPlayerPokemonStatus();
		PlayerPokemonStatusWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
	}
}

void ABattleHUD::ShowOpponentPokemonStatus()
{
	ABattleGameMode* GameMode = Cast<ABattleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode == nullptr) {
		return;
	}

	if (PlayerOwner && OpponentPokemonStatusWidget) {
		RefreshOpponentPokemonStatus();
		OpponentPokemonStatusWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
	}
}

void ABattleHUD::RefreshPlayerPokemonStatus()
{
	ABattleController* Controller = Cast<ABattleController>(PlayerOwner);
	ABattleGameMode* GameMode = Cast<ABattleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode == nullptr) {
		return;
	}

	FPokemonStruct PokemonData = Controller->PokemonParty[GameMode->GetPlayerPokemonId()];
	PlayerPokemonStatusWidget->SetPokemonHP(PokemonData.CurrHP, PokemonData.MaxHP);
	PlayerPokemonStatusWidget->SetPokemonEXP(PokemonData.CurrExp, PokemonData.RequiredExp);
	PlayerPokemonStatusWidget->SetPokemonLevel(PokemonData.Level);
	PlayerPokemonStatusWidget->SetPokemonName(PokemonData.SpeciesData.Name);
	PlayerPokemonStatusWidget->SetPokemonStatus(PokemonData.Effects);
}

void ABattleHUD::RefreshOpponentPokemonStatus()
{
	ABattleGameMode* GameMode = Cast<ABattleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode == nullptr) {
		return;
	}

	FPokemonStruct PokemonData = GameMode->GetCurrentOpponentStruct();
	OpponentPokemonStatusWidget->SetPokemonHP(PokemonData.CurrHP, PokemonData.MaxHP);
	OpponentPokemonStatusWidget->SetPokemonLevel(PokemonData.Level);
	OpponentPokemonStatusWidget->SetPokemonName(PokemonData.SpeciesData.Name);
	OpponentPokemonStatusWidget->SetPokemonStatus(PokemonData.Effects);
}

void ABattleHUD::ShowBattleStartWidget()
{
	Clear();
	ABattleGameMode* GameMode = Cast<ABattleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode == nullptr) {
		return;
	}

	if (PlayerOwner && BattleStartWidget) {
		BattleStartWidget->AddToViewport();
		ShowPlayerPokemonStatus();
		ShowOpponentPokemonStatus();
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
	}
}
