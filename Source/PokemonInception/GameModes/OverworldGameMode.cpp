// Copyright Epic Games, Inc. All Rights Reserved.

#include "OverworldGameMode.h"
#include "GameFramework/Controller.h"
#include "GameFramework/WorldSettings.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "../Player/PokemonInceptionCharacter.h"
#include "../Player/PlayerCharacterController.h"
#include "../UI/HUD/OverworldHUD.h"
#include "../Items/ExpCandyBaseStruct.h"
#include "../Items/EtherBaseStruct.h"
#include "../Items/PotionBaseStruct.h"
#include "../Items/ReviveBaseStruct.h"
#include "../SaveGame/WorldSaveData.h"
#include "../SaveGame/WildPokemonSaveData.h"
#include "../WildPokemon/WildPokemonSpawner.h"
#include "../WildPokemon/WildPokemon.h"


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

	UPlayerSaveData* SaveData = Cast<UPlayerSaveData>(UGameplayStatics::CreateSaveGameObject(UPlayerSaveData::StaticClass()));

	if (UGameplayStatics::DoesSaveGameExist("PlayerSaveSlot", 0)) {
		SaveData = Cast<UPlayerSaveData>(UGameplayStatics::LoadGameFromSlot("PlayerSaveSlot", 0));

		SaveData->OpponentData.Empty();

		PlayerController->Inventory = SaveData->InventoryData;
		PlayerController->Money = SaveData->MoneyData;
		PlayerController->PokemonParty = SaveData->PartyData;
		PlayerController->PokemonStorage = SaveData->StorageData;
		PlayerController->Pokedex = SaveData->PokedexData;
	}
	
	LoadLevelData();

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

	UPlayerSaveData* SaveData = Cast<UPlayerSaveData>(UGameplayStatics::CreateSaveGameObject(UPlayerSaveData::StaticClass()));

	SaveData->InventoryData = PlayerController->Inventory;
	SaveData->PokedexData = PlayerController->Pokedex;
	SaveData->MoneyData = PlayerController->Money;
	SaveData->PartyData = PlayerController->PokemonParty;
	SaveData->StorageData = PlayerController->PokemonStorage;

	UGameplayStatics::SaveGameToSlot(SaveData, "PlayerSaveSlot", 0);
}

void AOverworldGameMode::MarkActorAsDestroyed(AActor* Actor)
{
	ActorsToDestroy.Add(Actor);
}

void AOverworldGameMode::InitiateBattle(TArray<FPokemonStruct> OpponentTeam, bool bIsOpponentTrainer)
{
	UPlayerSaveData* SaveData = Cast<UPlayerSaveData>(UGameplayStatics::CreateSaveGameObject(UPlayerSaveData::StaticClass()));

	if (UGameplayStatics::DoesSaveGameExist("PlayerSaveSlot", 0)) {
		SaveData = Cast<UPlayerSaveData>(UGameplayStatics::LoadGameFromSlot("PlayerSaveSlot", 0));

		SaveData->OpponentData = OpponentTeam;
		SaveData->bIsOpponentTrainer = bIsOpponentTrainer;
	}

	UGameplayStatics::SaveGameToSlot(SaveData, "PlayerSaveSlot", 0);

	UGameplayStatics::OpenLevel(GetWorld(), FName("BattleMap"));
}

