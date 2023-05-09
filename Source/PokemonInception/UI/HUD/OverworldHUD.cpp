// Fill out your copyright notice in the Description page of Project Settings.


#include "OverworldHUD.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "../../GameModes/OverworldGameMode.h"
#include "../../Settings/PokemonInceptionGameInstance.h"
#include "../../Player/PokemonInceptionCharacter.h"
#include "../../Player/PlayerCharacterController.h"
#include "../../Pokemon/AttackMoveStruct.h"
#include "../../Pokemon/TypeStruct.h"


void AOverworldHUD::BeginPlay()
{
	Super::BeginPlay();
	
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(GameMode)) {
		return;
	}

	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(PlayerOwner);
	if (!IsValid(PlayerController)) {
		return;
	}

	UPokemonInceptionGameInstance* GameInstance = Cast<UPokemonInceptionGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!IsValid(GameInstance)) {
		return;
	}

	TextBoxWidget = CreateWidget<UTextBoxWidget>(UGameplayStatics::GetGameInstance(GetWorld()), TextBoxWidgetClass);
	OnScreenMessageWidget = CreateWidget<UTextBoxWidget>(UGameplayStatics::GetGameInstance(GetWorld()), OnScreenMessageWidgetClass);
	AreaMessageWidget = CreateWidget<UTextBoxWidget>(UGameplayStatics::GetGameInstance(GetWorld()), AreaMessageWidgetClass);
	InGameWidget = CreateWidget<UInGameWidget>(UGameplayStatics::GetGameInstance(GetWorld()), InGameWidgetClass);
	ButtonBlocker = CreateWidget<UUserWidget>(UGameplayStatics::GetGameInstance(GetWorld()), ButtonBlockerClass);

	MenuWidget = CreateWidget<UMenuWidget>(UGameplayStatics::GetGameInstance(GetWorld()), MenuWidgetClass);
	PokedexWidget = CreateWidget<UPokedexWidget>(UGameplayStatics::GetGameInstance(GetWorld()), PokedexWidgetClass);
	PokemonWidget = CreateWidget<UPokemonWidget>(UGameplayStatics::GetGameInstance(GetWorld()), PokemonWidgetClass);
	BagWidget = CreateWidget<UBagWidget>(UGameplayStatics::GetGameInstance(GetWorld()), BagWidgetClass);
	TrainerCardWidget = CreateWidget<UTrainerCardWidget>(UGameplayStatics::GetGameInstance(GetWorld()), TrainerCardWidgetClass);
	SettingsWidget = CreateWidget<USettingsWidget>(UGameplayStatics::GetGameInstance(GetWorld()), SettingsWidgetClass);
	PokemonStorageWidget = CreateWidget<UPokemonStorageWidget>(UGameplayStatics::GetGameInstance(GetWorld()), PokemonStorageWidgetClass);
	MoveManagerWidget = CreateWidget<UMoveManagerWidget>(UGameplayStatics::GetGameInstance(GetWorld()), MoveManagerWidgetClass);
	ShopWidget = CreateWidget<UShopWidget>(UGameplayStatics::GetGameInstance(GetWorld()), ShopWidgetClass);

	PokedexInfoWidget = CreateWidget<UPokedexInfoWidget>(UGameplayStatics::GetGameInstance(GetWorld()), PokedexInfoWidgetClass);
	
	PokemonSummaryWidget = CreateWidget<UPokemonSummaryWidget>(UGameplayStatics::GetGameInstance(GetWorld()), PokemonSummaryWidgetClass);
	SwapPositionWidget = CreateWidget<UPopupSelectionWidget>(UGameplayStatics::GetGameInstance(GetWorld()), SwapPositionWidgetClass);
	UseItemWidget = CreateWidget<UPopupSelectionWidget>(UGameplayStatics::GetGameInstance(GetWorld()), UseItemWidgetClass);
	MoveSelectionPopupWidget = CreateWidget<UMoveSelectionPopupWidget>(UGameplayStatics::GetGameInstance(GetWorld()), MoveSelectionPopupWidgetClass);
	
	ItemInfoWidget = CreateWidget<UItemInfoWidget>(UGameplayStatics::GetGameInstance(GetWorld()), ItemInfoWidgetClass);
	ItemShopInfoWidget = CreateWidget<UItemShopInfoWidget>(UGameplayStatics::GetGameInstance(GetWorld()), ItemShopInfoWidgetClass);

	StorageOperationPopupWidget = CreateWidget<UStorageOperationPopup>(UGameplayStatics::GetGameInstance(GetWorld()), StorageOperationPopupClass);
	ReleaseConfirmPopup = CreateWidget<UPopupSelectionWidget>(UGameplayStatics::GetGameInstance(GetWorld()), ReleaseConfirmPopupClass);
	MoveInfoWidget = CreateWidget<UMoveInfoWidget>(UGameplayStatics::GetGameInstance(GetWorld()), MoveInfoWidgetClass);
	MoveManagerOperationWidget = CreateWidget<UPopupSelectionWidget>(UGameplayStatics::GetGameInstance(GetWorld()), MoveManagerOperationWidgetClass);

	GameMode->ShopMessageDelegate.AddDynamic(ShopWidget, &UShopWidget::ShowMoney);
	GameMode->ItemSlotDelegate.AddDynamic(BagWidget, &UBagWidget::AddToItemBox);
	GameMode->ItemShopSlotDelegate.AddDynamic(ShopWidget, &UShopWidget::DisplayInShop);
	GameMode->PokemonSlotDelegate.AddDynamic(PokemonWidget, &UPokemonWidget::AddToWrapBox);
	GameMode->PokedexSlotDelegate.AddDynamic(PokedexWidget, &UPokedexWidget::AddPokedexSlotToBox);
	GameMode->PartyPokemonIconDelegate.AddDynamic(PokemonStorageWidget, &UPokemonStorageWidget::AddToPartyBox);
	GameMode->StoragePokemonIconDelegate.AddDynamic(PokemonStorageWidget, &UPokemonStorageWidget::AddToStorageBox);

	PlayerController->InteractableMessageDelegate.AddDynamic(InGameWidget, &UInGameWidget::SetMessage);

	MenuWidget->PokedexClicked.AddDynamic(this, &AOverworldHUD::ShowPokedex);
	MenuWidget->PokemonClicked.AddDynamic(this, &AOverworldHUD::ShowPokemon);
	MenuWidget->BagClicked.AddDynamic(this, &AOverworldHUD::ShowBag);
	MenuWidget->TrainerCardClicked.AddDynamic(this, &AOverworldHUD::ShowTrainerCard);
	MenuWidget->SettingsClicked.AddDynamic(this, &AOverworldHUD::ShowSettings);
	MenuWidget->SaveClicked.AddDynamic(GameMode, &AOverworldGameMode::SaveAndExit);

	PokedexWidget->BackClicked.AddDynamic(this, &AOverworldHUD::ShowMenu);
	PokemonWidget->BackClicked.AddDynamic(this, &AOverworldHUD::ShowMenu);
	BagWidget->BackClicked.AddDynamic(this, &AOverworldHUD::ShowMenu);
	TrainerCardWidget->BackClicked.AddDynamic(this, &AOverworldHUD::ShowMenu);
	SettingsWidget->BackClicked.AddDynamic(this, &AOverworldHUD::ShowMenu);
	ShopWidget->ExitClicked.AddDynamic(this, &AOverworldHUD::ClearAndUnpause);
	PokemonStorageWidget->BackClicked.AddDynamic(this, &AOverworldHUD::ClearAndUnpause);
	MoveManagerWidget->BackClicked.AddDynamic(this, &AOverworldHUD::ClearAndUnpause);

	SettingsWidget->GraphicsQualityChanged.AddDynamic(GameInstance, &UPokemonInceptionGameInstance::ChangeScalability);
	SettingsWidget->SEVolumeChanged.AddDynamic(GameInstance, &UPokemonInceptionGameInstance::ChangeSEVolume);
	SettingsWidget->PokemonCryVolumeChanged.AddDynamic(GameInstance, &UPokemonInceptionGameInstance::ChangePokemonCryVolume);
	SettingsWidget->BGMVolumeChanged.AddDynamic(GameInstance, &UPokemonInceptionGameInstance::ChangeBGMVolume);

	ShopWidget->BuyModeClicked.AddDynamic(this, &AOverworldHUD::ShowBuyShop);
	ShopWidget->SellModeClicked.AddDynamic(this, &AOverworldHUD::ShowSellShop);

	ItemInfoWidget->UseClicked.AddDynamic(GameMode, &AOverworldGameMode::SelectItem);

	SwapPositionWidget->ActionClicked.AddDynamic(GameMode, &AOverworldGameMode::SelectPokemon);
	SwapPositionWidget->CancelClicked.AddDynamic(this, &AOverworldHUD::ClearPopup);

	UseItemWidget->ActionClicked.AddDynamic(GameMode, &AOverworldGameMode::SelectPokemon);
	UseItemWidget->CancelClicked.AddDynamic(this, &AOverworldHUD::ClearPopup);

	MoveSelectionPopupWidget->BackClicked.AddDynamic(this, &AOverworldHUD::ClearMovePopup);

	StorageOperationPopupWidget->ActionClicked.AddDynamic(this, &AOverworldHUD::ShowReleaseConfirmPopup);
	StorageOperationPopupWidget->CancelClicked.AddDynamic(this, &AOverworldHUD::ClearPopup);
	ReleaseConfirmPopup->CancelClicked.AddDynamic(this, &AOverworldHUD::ClearPopup);

	MoveManagerOperationWidget->CancelClicked.AddDynamic(this, &AOverworldHUD::ClearPopup);

	ShowInGameWidget();
}

