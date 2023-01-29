// Copyright Epic Games, Inc. All Rights Reserved.

#include "OverworldGameMode.h"
#include "../Player/PokemonInceptionCharacter.h"
#include "../UI/OverworldUI/OverworldHUD.h"
#include "GameFramework/Controller.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/WorldSettings.h"
#include "../Player/PlayerCharacterController.h"


void AOverworldGameMode::BeginPlay()
{
	Super::BeginPlay();

	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController == nullptr) {
		return;
	}

	APokemonInceptionCharacter* PlayerOwner = Cast<APokemonInceptionCharacter>(PlayerController->GetPawn());
	if (PlayerOwner == nullptr) {
		return;
	}

	UWorldSaveData* SaveData = Cast<UWorldSaveData>(UGameplayStatics::CreateSaveGameObject(UWorldSaveData::StaticClass()));

	if (UGameplayStatics::DoesSaveGameExist("SaveSlot", 0)) {
		SaveData = Cast<UWorldSaveData>(UGameplayStatics::LoadGameFromSlot("SaveSlot", 0));

		PlayerController->LoadInventory(SaveData->InventoryData);
		PlayerController->RecieveMoney(SaveData->MoneyData);
		PlayerController->LoadPokemonParty(SaveData->PartyData);
		PlayerController->LoadPokemonStorage(SaveData->StorageData);
		PlayerOwner->SetActorLocation(SaveData->GameMapData.PlayerLocation);
		PlayerOwner->SetActorRotation(SaveData->GameMapData.PlayerRotation, ETeleportType::None);
		
		if (SaveData->GameMapData.ActorsToDestroy.Num() > 0) {
			ActorsToDestroy = SaveData->GameMapData.ActorsToDestroy;
		}

		SaveData->OpponentData.Empty();

		for (FPokemonBaseStruct SpeciesData : SaveData->PokedexData) {
			PlayerController->RegisterToPokedex(SpeciesData);
		}

		if (ActorsToDestroy.Num() > 0) {
			for (AActor* Actor : ActorsToDestroy) {
				//Actor->Destroy();
			}
		}
	}
	
	PlayerController->PauseDelegate.AddDynamic(this, &AOverworldGameMode::TogglePause);

	OnGamePaused.AddDynamic(Cast<AOverworldHUD>(PlayerController->GetHUD()), &AOverworldHUD::TogglePause);
}

void AOverworldGameMode::SaveGame()
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController == nullptr) {
		return;
	}

	APokemonInceptionCharacter* PlayerOwner = Cast<APokemonInceptionCharacter>(PlayerController->GetPawn());
	if (PlayerOwner == nullptr) {
		return;
	}

	UWorldSaveData* SaveData = Cast<UWorldSaveData>(UGameplayStatics::CreateSaveGameObject(UWorldSaveData::StaticClass()));

	SaveData->InventoryData = PlayerController->GetInventory();
	SaveData->PokedexData = PlayerController->GetPokedexData();
	SaveData->MoneyData = PlayerController->GetMoney();
	SaveData->GameMapData.PlayerLocation = PlayerOwner->GetActorLocation();
	SaveData->GameMapData.PlayerRotation = PlayerOwner->GetActorRotation();
	SaveData->PartyData = PlayerController->GetPokemonParty();
	SaveData->StorageData = PlayerController->GetPokemonStorage();
	
	if (ActorsToDestroy.Num() > 0) {
		SaveData->GameMapData.ActorsToDestroy = ActorsToDestroy;
	}

	UGameplayStatics::SaveGameToSlot(SaveData, "SaveSlot", 0);
}

void AOverworldGameMode::MarkActorAsDestroyed(AActor* Actor)
{
	ActorsToDestroy.Add(Actor);
}

void AOverworldGameMode::SaveOpponent(FPokemonStruct Opponent)
{
	UWorldSaveData* SaveData = Cast<UWorldSaveData>(UGameplayStatics::CreateSaveGameObject(UWorldSaveData::StaticClass()));

	if (UGameplayStatics::DoesSaveGameExist("SaveSlot", 0)) {
		SaveData = Cast<UWorldSaveData>(UGameplayStatics::LoadGameFromSlot("SaveSlot", 0));

		SaveData->OpponentData.Add(Opponent);
	}

	UGameplayStatics::SaveGameToSlot(SaveData, "SaveSlot", 0);
}