void AOverworldGameMode::SaveLevelData(AWildPokemon* PokemonToIgnore)
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController == nullptr) {
		return;
	}

	APokemonInceptionCharacter* PlayerOwner = Cast<APokemonInceptionCharacter>(PlayerController->GetPawn());
	if (PlayerOwner == nullptr) {
		return;
	}

	ULevelSaveData* SaveData = Cast<ULevelSaveData>(UGameplayStatics::CreateSaveGameObject(ULevelSaveData::StaticClass()));

	SaveData->PokemonSpawners.Empty();

	SaveData->PlayerLocation = PlayerOwner->GetActorLocation();
	SaveData->PlayerRotation = PlayerOwner->GetActorRotation();
	SaveData->ActorsToDestroy = ActorsToDestroy;

	TArray<AActor*> Spawners;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), SpawnerClass, Spawners);
	
	for (AActor* Spawner : Spawners) {
		AWildPokemonSpawner* PokemonSpawner = Cast<AWildPokemonSpawner>(Spawner);
		
		if (IsValid(PokemonSpawner->SpawnedPokemon) && PokemonSpawner->SpawnedPokemon != PokemonToIgnore) {
			FWildPokemonData PokemonData;

			PokemonData.Pokemon = PokemonSpawner->SpawnedPokemon;
			PokemonData.PokemonStruct = PokemonSpawner->SpawnedPokemon->Pokemon;
			PokemonData.PokemonClass = PokemonSpawner->SpawnedPokemon->GetClass();
			PokemonData.PokemonLocation = PokemonSpawner->SpawnedPokemon->GetActorLocation();
			PokemonData.PokemonRotation = PokemonSpawner->SpawnedPokemon->GetActorRotation();

			SaveData->PokemonSpawners.Add(PokemonSpawner, PokemonData);
		}
	}

	UGameplayStatics::SaveGameToSlot(SaveData, "WorldSaveSlot", 0);
}

void AOverworldGameMode::LoadLevelData()
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController == nullptr) {
		return;
	}

	APokemonInceptionCharacter* PlayerOwner = Cast<APokemonInceptionCharacter>(PlayerController->GetPawn());
	if (PlayerOwner == nullptr) {
		return;
	}

	ULevelSaveData* SaveData = Cast<ULevelSaveData>(UGameplayStatics::CreateSaveGameObject(ULevelSaveData::StaticClass()));

	if (UGameplayStatics::DoesSaveGameExist("WorldSaveSlot", 0)) {
		SaveData = Cast<ULevelSaveData>(UGameplayStatics::LoadGameFromSlot("WorldSaveSlot", 0));

		if (PlayerController->bIsPartyDefeated() == true) {
			PlayerController->FullRestoreAllPokemon();
		}
		else {
			PlayerOwner->SetActorLocation(SaveData->PlayerLocation);
			PlayerOwner->SetActorRotation(SaveData->PlayerRotation, ETeleportType::None);
		}

		ActorsToDestroy = SaveData->ActorsToDestroy;

		for (AActor* Actor : ActorsToDestroy) {
			if (IsValid(Actor)) {
				Actor->Destroy();
			}
		}

		for (auto& Spawner : SaveData->PokemonSpawners) {
			Spawner.Key->ManualGenerate(Spawner.Value);
		}
	}
}

void AOverworldGameMode::SaveAndExit()
{
	SaveGame();
	SaveLevelData(nullptr);
	UGameplayStatics::OpenLevel(GetWorld(), "TitleMap");
}

void AOverworldGameMode::SelectMove(int InId)
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	SelectedMoveID = InId;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Using ether on " + PlayerController->PokemonParty[SelectedPokemonID].SpeciesData.Name.ToString() +
		" to restore " + PlayerController->PokemonParty[SelectedPokemonID].Moves[SelectedMoveID].Name.ToString()));

	UseItem();
}

void AOverworldGameMode::SelectPokemon(int InId)
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	AOverworldHUD* Hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	if (Hud->BIsMovePopupInViewport()) {
		return;
	}

	SelectedPokemonID = InId;

	if (bHasSelectedItem) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Using item on " + PlayerController->PokemonParty[SelectedPokemonID].SpeciesData.Name.ToString()));
		UseItem();
	}
	else {
		bIsSwappingPosition = true;
		ShowPokemonInMenu();
		Hud->ClearPopup();
		Hud->OnScreenMessage("Select a pokemon to swap positions");
	}
}