void AOverworldHUD::ShowInGameWidget()
{
	Clear();
	ShopWidget->ClearShopInfo();

	if (PlayerOwner && InGameWidget) {
		InGameWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = false;
		PlayerOwner->SetInputMode(FInputModeGameOnly());
	}
}

void AOverworldHUD::ShowMenu()
{
	Clear();
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(GameMode)) {
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
	if (!IsValid(GameMode)) {
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
	if (!IsValid(GameMode)) {
		return;
	}

	if (PlayerOwner && PokedexInfoWidget) {
		FPokemonBaseStruct PokemonData = GameMode->GetPokemonSpeciesData(PokemonID);
		
		FString PokemonType;
		if (PokemonData.Type2 == ETypes::None) {
			PokemonType = FTypeStruct::ToString(PokemonData.Type1);
		}
		else {
			PokemonType = FTypeStruct::ToString(PokemonData.Type1) + " " + FTypeStruct::ToString(PokemonData.Type2);
		}

		PokedexInfoWidget->SetPokedexInfo(PokemonData, PokemonType);
		PokedexWidget->AddPokedexInfoToBox(PokedexInfoWidget);
	}
}

void AOverworldHUD::ShowPokemon()
{
	Clear();
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(GameMode)) {
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
	ButtonBlocker->AddToViewport();

	if (PlayerOwner && SwapPositionWidget) {
		FVector2D ScreenSize;
		GEngine->GameViewport->GetViewportSize(ScreenSize);

		float XDiference = 1920 / ScreenSize.X;
		float YDiference = 1080 / ScreenSize.Y;

		float MouseX;
		float MouseY;

		PlayerOwner->GetMousePosition(MouseX, MouseY);

		MouseX *= XDiference;
		MouseY *= YDiference;

		SwapPositionWidget->SetId(PokemonId);
		SwapPositionWidget->AddToViewport();
		SwapPositionWidget->SetPositionInViewport(FVector2D(MouseX, MouseY), false);
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());

		/*GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, "Teoretical resolution: 1920x1080 - 1,777777");
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, "Real resolution: " + FString::SanitizeFloat(ScreenSize.X) +
			"x" + FString::SanitizeFloat(ScreenSize.Y) + " - " + FString::SanitizeFloat(ScreenSize.X / ScreenSize.Y));*/
	}
}