void AOverworldGameMode::SaveAndExit()
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController == nullptr) {
		return;
	}

	SaveGame();
	PlayerController->ConsoleCommand("quit");
}

FString AOverworldGameMode::ETypeToString(ETypes Type)
{
	switch (Type) {
	case ETypes::Normal:
		return "Normal";
	case ETypes::Grass:
		return "Grass";
	case ETypes::Fire:
		return "Fire";
	case ETypes::Water:
		return "Water";
	case ETypes::Earth:
		return "Earth";
	case ETypes::Air:
		return "Air";
	case ETypes::Electric:
		return "Electric";
	case ETypes::Bug:
		return "Bug";
	case ETypes::Light:
		return "Light";
	case ETypes::Dark:
		return "Dark";
	case ETypes::Dragon:
		return "Dragon";
	default:
		return " ";
	}
}

void AOverworldGameMode::OnScreenMessage(FString MessageToDisplay)
{
	AOverworldHUD* Hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	OnScreenMessageDelegate.Broadcast(MessageToDisplay);
	GetWorldTimerManager().SetTimer(ScreenMessageTimer, Hud, &AOverworldHUD::ClearOnScreenMessage, 1, false);
}

void AOverworldGameMode::EndOnScreenMessage()
{
	AOverworldHUD* Hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (Hud != nullptr) {
		Hud->ClearOnScreenMessage();
	}
}

void AOverworldGameMode::FillBagWidget()
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	TArray<FItemBaseStruct> Inventory = PlayerController->GetInventory();

	TArray<FItemBaseStruct> UniqueItems;
	TArray<int> ItemCount;

	for (FItemBaseStruct Item : Inventory) {
		if (UniqueItems.Contains(Item) == false) {
			UniqueItems.Add(Item);
		}
	}

	for (FItemBaseStruct Item : UniqueItems) {
		int Count = 0;

		for (FItemBaseStruct ItemToSearch : Inventory) {
			if (Item == ItemToSearch) {
				Count++;
			}
		}

		ItemCount.Add(Count);
	}

	for (int i = 0; i < UniqueItems.Num(); i++) {
		AOverworldHUD* Hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
		UItemSlotWidget* ItemSlotWidget = CreateWidget<UItemSlotWidget>(UGameplayStatics::GetGameInstance(GetWorld()), Hud->GetItemSlotWidgetClass());

		ItemSlotWidget->SetItemName(UniqueItems[i].Name);
		ItemSlotWidget->SetItemImage(UniqueItems[i].Image);
		ItemSlotWidget->SetItemCount(ItemCount[i]);
		ItemSlotWidget->SetItem(UniqueItems[i]);

		ItemSlotWidget->ItemClicked.AddDynamic(this, &AOverworldGameMode::ShowItemInfo);

		ItemSlotDelegate.Broadcast(ItemSlotWidget);
	}
}

void AOverworldGameMode::ShowItemInfo(FItemBaseStruct InventoryItem)
{
	AOverworldHUD* Hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	UItemInfoWidget* ItemInfo = CreateWidget<UItemInfoWidget>(UGameplayStatics::GetGameInstance(GetWorld()), Hud->GetItemInfoWidgetClass());

	ItemInfo->SetDescription(InventoryItem.Description);

	ItemInfoDelegate.Broadcast(ItemInfo);
}