void AOverworldGameMode::SelectItem(int InId)
{
	AOverworldHUD* Hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController == nullptr) {
		return;
	}

	FItemBaseStruct Item = PlayerController->Inventory[InId];

	if (!Item.bUsableOutsideBattle) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Can't use this item!"));
		return;
	}

	if (Item.ItemStructType == "Ether") {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Using an ether!"));
		bHasSelectedEther = true;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Using a healing item/candy!"));
		bHasSelectedItem = true;
	}

	SelectedItemID = InId;
	Hud->ShowPokemon();
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

void AOverworldGameMode::FillBagWidget()
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	TArray<FItemBaseStruct> Inventory = PlayerController->Inventory;

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
		ItemSlotWidget->SetItemID(Inventory.Find(UniqueItems[i]));

		ItemSlotWidget->ItemClicked.AddDynamic(Hud, &AOverworldHUD::ShowItemInfo);

		ItemSlotDelegate.Broadcast(ItemSlotWidget);
	}
}

void AOverworldGameMode::InitShop(TArray<FName> ItemsToSell)
{
	ShopSlots.Empty();
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	TArray<FItemBaseStruct> Inventory = PlayerController->Inventory;

	TArray<FItemBaseStruct> ItemsInShop;

	for (FName ItemID : ItemsToSell) 
	{
		for (UDataTable* ItemTable : GetItemDT()) 
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
	TArray<FItemBaseStruct> Inventory = PlayerController->Inventory;

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


		if (PlayerController->Money >= Item.Value)
		{
			Slot->SetBuyState(true);
		}
		else {
			Slot->SetBuyState(false);
		}
	}

	ShopMessageDelegate.Broadcast("You currently have " + FString::FromInt(PlayerController->Money) + "$");
}

void AOverworldGameMode::BuyItem(FItemBaseStruct Item)
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PlayerController->ObtainItem(Item);
	PlayerController->Money -= Item.Value;

	RefreshShop();
}

void AOverworldGameMode::SellItem(FItemBaseStruct Item)
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PlayerController->LoseItem(Item);
	PlayerController->Money += Item.Value;

	RefreshShop();
}

void AOverworldGameMode::ShowPokemonInMenu()
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	AOverworldHUD* Hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	Hud->ClearPokemonSlots();

	TArray<FPokemonStruct> Party = PlayerController->PokemonParty;

	for (int i = 0; i < Party.Num(); i++) {
		
		UPokemonSlotWidget* PokemonSlotWidget = CreateWidget<UPokemonSlotWidget>(UGameplayStatics::GetGameInstance(GetWorld()), Hud->GetPokemonSlotWidgetClass());
		PokemonSlotWidget->SetPokemonName(Party[i].SpeciesData.Name);
		PokemonSlotWidget->SetPokemonLevel(Party[i].Level);
		PokemonSlotWidget->SetPokemonImage(Party[i].SpeciesData.Image);
		PokemonSlotWidget->SetPokemonHP(Party[i].CurrHP, Party[i].MaxHP);
		PokemonSlotWidget->SetPokemonEXP(Party[i].CurrExp, Party[i].RequiredExp);
		PokemonSlotWidget->SetPokemon(i);

		PokemonSlotWidget->PokemonClick.AddDynamic(Hud, &AOverworldHUD::ShowPokemonSummary);

		if (bHasSelectedItem) {
			PokemonSlotWidget->PokemonClick.AddDynamic(Hud, &AOverworldHUD::ShowUseItemPopup);
		}

		else if (bHasSelectedEther) {
			PokemonSlotWidget->PokemonClick.AddDynamic(Hud, &AOverworldHUD::ShowMoveSelectionPopup);
			PokemonSlotWidget->PokemonClick.AddDynamic(this, &AOverworldGameMode::SelectPokemon);
		}

		else if (bIsSwappingPosition) {
			PokemonSlotWidget->PokemonClick.AddDynamic(this, &AOverworldGameMode::SwapPositionWith);
		}

		else {
			PokemonSlotWidget->PokemonClick.AddDynamic(Hud, &AOverworldHUD::ShowSwapPositionPopup);
		}

		PokemonSlotDelegate.Broadcast(PokemonSlotWidget);
	}
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
			PokedexSlotWidget->SlotClicked.AddDynamic(Hud, &AOverworldHUD::ShowPokedexInfo);
		}
		else {
			PokedexSlotWidget->InitEmptySlot(PokemonSpecies->PokemonID);

		}

		PokedexSlotDelegate.Broadcast(PokedexSlotWidget);
	}
}