void AOverworldHUD::ShowPokemonSummary(int PokemonID)
{
	if (UseItemWidget->IsInViewport() == true || MoveSelectionPopupWidget->IsInViewport() == true || SwapPositionWidget->IsInViewport() == true) {
		return;
	}

	PokemonSummaryWidget->ClearMoves();
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(GameMode)) {
		return;
	}

	if (PlayerOwner && PokemonSummaryWidget) {
		APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		FPokemonStruct Pokemon = PlayerController->PokemonParty[PokemonID];

		FString PokemonType;
		if (Pokemon.SpeciesData.Type2 == ETypes::None) {
			PokemonType = FTypeStruct::ToString(Pokemon.SpeciesData.Type1);
		}
		else {
			PokemonType = FTypeStruct::ToString(Pokemon.SpeciesData.Type1) + " " + FTypeStruct::ToString(Pokemon.SpeciesData.Type2);
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
	if (!IsValid(GameMode)) {
		return;
	}

	if (PlayerOwner && BagWidget) {
		BagWidget->ClearItemBox();
		BagWidget->ClearInfoBox();
		GameMode->FillBagWidget();
		BagWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
	}
}

void AOverworldHUD::ShowItemInfo(int ItemID)
{
	if (PlayerOwner && ItemInfoWidget) {
		APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(PlayerOwner);
		TArray<FItemBaseStruct> Inventory = PlayerController->Inventory;

		ItemInfoWidget->SetDescription(Inventory[ItemID].Description);
		ItemInfoWidget->SetID(ItemID);
		ItemInfoWidget->SetUseButton(Inventory[ItemID].bUsableOutsideBattle);

		BagWidget->ShowInfo(ItemInfoWidget);
	}
}

void AOverworldHUD::ShowItemShopInfo(int ItemID)
{
	ClearOnScreenMessage();
	ShopWidget->ClearShopInfo();

	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(GameMode)) {
		return;
	}
	GameMode->SelectedItemID = ItemID;

	ItemShopInfoWidget->OperationClicked.RemoveAll(GameMode);

	if (PlayerOwner && ItemShopInfoWidget) {
		if (GameMode->ShopMode == "Buy") {
			ItemShopInfoWidget->SetOperation("Buy");
			ItemShopInfoWidget->SetDescription(GameMode->ItemsToSell[ItemID].Description);
			ItemShopInfoWidget->SetPrice(GameMode->ItemsToSell[ItemID].Value);
			ItemShopInfoWidget->SetID(ItemID);

			ItemShopInfoWidget->OperationClicked.AddDynamic(GameMode, &AOverworldGameMode::BuyItem);
		}

		else if (GameMode->ShopMode == "Sell") {
			APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(PlayerOwner);

			ItemShopInfoWidget->SetOperation("Sell");
			ItemShopInfoWidget->SetDescription(PlayerController->Inventory[ItemID].Description);
			ItemShopInfoWidget->SetPrice(PlayerController->Inventory[ItemID].Value);
			ItemShopInfoWidget->SetID(ItemID);

			ItemShopInfoWidget->OperationClicked.AddDynamic(GameMode, &AOverworldGameMode::SellItem);
		}

		ShopWidget->ShowShopInfo(ItemShopInfoWidget);
	}
}

