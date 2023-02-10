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
	PokedexInfoWidget = CreateWidget<UPokedexInfoWidget>(UGameplayStatics::GetGameInstance(GetWorld()), PokedexInfoWidgetClass);
	PokemonWidget = CreateWidget<UPokemonWidget>(UGameplayStatics::GetGameInstance(GetWorld()), PokemonWidgetClass);
	SwapPositionWidget = CreateWidget<UPopupSelectionWidget>(UGameplayStatics::GetGameInstance(GetWorld()), SwapPositionWidgetClass);
	PokemonSummaryWidget = CreateWidget<UPokemonSummaryWidget>(UGameplayStatics::GetGameInstance(GetWorld()), PokemonSummaryWidgetClass);
	BagWidget = CreateWidget<UBagWidget>(UGameplayStatics::GetGameInstance(GetWorld()), BagWidgetClass);
	ItemInfoWidget = CreateWidget<UItemInfoWidget>(UGameplayStatics::GetGameInstance(GetWorld()), ItemInfoWidgetClass);
	UseItemWidget = CreateWidget<UPopupSelectionWidget>(UGameplayStatics::GetGameInstance(GetWorld()), UseItemWidgetClass);
	MoveSelectionPopupWidget = CreateWidget<UMoveSelectionPopupWidget>(UGameplayStatics::GetGameInstance(GetWorld()), MoveSelectionPopupWidgetClass);
	TrainerCardWidget = CreateWidget<UTrainerCardWidget>(UGameplayStatics::GetGameInstance(GetWorld()), TrainerCardWidgetClass);
	ShopWidget = CreateWidget<UShopWidget>(UGameplayStatics::GetGameInstance(GetWorld()), ShopWidgetClass);

	GameMode->ShopMessageDelegate.AddDynamic(ShopWidget, &UShopWidget::ShowText);
	GameMode->ItemSlotDelegate.AddDynamic(BagWidget, &UBagWidget::AddToWrapBox);
	GameMode->ItemShopSlotDelegate.AddDynamic(ShopWidget, &UShopWidget::DisplayInShop);
	GameMode->PokemonSlotDelegate.AddDynamic(PokemonWidget, &UPokemonWidget::AddToWrapBox);
	GameMode->PokedexSlotDelegate.AddDynamic(PokedexWidget, &UPokedexWidget::AddPokedexSlotToBox);

	MenuWidget->PokedexClicked.AddDynamic(this, &AOverworldHUD::ShowPokedex);
	MenuWidget->PokemonClicked.AddDynamic(this, &AOverworldHUD::ShowPokemon);
	MenuWidget->BagClicked.AddDynamic(this, &AOverworldHUD::ShowBag);
	MenuWidget->TrainerCardClicked.AddDynamic(this, &AOverworldHUD::ShowTrainerCard);
	MenuWidget->SaveClicked.AddDynamic(GameMode, &AOverworldGameMode::SaveAndExit);

	PokedexWidget->BackClicked.AddDynamic(this, &AOverworldHUD::ShowMenu);
	PokemonWidget->BackClicked.AddDynamic(this, &AOverworldHUD::ShowMenu);
	BagWidget->BackClicked.AddDynamic(this, &AOverworldHUD::ShowMenu);
	TrainerCardWidget->BackClicked.AddDynamic(this, &AOverworldHUD::ShowMenu);
	ShopWidget->ExitClicked.AddDynamic(this, &AOverworldHUD::ClearShop);

	ItemInfoWidget->UseClicked.AddDynamic(GameMode, &AOverworldGameMode::SelectItem);

	SwapPositionWidget->ActionClicked.AddDynamic(GameMode, &AOverworldGameMode::SelectPokemon);
	SwapPositionWidget->CancelClicked.AddDynamic(this, &AOverworldHUD::ClearPopup);

	UseItemWidget->ActionClicked.AddDynamic(GameMode, &AOverworldGameMode::SelectPokemon);
	UseItemWidget->CancelClicked.AddDynamic(this, &AOverworldHUD::ClearPopup);

	MoveSelectionPopupWidget->BackClicked.AddDynamic(this, &AOverworldHUD::ClearMovePopup);

}