void AOverworldGameMode::ShowPokemonInStorage()
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	AOverworldHUD* Hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	Hud->ClearPokemonIcons();

	TArray<FPokemonStruct> Party = PlayerController->PokemonParty;
	TArray<FPokemonStruct> Storage = PlayerController->PokemonStorage;

	for (int i = 0; i < Party.Num(); i++) {

		UPokemonIconWidget* PokemonIconWidget = CreateWidget<UPokemonIconWidget>(UGameplayStatics::GetGameInstance(GetWorld()), Hud->GetPokemonIconWidgetClass());
		PokemonIconWidget->SetPokemonName(Party[i].SpeciesData.Name);
		PokemonIconWidget->SetPokemonLevel(Party[i].Level);
		PokemonIconWidget->SetPokemonImage(Party[i].SpeciesData.Image);
		PokemonIconWidget->SetPokemonHP(Party[i].CurrHP, Party[i].MaxHP);
		PokemonIconWidget->SetPokemon(i);

		PokemonIconWidget->PokemonClick.AddDynamic(Hud, &AOverworldHUD::ShowDepositPopup);

		PartyPokemonIconDelegate.Broadcast(PokemonIconWidget);
	}

	for (int i = 0; i < Storage.Num(); i++) {

		UPokemonIconWidget* PokemonIconWidget = CreateWidget<UPokemonIconWidget>(UGameplayStatics::GetGameInstance(GetWorld()), Hud->GetPokemonIconWidgetClass());
		PokemonIconWidget->SetPokemonName(Storage[i].SpeciesData.Name);
		PokemonIconWidget->SetPokemonLevel(Storage[i].Level);
		PokemonIconWidget->SetPokemonImage(Storage[i].SpeciesData.Image);
		PokemonIconWidget->SetPokemonHP(Storage[i].CurrHP, Storage[i].MaxHP);
		PokemonIconWidget->SetPokemon(i);

		PokemonIconWidget->PokemonClick.AddDynamic(Hud, &AOverworldHUD::ShowWithdrawPopup);

		StoragePokemonIconDelegate.Broadcast(PokemonIconWidget);
	}
}

FPokemonBaseStruct AOverworldGameMode::GetPokemonSpeciesData(FName PokemonID)
{
	FPokemonBaseStruct* SpeciesData = PokemonDT->FindRow<FPokemonBaseStruct>(PokemonID, "");
	return *SpeciesData;
}

void AOverworldGameMode::TogglePause()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	bIsPaused = !bIsPaused;
	PlayerController->SetPause(bIsPaused);
	OnGamePaused.Broadcast(bIsPaused);
}