void AOverworldHUD::ShowUseItemPopup(int PokemonId)
{
	if (UseItemWidget->IsInViewport() == true) {
		return;
	}
	ButtonBlocker->AddToViewport();

	if (PlayerOwner && UseItemWidget) {
		FVector2D ScreenSize;
		GEngine->GameViewport->GetViewportSize(ScreenSize);

		float XDiference = 1920 / ScreenSize.X;
		float YDiference = 1080 / ScreenSize.Y;

		float MouseX;
		float MouseY;

		PlayerOwner->GetMousePosition(MouseX, MouseY);

		MouseX *= XDiference;
		MouseY *= YDiference;

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
	if (!IsValid(GameMode)) {
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
		APlayerCharacterController* Controller = Cast<APlayerCharacterController>(PlayerOwner);

		TrainerCardWidget->SetPlayerName(Controller->PlayerName);
		TrainerCardWidget->SetRegisteredSpecies(Controller->Pokedex.Num());
		TrainerCardWidget->SetPokemonCaught(Controller->PokemonParty.Num() + Controller->PokemonStorage.Num());
		TrainerCardWidget->SetMoneyObtained(Controller->Money);
		TrainerCardWidget->SetTrainersDefeated(Controller->TrainersDefeated);

		TrainerCardWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
	}
}

void AOverworldHUD::ShowSettings()
{
	Clear();

	if (PlayerOwner && SettingsWidget) {
		APlayerCharacterController* Controller = Cast<APlayerCharacterController>(PlayerOwner);

		SettingsWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
	}
}

void AOverworldHUD::ShowBuyShop()
{
	ClearOnScreenMessage();

	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(GameMode)) {
		return;
	}

	if (PlayerOwner && ShopWidget) {
		APlayerCharacterController* Controller = Cast<APlayerCharacterController>(PlayerOwner);

		ShopWidget->ClearShop();

		if (GameMode->ShopMode == "Sell") {
			ShopWidget->ClearShopInfo();
		}
		
		ShopWidget->ShowMoney("Money: " + FString::FromInt(Controller->Money) + "$");
		GameMode->InitBuyShop();

		if (!ShopWidget->IsInViewport()) {
			ShopWidget->AddToViewport();
		}
		
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
		GameMode->PauseGame(EPause::Pause);
	}
}