void AOverworldGameMode::InitShop(TArray<FName> ItemsToSell)
{
	ShopSlots.Empty();
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	TArray<FItemBaseStruct> Inventory = PlayerController->GetInventory();

	TArray<FItemBaseStruct> ItemsInShop;

	for (FName ItemID : ItemsToSell) 
	{
		for (UDataTable* ItemTable : ItemDT) 
		{
			FItemBaseStruct* Item = ItemTable->FindRow<FItemBaseStruct>(ItemID, "");

			if(Item) 
			{
				ItemsInShop.Add(*Item);
			}
		}
	}
	
	for (int i = 0; i < ItemsInShop.Num(); i++) {
		AOverworldHUD* Hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
		UItemShopSlotWidget* ShopSlotWidget = CreateWidget<UItemShopSlotWidget>(UGameplayStatics::GetGameInstance(GetWorld()), Hud->GetItemShopSlotWidgetClass());

		ShopSlotWidget->SetItemName(ItemsInShop[i].Name);
		ShopSlotWidget->SetItemImage(ItemsInShop[i].Image);
		ShopSlotWidget->SetItemCount(0);
		ShopSlotWidget->SetItem(ItemsInShop[i]);

		ShopSlotWidget->BuyClicked.AddDynamic(this, &AOverworldGameMode::BuyItem);
		ShopSlotWidget->SellClicked.AddDynamic(this, &AOverworldGameMode::SellItem);

		ShopSlots.Add(ShopSlotWidget);

		ItemShopSlotDelegate.Broadcast(ShopSlotWidget);
	}

	RefreshShop();
}

void AOverworldGameMode::RefreshShop()
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	TArray<FItemBaseStruct> Inventory = PlayerController->GetInventory();

	for (UItemShopSlotWidget* Slot : ShopSlots) {

		FItemBaseStruct Item = Slot->GetItem();

		int Count = 0;

		if (Inventory.Contains(Item)) {

			for (FItemBaseStruct ItemToSearch : Inventory)
			{
				if (Item == ItemToSearch)
				{
					Count++;
				}
			}

			Slot->SetItemCount(Count);
			Slot->SetSellState(true);
		}
		else {
			Slot->SetItemCount(Count);
			Slot->SetSellState(false);
		}


		if (PlayerController->GetMoney() >= Item.Value)
		{
			Slot->SetBuyState(true);
		}
		else {
			Slot->SetBuyState(false);
		}
	}

	ShopMessageDelegate.Broadcast("You currently have " + FString::FromInt(PlayerController->GetMoney()) + "$");
}

void AOverworldGameMode::BuyItem(FItemBaseStruct Item)
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PlayerController->ObtainItem(Item);
	PlayerController->LoseMoney(Item.Value);

	RefreshShop();
}

void AOverworldGameMode::SellItem(FItemBaseStruct Item)
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PlayerController->LoseItem(Item);
	PlayerController->RecieveMoney(Item.Value);

	RefreshShop();
}

void AOverworldGameMode::ShowPokemonInMenu()
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	TArray<FPokemonStruct> Party = PlayerController->GetPokemonParty();

	for (int i = 0; i < Party.Num(); i++) {
		AOverworldHUD* Hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
		UPokemonSlotWidget* PokemonSlotWidget = CreateWidget<UPokemonSlotWidget>(UGameplayStatics::GetGameInstance(GetWorld()), Hud->GetPokemonSlotWidgetClass());

		PokemonSlotWidget->SetPokemonName(Party[i].SpeciesData.Name);
		PokemonSlotWidget->SetPokemonLevel(Party[i].Level);
		PokemonSlotWidget->SetPokemonImage(Party[i].SpeciesData.Image);
		PokemonSlotWidget->SetPokemonHP(Party[i].CurrHP, Party[i].MaxHP);
		PokemonSlotWidget->SetPokemonEXP(Party[i].CurrExp, Party[i].RequiredExp);
		PokemonSlotWidget->SetPokemon(i);

		PokemonSlotWidget->PokemonClick.AddDynamic(this, &AOverworldGameMode::ShowPokemonSummary);

		PokemonSlotDelegate.Broadcast(PokemonSlotWidget);
	}
}