void AOverworldGameMode::UseItem()
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	AOverworldHUD* Hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	FString ItemMessage = "You used a " + PlayerController->Inventory[SelectedItemID].Name.ToString() + ", ";

	if (PlayerController->Inventory[SelectedItemID].ItemStructType == "Potion") {
		if (PlayerController->PokemonParty[SelectedPokemonID].bIsFainted == true || PlayerController->PokemonParty[SelectedPokemonID].bIsFullHp() == true) {
			ItemMessage += "but it failed!";
		}

		else {
			FPotionBaseStruct* Potion = PotionsDT->FindRow<FPotionBaseStruct>(PlayerController->Inventory[SelectedItemID].ItemID, "");
			PlayerController->PokemonParty[SelectedPokemonID].RestoreHP(Potion->RestoredHP);

			PlayerController->Inventory.RemoveAt(SelectedItemID);

			ItemMessage += PlayerController->PokemonParty[SelectedPokemonID].SpeciesData.Name.ToString() + "'s HP was restored!";
		}
	}

	else if (PlayerController->Inventory[SelectedItemID].ItemStructType == "Revive") {
		if (PlayerController->PokemonParty[SelectedPokemonID].bIsFainted == false) {
			ItemMessage += "but it failed!";
		}

		else {
			FReviveBaseStruct* Revive = RevivesDT->FindRow<FReviveBaseStruct>(PlayerController->Inventory[SelectedItemID].ItemID, "");
			PlayerController->PokemonParty[SelectedPokemonID].RecoverStatus();
			PlayerController->PokemonParty[SelectedPokemonID].RestoreHP(PlayerController->PokemonParty[SelectedPokemonID].MaxHP * Revive->RevivePercent);

			PlayerController->Inventory.RemoveAt(SelectedItemID);

			ItemMessage += PlayerController->PokemonParty[SelectedPokemonID].SpeciesData.Name.ToString() + " was revived!";
		}
	}

	else if (PlayerController->Inventory[SelectedItemID].ItemStructType == "Ether") {
		if (PlayerController->PokemonParty[SelectedPokemonID].Moves[SelectedMoveID].bHasMaxPP() == true) {
			ItemMessage += "but it failed!";
		}

		else {
			FEtherBaseStruct* Ether = EthersDT->FindRow<FEtherBaseStruct>(PlayerController->Inventory[SelectedItemID].ItemID, "");
			PlayerController->PokemonParty[SelectedPokemonID].RestorePP(Ether->RestoredPP, SelectedMoveID);

			PlayerController->Inventory.RemoveAt(SelectedItemID);
			ItemMessage += "restored PP to " + PlayerController->PokemonParty[SelectedPokemonID].SpeciesData.Name.ToString() +
				"'s move " + PlayerController->PokemonParty[SelectedPokemonID].Moves[SelectedMoveID].Name.ToString() + "!";
		}
	}

	else if (PlayerController->Inventory[SelectedItemID].ItemStructType == "Candy") {
		if (PlayerController->PokemonParty[SelectedPokemonID].Level == 100) {
			ItemMessage += "but it failed!";
		}

		else {
			FExpCandyBaseStruct* Candy = CandyDT->FindRow<FExpCandyBaseStruct>(PlayerController->Inventory[SelectedItemID].ItemID, "");
			
			if (PlayerController->PokemonParty[SelectedPokemonID].GainExp(Candy->ExpRecieved) == true) {
				PlayerController->PokemonParty[SelectedPokemonID].CheckForNewMoves();
			}

			PlayerController->Inventory.RemoveAt(SelectedItemID);
			ItemMessage += PlayerController->PokemonParty[SelectedPokemonID].SpeciesData.Name.ToString() + " gained some Exp!";
		}
	}
	bHasSelectedItem = false;
	bHasSelectedEther = false;

	Hud->ShowBag();
	Hud->OnScreenMessage(ItemMessage);
}

void AOverworldGameMode::SwapPositionWith(int NewPositionId)
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	AOverworldHUD* Hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	PlayerController->PokemonParty.Swap(SelectedPokemonID, NewPositionId);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Swapping " + PlayerController->PokemonParty[SelectedPokemonID].SpeciesData.Name.ToString()
	+ " and " + PlayerController->PokemonParty[NewPositionId].SpeciesData.Name.ToString()));

	bIsSwappingPosition = false;

	Hud->ClearOnScreenMessage();
	ShowPokemonInMenu();
}

TArray<class UDataTable*> AOverworldGameMode::GetItemDT() const
{
	TArray<class UDataTable*> ItemDT;

	ItemDT.Add(BallsDT);
	ItemDT.Add(PotionsDT);
	ItemDT.Add(ValuablesDT);
	ItemDT.Add(EthersDT);
	ItemDT.Add(RevivesDT);
	ItemDT.Add(CandyDT);

	return ItemDT;
}

TArray<class UDataTable*> AOverworldGameMode::GetMoveDT() const
{
	TArray<class UDataTable*> MoveDT;

	MoveDT.Add(AttackMovesDT);
	MoveDT.Add(StatusMovesDT);

	return MoveDT;
}