void AOverworldHUD::ShowSellShop()
{
	ClearOnScreenMessage();

	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(GameMode)) {
		return;
	}

	if (PlayerOwner && ShopWidget) {
		APlayerCharacterController* Controller = Cast<APlayerCharacterController>(PlayerOwner);

		ShopWidget->ClearShop();
		ShopWidget->ClearShopInfo();
		
		ShopWidget->ShowMoney("Money: " + FString::FromInt(Controller->Money) + "$");
		GameMode->InitSellShop();

		if (!ShopWidget->IsInViewport()) {
			ShopWidget->AddToViewport();
		}

		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
		GameMode->PauseGame(EPause::Pause);
	}
}

void AOverworldHUD::ShowPokemonStorage()
{
	Clear();
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(GameMode)) {
		return;
	}

	if (PlayerOwner && PokemonStorageWidget) {
		GameMode->ShowPokemonInStorage();
		PokemonStorageWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
		GameMode->PauseGame(EPause::Pause);
	}
}

void AOverworldHUD::ShowWithdrawPopup(int PokemonID)
{
	if (StorageOperationPopupWidget->IsInViewport() || ReleaseConfirmPopup->IsInViewport()) {
		return;
	}
	ButtonBlocker->AddToViewport();

	if (PlayerOwner && StorageOperationPopupWidget) {
		APlayerCharacterController* Controller = Cast<APlayerCharacterController>(PlayerOwner);

		StorageOperationPopupWidget->OperationClicked.RemoveAll(Controller);

		FVector2D ScreenSize;
		GEngine->GameViewport->GetViewportSize(ScreenSize);

		float XDiference = 1920 / ScreenSize.X;
		float YDiference = 1080 / ScreenSize.Y;

		float MouseX;
		float MouseY;

		PlayerOwner->GetMousePosition(MouseX, MouseY);

		MouseX *= XDiference;
		MouseY *= YDiference;
		MousePossition = FVector2D(MouseX, MouseY);

		StorageOperationPopupWidget->SetId(PokemonID);
		StorageOperationPopupWidget->SetOperationText(FText::FromString("Withdraw"));

		StorageOperationPopupWidget->OperationClicked.AddDynamic(Controller, &APlayerCharacterController::MovePokemonToParty);

		StorageOperationPopupWidget->AddToViewport();
		StorageOperationPopupWidget->SetPositionInViewport(FVector2D(MouseX, MouseY), false);
		Controller->bShowMouseCursor = true;
		Controller->SetInputMode(FInputModeUIOnly());

		SelectedPokemonLocation = "Storage";
	}
}