void AOverworldHUD::ShowMenu()
{
	Clear();
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode == nullptr) {
		return;
	}

	GameMode->bHasSelectedEther = false;
	GameMode->bHasSelectedItem = false;
	GameMode->bIsSwappingPosition = false;

	if (PlayerOwner && MenuWidget) {
		MenuWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeGameAndUI());
	}
}

void AOverworldHUD::ShowPokedex()
{
	Clear();
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode == nullptr) {
		return;
	}

	if (PlayerOwner && PokedexWidget) {
		PokedexWidget->ClearSlotBox();
		PokedexWidget->ClearInfoBox();
		GameMode->FillPokedex();
		PokedexWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
	}
}

void AOverworldHUD::ShowPokedexInfo(FName PokemonID)
{
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode == nullptr) {
		return;
	}

	if (PlayerOwner && PokedexInfoWidget) {
		FPokemonBaseStruct PokemonData = GameMode->GetPokemonSpeciesData(PokemonID);
		
		FString PokemonType;
		if (PokemonData.Type2 == ETypes::None) {
			PokemonType = GameMode->ETypeToString(PokemonData.Type1);
		}
		else {
			PokemonType = GameMode->ETypeToString(PokemonData.Type1) + " " + GameMode->ETypeToString(PokemonData.Type2);
		}

		PokedexInfoWidget->SetPokedexInfo(PokemonData, PokemonType);
		PokedexWidget->AddPokedexInfoToBox(PokedexInfoWidget);
	}
}

void AOverworldHUD::ShowPokemon()
{
	Clear();
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode == nullptr) {
		return;
	}

	if (PlayerOwner && PokemonWidget) {
		PokemonWidget->ClearSummaryBox();
		GameMode->ShowPokemonInMenu();
		PokemonWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
	}
}

void AOverworldHUD::ShowSwapPositionPopup(int PokemonId)
{
	if (SwapPositionWidget->IsInViewport() == true) {
		return;
	}

	if (PlayerOwner && SwapPositionWidget) {
		float MouseX;
		float MouseY;

		PlayerOwner->GetMousePosition(MouseX, MouseY);

		SwapPositionWidget->SetId(PokemonId);
		SwapPositionWidget->AddToViewport();
		SwapPositionWidget->SetPositionInViewport(FVector2D(MouseX, MouseY), false);
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
	}
}