void AOverworldGameMode::ShowPokemonSummary(int PokemonID)
{
	AOverworldHUD* Hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	UPokemonSummaryWidget* PokemonSummary = CreateWidget<UPokemonSummaryWidget>(UGameplayStatics::GetGameInstance(GetWorld()), Hud->GetPokemonSummaryWidgetClass());

	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	FPokemonStruct Pokemon = PlayerController->GetPokemonParty()[PokemonID];

	FString PokemonType;
	if (Pokemon.SpeciesData.Type2 == ETypes::None) {
		PokemonType = ETypeToString(Pokemon.SpeciesData.Type1);
	}
	else {
		PokemonType = ETypeToString(Pokemon.SpeciesData.Type1) + " " + ETypeToString(Pokemon.SpeciesData.Type2);
	}

	FString PokemonHP = FString::FromInt(Pokemon.CurrHP) + "/" + FString::FromInt(Pokemon.MaxHP);

	PokemonSummary->SetImage(Pokemon.SpeciesData.Image);
	PokemonSummary->SetGeneralInfo(Pokemon.SpeciesData.Name, Pokemon.SpeciesData.PokemonID, PokemonType, Pokemon.Level, (Pokemon.RequiredExp - Pokemon.CurrExp));
	PokemonSummary->SetStats(PokemonHP, Pokemon.Attack, Pokemon.Defence, Pokemon.Speed);


	for (int i = 0; i < Pokemon.CurrentMoves.Num(); i++) {
		UMoveButtonWidget* MoveButton = CreateWidget<UMoveButtonWidget>(UGameplayStatics::GetGameInstance(GetWorld()), Hud->GetMoveButtonWidgetClass());

		MoveButton->InitButton(Pokemon.Moves[Pokemon.CurrentMoves[i]].Name, Pokemon.Moves[Pokemon.CurrentMoves[i]].CurrPowerPoints,
			Pokemon.Moves[Pokemon.CurrentMoves[i]].PowerPoints, Pokemon.Moves[Pokemon.CurrentMoves[i]].MoveType);
		PokemonSummary->AddMove(MoveButton);
	}

	PokemonSummaryDelegate.Broadcast(PokemonSummary);
}

void AOverworldGameMode::FillPokedex()
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	AOverworldHUD* Hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	TArray<FPokemonBaseStruct*> AllPokemonData;
	PokemonDT->GetAllRows<FPokemonBaseStruct>(TEXT("PokemonDT"), AllPokemonData);

	for (FPokemonBaseStruct* PokemonSpecies : AllPokemonData) {
		UPokedexSlotWidget* PokedexSlotWidget = CreateWidget<UPokedexSlotWidget>(UGameplayStatics::GetGameInstance(GetWorld()), Hud->GetPokedexSlotWidgetClass());

		if (PlayerController->bIsRegisteredInPokedex(PokemonSpecies->PokemonID)) {
			PokedexSlotWidget->InitFilledSlot(*PokemonSpecies);
			PokedexSlotWidget->SlotClicked.AddDynamic(this, &AOverworldGameMode::ShowPokedexInfo);
		}
		else {
			PokedexSlotWidget->InitEmptySlot(PokemonSpecies->PokemonID);

		}

		PokedexSlotDelegate.Broadcast(PokedexSlotWidget);
	}
}

void AOverworldGameMode::ShowPokedexInfo(FPokemonBaseStruct PokemonData)
{
	AOverworldHUD* Hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	UPokedexInfoWidget* PokedexInfo = CreateWidget<UPokedexInfoWidget>(UGameplayStatics::GetGameInstance(GetWorld()), Hud->GetPokedexInfoWidgetClass());

	FString PokemonType;
	if (PokemonData.Type2 == ETypes::None) {
		PokemonType = ETypeToString(PokemonData.Type1);
	}
	else {
		PokemonType = ETypeToString(PokemonData.Type1) + " " + ETypeToString(PokemonData.Type2);
	}

	PokedexInfo->SetPokedexInfo(PokemonData, PokemonType);
	PokedexInfoDelegate.Broadcast(PokedexInfo);
}

void AOverworldGameMode::TogglePause()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	bIsPaused = !bIsPaused;
	PlayerController->SetPause(bIsPaused);
	OnGamePaused.Broadcast(bIsPaused);
}

TArray<class UDataTable*> AOverworldGameMode::GetItemDT() const
{
	return ItemDT;
}

TArray<class UDataTable*> AOverworldGameMode::GetMoveDT() const
{
	return MoveDT;
}