void AOverworldHUD::ShowDepositPopup(int PokemonID)
{
	if (StorageOperationPopupWidget->IsInViewport() || ReleaseConfirmPopup->IsInViewport()) {
		return;
	}
	ButtonBlocker->AddToViewport();

	if (PlayerOwner && StorageOperationPopupWidget) {
		APlayerCharacterController* Controller = Cast<APlayerCharacterController>(PlayerOwner);

		StorageOperationPopupWidget->OperationClicked.RemoveAll(Controller);

		FVector2D ScreenSize;
		GEngine->GameViewport->GetViewportSize(ScreenSize);

		float XDiference = 1920 / ScreenSize.X;
		float YDiference = 1080 / ScreenSize.Y;

		float MouseX;
		float MouseY;

		PlayerOwner->GetMousePosition(MouseX, MouseY);

		MouseX *= XDiference;
		MouseY *= YDiference;
		MousePossition = FVector2D(MouseX, MouseY);

		StorageOperationPopupWidget->SetId(PokemonID);
		StorageOperationPopupWidget->SetOperationText(FText::FromString("Deposit"));

		StorageOperationPopupWidget->OperationClicked.AddDynamic(Controller, &APlayerCharacterController::MovePokemonToStorage);

		StorageOperationPopupWidget->AddToViewport();
		StorageOperationPopupWidget->SetPositionInViewport(FVector2D(MouseX, MouseY), false);
		Controller->bShowMouseCursor = true;
		Controller->SetInputMode(FInputModeUIOnly());

		SelectedPokemonLocation = "Party";
	}
}

void AOverworldHUD::ShowReleaseConfirmPopup(int PokemonID)
{
	ClearPopup();
	ButtonBlocker->AddToViewport();

	if (PlayerOwner && ReleaseConfirmPopup) {
		APlayerCharacterController* Controller = Cast<APlayerCharacterController>(PlayerOwner);

		ReleaseConfirmPopup->ActionClicked.RemoveAll(Controller);

		ReleaseConfirmPopup->SetId(PokemonID);

		if (SelectedPokemonLocation == "Party") {
			ReleaseConfirmPopup->ActionClicked.AddDynamic(Controller, &APlayerCharacterController::ReleasePokemonFromParty);
		}
		if (SelectedPokemonLocation == "Storage") {
			ReleaseConfirmPopup->ActionClicked.AddDynamic(Controller, &APlayerCharacterController::ReleasePokemonFromStorage);
		}

		ReleaseConfirmPopup->SetPositionInViewport(MousePossition, false);
		ReleaseConfirmPopup->AddToViewport();
		Controller->bShowMouseCursor = true;
		Controller->SetInputMode(FInputModeUIOnly());
	}
}

void AOverworldHUD::ShowMoveManager(int PokemonID)
{
	Clear();
	MoveManagerWidget->Clear();
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(GameMode)) {
		return;
	}

	if (PlayerOwner && MoveManagerWidget) {
		APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		FPokemonStruct Pokemon = PlayerController->PokemonParty[PokemonID];

		GameMode->SelectedPokemonID = PokemonID;

		for (int i = 0; i < Pokemon.Moves.Num(); i++) {
			if (Pokemon.Moves[i].bIsLocked == true) {
				continue;
			}

			UMoveButtonWidget* MoveButton = CreateWidget<UMoveButtonWidget>(UGameplayStatics::GetGameInstance(GetWorld()), MoveButtonWidgetClass);

			MoveButton->InitButton(Pokemon.Moves[i].Name, Pokemon.Moves[i].CurrPowerPoints, Pokemon.Moves[i].PowerPoints, Pokemon.Moves[i].MoveType);
			MoveButton->SetMove(i);
			MoveButton->ButtonClicked.AddDynamic(this, &AOverworldHUD::ShowMoveInfo);

			if (Pokemon.CurrentMoves.Contains(i)) {
				MoveManagerWidget->AddToCurrentMoves(MoveButton);
				MoveButton->ButtonClicked.AddDynamic(this, &AOverworldHUD::ShowForgetPopup);
			}
			else {
				MoveManagerWidget->AddToAvailableMoves(MoveButton);
				MoveButton->ButtonClicked.AddDynamic(this, &AOverworldHUD::ShowLearnPopup);
			}
		}

		MoveManagerWidget->AddToViewport();
		PlayerOwner->bShowMouseCursor = true;
		PlayerOwner->SetInputMode(FInputModeUIOnly());
		GameMode->PauseGame(EPause::Pause);
	}
}