void AOverworldHUD::ShowPokemonSummary(int PokemonID)
{
	if (UseItemWidget->IsInViewport() == true || MoveSelectionPopupWidget->IsInViewport() == true || SwapPositionWidget->IsInViewport() == true) {
		return;
	}

	PokemonSummaryWidget->ClearMoves();
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode == nullptr) {
		return;
	}

	if (PlayerOwner && PokemonSummaryWidget) {
		APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		FPokemonStruct Pokemon = PlayerController->PokemonParty[PokemonID];

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

void AOverworldHUD::ShowBag()
{
	Clear();
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
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

void AOverworldHUD::ShowItemInfo(int ItemID)
{
	if (PlayerOwner && BagWidget) {
		APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(PlayerOwner);
		TArray<FItemBaseStruct> Inventory = PlayerController->Inventory;

		ItemInfoWidget->SetDescription(Inventory[ItemID].Description);
		ItemInfoWidget->SetID(ItemID);

		BagWidget->ShowInfo(ItemInfoWidget);
	}
}

void AOverworldHUD::ShowUseItemPopup(int PokemonId)
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

void AOverworldHUD::ShowMoveSelectionPopup(int PokemonId)
{
	if (MoveSelectionPopupWidget->IsInViewport() == true) {
		return;
	}

	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode == nullptr) {
		return;
	}

	if (PlayerOwner && MoveSelectionPopupWidget) {
		MoveSelectionPopupWidget->ClearWrapBox();
		APlayerCharacterController* Controller = Cast<APlayerCharacterController>(PlayerOwner);

		FPokemonStruct Pokemon = Controller->PokemonParty[PokemonId];
		for (int i = 0; i < Pokemon.CurrentMoves.Num(); i++) {
			UMoveButtonWidget* MoveButton = CreateWidget<UMoveButtonWidget>(UGameplayStatics::GetGameInstance(GetWorld()), MoveButtonWidgetClass);

			MoveButton->InitButton(Pokemon.Moves[Pokemon.CurrentMoves[i]].Name, Pokemon.Moves[Pokemon.CurrentMoves[i]].CurrPowerPoints,
				Pokemon.Moves[Pokemon.CurrentMoves[i]].PowerPoints, Pokemon.Moves[Pokemon.CurrentMoves[i]].MoveType);
			MoveButton->SetMove(Pokemon.CurrentMoves[i]);
			MoveButton->ButtonClicked.AddDynamic(GameMode, &AOverworldGameMode::SelectMove);
			MoveSelectionPopupWidget->AddToWrapBox(MoveButton);
		}

		MoveSelectionPopupWidget->AddToViewport();
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

void AOverworldHUD::ShowShop(TArray<FName> ItemsToSell)
{
	Clear();
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode == nullptr) {
		return;
	}

	if (PlayerOwner && ShopWidget) {
		GameMode->InitShop(ItemsToSell);
		ShopWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
		PlayerOwner->SetPause(true);
	}
}

void AOverworldHUD::OnScreenMessage(FString Message)
{
	ClearOnScreenMessage();

	if (PlayerOwner && OnScreenMessageWidget) {
		FTimerHandle ClearTimer;

		OnScreenMessageWidget->ReturnMessage(Message);
		OnScreenMessageWidget->AddToViewport();

		GetWorldTimerManager().SetTimer(ClearTimer, this, &AOverworldHUD::ClearOnScreenMessage, 1, false);
	}
}

void AOverworldHUD::ClearOnScreenMessage()
{
	OnScreenMessageWidget->RemoveFromViewport();
}

void AOverworldHUD::ClearPokemonSlots()
{
	PokemonWidget->ClearWrapBox();
}

void AOverworldHUD::ShowText(FString Message)
{
	Clear();

	if (PlayerOwner && TextBoxWidget) {
		TextBoxWidget->ReturnMessage(Message);
		TextBoxWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = false;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
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

void AOverworldHUD::ClearShop()
{
	Clear();
	ShopWidget->ClearShop();
	PlayerOwner->SetInputMode(FInputModeGameOnly());
	PlayerOwner->bShowMouseCursor = false;
	PlayerOwner->SetPause(false);
}

void AOverworldHUD::ClearPopup()
{
	UseItemWidget->RemoveFromViewport();
	SwapPositionWidget->RemoveFromViewport();
}

void AOverworldHUD::ClearMovePopup()
{
	MoveSelectionPopupWidget->RemoveFromViewport();
}

bool AOverworldHUD::BIsMovePopupInViewport()
{
	return MoveSelectionPopupWidget->IsInViewport();
}

TSubclassOf<UItemSlotWidget> AOverworldHUD::GetItemSlotWidgetClass()
{
	return ItemSlotWidgetClass;
}

TSubclassOf<UItemShopSlotWidget> AOverworldHUD::GetItemShopSlotWidgetClass()
{
	return ItemShopSlotWidgetClass;
}

TSubclassOf<UPokedexSlotWidget> AOverworldHUD::GetPokedexSlotWidgetClass()
{
	return PokedexSlotWidgetClass;
}

TSubclassOf<UPokemonSlotWidget> AOverworldHUD::GetPokemonSlotWidgetClass()
{
	return PokemonSlotWidgetClass;
}

TSubclassOf<UMoveButtonWidget> AOverworldHUD::GetMoveButtonWidgetClass()
{
	return MoveButtonWidgetClass;
}