void AOverworldHUD::ShowMoveInfo(int MoveID)
{
	if (MoveManagerOperationWidget->IsInViewport()) {
		return;
	}

	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(GameMode)) {
		return;
	}

	if (PlayerOwner && MoveInfoWidget) {
		APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		FPokemonStruct Pokemon = PlayerController->PokemonParty[GameMode->SelectedPokemonID];
		FMoveBaseStruct Move = Pokemon.Moves[MoveID];

		MoveInfoWidget->SetMoveName(Move.Name);
		MoveInfoWidget->SetMoveType(FText::FromString(FTypeStruct::ToString(Move.MoveType)));
		MoveInfoWidget->SetMoveDescription(Move.Description);
		MoveInfoWidget->SetWidgetColor(Move.MoveType);

		if (Move.MoveStructType == "Attack") {
			FAttackMoveStruct* Attack = GameMode->AttackMovesDT->FindRow<FAttackMoveStruct>(Move.MoveID, "");
			MoveInfoWidget->SetMovePower(FText::FromString(FString::FromInt(Attack->BaseDamage)));
		}
		else {
			MoveInfoWidget->SetMovePower(FText::FromString("-"));
		}

		MoveManagerWidget->ShowMoveInfo(MoveInfoWidget);
	}
}

void AOverworldHUD::ShowLearnPopup(int MoveID)
{
	if (MoveManagerOperationWidget->IsInViewport()) {
		return;
	}
	ButtonBlocker->AddToViewport();

	if (PlayerOwner && MoveManagerOperationWidget) {
		APlayerCharacterController* Controller = Cast<APlayerCharacterController>(PlayerOwner);

		MoveManagerOperationWidget->ActionClicked.RemoveAll(Controller);

		FVector2D ScreenSize;
		GEngine->GameViewport->GetViewportSize(ScreenSize);

		float XDiference = 1920 / ScreenSize.X;
		float YDiference = 1080 / ScreenSize.Y;

		float MouseX;
		float MouseY;

		PlayerOwner->GetMousePosition(MouseX, MouseY);

		MouseX *= XDiference;
		MouseY *= YDiference;

		MoveManagerOperationWidget->SetId(MoveID);
		MoveManagerOperationWidget->SetActionText(FText::FromString("Learn"));

		MoveManagerOperationWidget->ActionClicked.AddDynamic(Controller, &APlayerCharacterController::LearnMove);

		MoveManagerOperationWidget->AddToViewport();
		MoveManagerOperationWidget->SetPositionInViewport(FVector2D(MouseX, MouseY), false);
		Controller->bShowMouseCursor = true;
		Controller->SetInputMode(FInputModeUIOnly());
	}
}

void AOverworldHUD::ShowForgetPopup(int MoveID)
{
	if (MoveManagerOperationWidget->IsInViewport()) {
		return;
	}
	ButtonBlocker->AddToViewport();

	if (PlayerOwner && MoveManagerOperationWidget) {
		APlayerCharacterController* Controller = Cast<APlayerCharacterController>(PlayerOwner);

		MoveManagerOperationWidget->ActionClicked.RemoveAll(Controller);

		FVector2D ScreenSize;
		GEngine->GameViewport->GetViewportSize(ScreenSize);

		float XDiference = 1920 / ScreenSize.X;
		float YDiference = 1080 / ScreenSize.Y;

		float MouseX;
		float MouseY;

		PlayerOwner->GetMousePosition(MouseX, MouseY);

		MouseX *= XDiference;
		MouseY *= YDiference;

		MoveManagerOperationWidget->SetId(MoveID);
		MoveManagerOperationWidget->SetActionText(FText::FromString("Forget"));

		MoveManagerOperationWidget->ActionClicked.AddDynamic(Controller, &APlayerCharacterController::ForgetMove);

		MoveManagerOperationWidget->AddToViewport();
		MoveManagerOperationWidget->SetPositionInViewport(FVector2D(MouseX, MouseY), false);
		Controller->bShowMouseCursor = true;
		Controller->SetInputMode(FInputModeUIOnly());
	}
}

void AOverworldHUD::OnScreenMessage(FString Message)
{
	if (OnScreenMessageWidget->IsInViewport()) {
		ClearOnScreenMessage();
	}

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

void AOverworldHUD::ClearPokemonIcons()
{
	PokemonStorageWidget->ClearPartyBox();
	PokemonStorageWidget->ClearStorageBox();
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

void AOverworldHUD::ShowAreaName(FString AreaName)
{
	FTimerHandle ClearTimer;

	if (AreaMessageWidget->IsInViewport()) {
		AreaMessageWidget->RemoveFromViewport();
		GetWorldTimerManager().ClearTimer(ClearTimer);
	}

	if (PlayerOwner && AreaMessageWidget) {
		AreaMessageWidget->ReturnMessage(AreaName);
		AreaMessageWidget->AddToViewport();
		
		GetWorldTimerManager().SetTimer(ClearTimer, AreaMessageWidget, &UTextBoxWidget::RemoveFromViewport, 2, false);
	}
}

void AOverworldHUD::ShowPickupMessage(FString Message, UTexture2D* Image)
{
	if (PlayerOwner && InGameWidget) {
		FTimerHandle ClearTimer;

		UPickupMessageWidget* PickupMessageWidget = CreateWidget<UPickupMessageWidget>(UGameplayStatics::GetGameInstance(GetWorld()), PickupMessageWidgetClass);
		PickupMessageWidget->ReturnMessage(Message);

		if (Image != nullptr) {
			PickupMessageWidget->SetImage(Image);
		}

		InGameWidget->ShowPickupMessage(PickupMessageWidget);
		GetWorldTimerManager().SetTimer(ClearTimer, PickupMessageWidget, &UPickupMessageWidget::RemoveFromViewport, 2, false);
	}
}


void AOverworldHUD::ToggleMainMenu(bool IsPaused)
{
	if (IsPaused) {
		ShowMenu();
	}
	else {
		ShowInGameWidget();
		PlayerOwner->SetInputMode(FInputModeGameOnly());
		PlayerOwner->bShowMouseCursor = false;
	}
}

void AOverworldHUD::Clear()
{
	UWidgetLayoutLibrary::RemoveAllWidgets(this);
}

void AOverworldHUD::ClearAndUnpause()
{
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(GameMode)) {
		return;
	}

	ShowInGameWidget();
	PlayerOwner->SetInputMode(FInputModeGameOnly());
	PlayerOwner->bShowMouseCursor = false;
	GameMode->PauseGame(EPause::UnPause);
}

void AOverworldHUD::ClearPopup()
{
	UseItemWidget->RemoveFromViewport();
	SwapPositionWidget->RemoveFromViewport();
	StorageOperationPopupWidget->RemoveFromViewport();
	ReleaseConfirmPopup->RemoveFromViewport();
	MoveManagerOperationWidget->RemoveFromViewport();

	ButtonBlocker->RemoveFromViewport();
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

TSubclassOf<UPokemonIconWidget> AOverworldHUD::GetPokemonIconWidgetClass()
{
	return PokemonIconWidgetClass;
}
