// Copyright Epic Games, Inc. All Rights Reserved.

#include "OverworldGameMode.h"
#include "GameFramework/Controller.h"
#include "GameFramework/WorldSettings.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "../Settings/PokemonInceptionGameInstance.h"
#include "../Player/PokemonInceptionCharacter.h"
#include "../Player/PlayerCharacterController.h"
#include "../UI/HUD/OverworldHUD.h"
#include "../Interactables/PickUp/PickupBase.h"
#include "../Items/ExpCandyBaseStruct.h"
#include "../Items/EtherBaseStruct.h"
#include "../Items/PotionBaseStruct.h"
#include "../Items/ReviveBaseStruct.h"
#include "../SaveGame/PlayerSaveData.h"
#include "../SaveGame/LevelSaveData.h"
#include "../WildPokemon/WildPokemonSpawner.h"
#include "../WildPokemon/WildPokemon.h"

#include "GameFramework/GameUserSettings.h"

void AOverworldGameMode::BeginPlay()
{
	Super::BeginPlay();

	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	UPlayerSaveData* SaveData = Cast<UPlayerSaveData>(UGameplayStatics::CreateSaveGameObject(UPlayerSaveData::StaticClass()));

	if (UGameplayStatics::DoesSaveGameExist("PlayerSaveSlot", 0)) {
		SaveData = Cast<UPlayerSaveData>(UGameplayStatics::LoadGameFromSlot("PlayerSaveSlot", 0));

		SaveData->OpponentData.Empty();

		PlayerController->PlayerName = SaveData->PlayerName;
		PlayerController->Inventory = SaveData->InventoryData;
		PlayerController->Money = SaveData->MoneyData;
		PlayerController->PokemonParty = SaveData->PartyData;
		PlayerController->PokemonStorage = SaveData->StorageData;
		PlayerController->Pokedex = SaveData->PokedexData;
		PlayerController->TrainersDefeated = SaveData->TrainersDefeated;
	}

	TArray<AActor*> Trainers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), TrainerClass, Trainers);
	
	for (AActor* i : Trainers) {
		ATrainer* Trainer = Cast<ATrainer>(i);

		if (PlayerController->TrainersDefeated > Trainer->DefeatedTrainersRequiredToChallange) {
			Trainer->bHasBeenDefeated = true;
		}
	}

	if (PlayerController->PokemonParty.IsEmpty()) {
		FPokemonBaseStruct* PokemonStarterSpecies = PokemonDT->FindRow<FPokemonBaseStruct>(FName(*FString::FromInt(4)), "");
		FPokemonStruct StarterPokemon;

		StarterPokemon.Init(5, *PokemonStarterSpecies);
		StarterPokemon.InitMoves(GetMoveDT());

		PlayerController->ObtainPokemon(StarterPokemon);
	}

	LoadLevelData();

	if (BGM) {
		UGameplayStatics::PlaySound2D(GetWorld(), BGM);
	}

	PlayerController->PauseDelegate.AddDynamic(this, &AOverworldGameMode::ToggleMainMenu);
	PlayerController->PeacefulModeDelegate.AddDynamic(this, &AOverworldGameMode::TogglePeacefulMode);
	OnGamePaused.AddDynamic(Cast<AOverworldHUD>(PlayerController->GetHUD()), &AOverworldHUD::ToggleMainMenu);

	FLatentActionInfo LatentInfo;
	UGameplayStatics::LoadStreamLevel(this, FName("BattleMap"), false, true, LatentInfo);

	ALevelSequenceActor* SequenceActor;
	SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), EncounterSequence, FMovieSceneSequencePlaybackSettings(), SequenceActor);
}

void AOverworldGameMode::SaveGame()
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	UPlayerSaveData* SaveData = Cast<UPlayerSaveData>(UGameplayStatics::CreateSaveGameObject(UPlayerSaveData::StaticClass()));

	SaveData->PlayerName = PlayerController->PlayerName;
	SaveData->InventoryData = PlayerController->Inventory;
	SaveData->PokedexData = PlayerController->Pokedex;
	SaveData->MoneyData = PlayerController->Money;
	SaveData->PartyData = PlayerController->PokemonParty;
	SaveData->StorageData = PlayerController->PokemonStorage;
	SaveData->TrainersDefeated = PlayerController->TrainersDefeated;

	UGameplayStatics::SaveGameToSlot(SaveData, "PlayerSaveSlot", 0);
}

void AOverworldGameMode::MarkActorAsDestroyed(AActor* Actor)
{
	ULevelSaveData* LevelSaveData = Cast<ULevelSaveData>(UGameplayStatics::CreateSaveGameObject(ULevelSaveData::StaticClass()));

	if (UGameplayStatics::DoesSaveGameExist("WorldSaveSlot", 0)) {
		LevelSaveData = Cast<ULevelSaveData>(UGameplayStatics::LoadGameFromSlot("WorldSaveSlot", 0));
	}

	APickupBase* PickUp = Cast<APickupBase>(Actor);
	LevelSaveData->PickUpsToDestroy.Add(PickUp->ID);

	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, "Marked PickUp #" + PickUp->ID.ToString() + " as destroyed");
	UGameplayStatics::SaveGameToSlot(LevelSaveData, "WorldSaveSlot", 0);
}

void AOverworldGameMode::InitiateBattle(TArray<FPokemonStruct> OpponentTeam, bool bIsOpponentTrainer, bool bHasDefeatedTrainerBefore)
{
	UPlayerSaveData* SaveData = Cast<UPlayerSaveData>(UGameplayStatics::CreateSaveGameObject(UPlayerSaveData::StaticClass()));

	if (UGameplayStatics::DoesSaveGameExist("PlayerSaveSlot", 0)) {
		SaveData = Cast<UPlayerSaveData>(UGameplayStatics::LoadGameFromSlot("PlayerSaveSlot", 0));

		SaveData->OpponentData = OpponentTeam;
		SaveData->bIsOpponentTrainer = bIsOpponentTrainer;
		SaveData->bHasDefeatedTrainerBefore = bHasDefeatedTrainerBefore;
	}

	UGameplayStatics::SaveGameToSlot(SaveData, "PlayerSaveSlot", 0);

	UGameplayStatics::OpenLevel(GetWorld(), FName("BattleMap"));
}

void AOverworldGameMode::SaveLevelData(AWildPokemon* PokemonToIgnore)
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	APokemonInceptionCharacter* PlayerPawn = Cast<APokemonInceptionCharacter>(PlayerController->GetPawn());
	if (!IsValid(PlayerPawn)) {
		return;
	}

	ULevelSaveData* LevelSaveData = Cast<ULevelSaveData>(UGameplayStatics::CreateSaveGameObject(ULevelSaveData::StaticClass()));

	if (!UGameplayStatics::DoesSaveGameExist("WorldSaveSlot", 0)) {
		return;
	}
	LevelSaveData = Cast<ULevelSaveData>(UGameplayStatics::LoadGameFromSlot("WorldSaveSlot", 0));

	LevelSaveData->PokemonSpawners.Empty();

	LevelSaveData->PlayerLocation = PlayerPawn->GetActorLocation();
	LevelSaveData->PlayerRotation = PlayerPawn->GetActorRotation();

	if (!PokemonInLevel.IsEmpty()) {
		TArray<AActor*> Spawners;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), SpawnerClass, Spawners);

		for (AActor* Spawner : Spawners) {
			AWildPokemonSpawner* PokemonSpawner = Cast<AWildPokemonSpawner>(Spawner);
			AWildPokemon* PokemonRef = PokemonSpawner->SpawnedPokemon;

			if (PokemonRef == nullptr) {
				continue;
			}

			if (PokemonRef != PokemonToIgnore) {
				FWildPokemonData PokemonData;

				PokemonData.PokemonLevel = PokemonRef->Pokemon.Level;
				PokemonData.PokemonClass = PokemonRef->GetClass();

				PokemonData.PokemonLocation = PokemonRef->GetActorLocation();
				PokemonData.PokemonRotation = PokemonRef->GetActorRotation();

				LevelSaveData->PokemonSpawners.Add(PokemonSpawner, PokemonData);
			}
		}
	}

	UGameplayStatics::SaveGameToSlot(LevelSaveData, "WorldSaveSlot", 0);
}

void AOverworldGameMode::LoadLevelData()
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	APokemonInceptionCharacter* PlayerPawn = Cast<APokemonInceptionCharacter>(PlayerController->GetPawn());
	if (!IsValid(PlayerPawn)) {
		return;
	}

	ULevelSaveData* LevelSaveData = Cast<ULevelSaveData>(UGameplayStatics::CreateSaveGameObject(ULevelSaveData::StaticClass()));

	if (UGameplayStatics::DoesSaveGameExist("WorldSaveSlot", 0)) {
		LevelSaveData = Cast<ULevelSaveData>(UGameplayStatics::LoadGameFromSlot("WorldSaveSlot", 0));

		if (PlayerController->bIsPartyDefeated() == true) {
			PlayerController->FullRestoreAllPokemon();
		}
		else {
			PlayerPawn->ChangePositionInWorld(LevelSaveData->PlayerLocation, LevelSaveData->PlayerRotation);
			
		}

		TArray<AActor*> PickUps;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APickupBase::StaticClass(), PickUps);

		for (AActor* Actor : PickUps) {
			APickupBase* PickUp = Cast<APickupBase>(Actor);

			if (!LevelSaveData->PickUpsToDestroy.Contains(PickUp->ID)) {
				//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, "PickUp still exists, continuing");
				continue;
			}

			if (!IsValid(PickUp)) {
				//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, "PickUp is not valid, continuing");
				continue;
			}

			//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, "Destroying PickUp #" + PickUp->ID.ToString());
			PickUp->Destroy();
		}

		for (auto& Spawner : LevelSaveData->PokemonSpawners) {
			Spawner.Key->ManualGenerate(Spawner.Value);
		}
	}

	else {
		TArray<AActor*> PickUps;
		TArray<FName> PickUpIDs;

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APickupBase::StaticClass(), PickUps);

		for (AActor* Actor : PickUps) {
			APickupBase* PickUp = Cast<APickupBase>(Actor);
			if (!IsValid(PickUp)) {
				continue;
			}

			if (PickUpIDs.Contains(PickUp->ID)) {
				GEngine->AddOnScreenDebugMessage(68, 20, FColor::Red, "ERROR: There are more than 1 PickUps with ID #" + PickUp->ID.ToString());
			}
			else {
				PickUpIDs.Add(PickUp->ID);
			}
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
	if (!IsValid(PlayerController)) {
		return;
	}

	SelectedMoveID = InId;
	UseItem();
}

void AOverworldGameMode::SelectPokemon(int InId)
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	AOverworldHUD* Hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	if (Hud->BIsMovePopupInViewport()) {
		return;
	}

	SelectedPokemonID = InId;

	if (bHasSelectedItem) {
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
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	AOverworldHUD* Hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	FItemBaseStruct Item = PlayerController->Inventory[InId];

	if (!Item.bUsableOutsideBattle) {
		return;
	}

	if (Item.ItemStructType == "Ether") {
		bHasSelectedEther = true;
	}

	else {
		bHasSelectedItem = true;
	}

	SelectedItemID = InId;
	Hud->ShowPokemon();
}

void AOverworldGameMode::FillBagWidget()
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	AOverworldHUD* Hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	TArray<FItemBaseStruct> Inventory = PlayerController->Inventory;
	TArray<FItemBaseStruct> UniqueItems;

	for (FItemBaseStruct Item : Inventory) {
		if (UniqueItems.Contains(Item) == false) {
			UniqueItems.Add(Item);
		}
	}

	for (int i = 0; i < UniqueItems.Num(); i++) {
		UItemSlotWidget* ItemSlotWidget = CreateWidget<UItemSlotWidget>(UGameplayStatics::GetGameInstance(GetWorld()), Hud->GetItemSlotWidgetClass());

		ItemSlotWidget->SetItemName(UniqueItems[i].Name);
		ItemSlotWidget->SetItemImage(UniqueItems[i].Image);
		ItemSlotWidget->SetItemCount(PlayerController->GetItemCount(UniqueItems[i].ItemID));
		ItemSlotWidget->SetItemID(Inventory.Find(UniqueItems[i]));

		ItemSlotWidget->ItemClicked.AddDynamic(Hud, &AOverworldHUD::ShowItemInfo);

		ItemSlotDelegate.Broadcast(ItemSlotWidget);
	}
}

void AOverworldGameMode::InitItemsToSell(TArray<FName> ItemIDs)
{
	ItemsToSell.Empty();

	for (FName ItemID : ItemIDs){
		for (UDataTable* ItemTable : GetItemDT()){
			FItemBaseStruct* Item = ItemTable->FindRow<FItemBaseStruct>(ItemID, "");

			if (Item){
				ItemsToSell.Add(*Item);
			}
		}
	}
}

void AOverworldGameMode::InitBuyShop()
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	AOverworldHUD* Hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}
	
	ShopMode = "Buy";

	for (int i = 0; i < ItemsToSell.Num(); i++) {
		UItemSlotWidget* ItemSlotWidget = CreateWidget<UItemSlotWidget>(UGameplayStatics::GetGameInstance(GetWorld()), Hud->GetItemSlotWidgetClass());

		ItemSlotWidget->SetItemName(ItemsToSell[i].Name);
		ItemSlotWidget->SetItemImage(ItemsToSell[i].Image);
		ItemSlotWidget->SetItemCount(PlayerController->GetItemCount(ItemsToSell[i].ItemID));
		ItemSlotWidget->SetItemID(i);

		ItemSlotWidget->ItemClicked.AddDynamic(Hud, &AOverworldHUD::ShowItemShopInfo);

		ItemShopSlotDelegate.Broadcast(ItemSlotWidget);
	}
}

void AOverworldGameMode::InitSellShop()
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	AOverworldHUD* Hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	ShopMode = "Sell";

	TArray<FItemBaseStruct> Inventory = PlayerController->Inventory;

	TArray<FItemBaseStruct> UniqueItems;

	for (FItemBaseStruct Item : Inventory) {
		if (UniqueItems.Contains(Item) == false) {
			UniqueItems.Add(Item);
		}
	}

	for (int i = 0; i < UniqueItems.Num(); i++) {
		UItemSlotWidget* ItemSlotWidget = CreateWidget<UItemSlotWidget>(UGameplayStatics::GetGameInstance(GetWorld()), Hud->GetItemSlotWidgetClass());

		ItemSlotWidget->SetItemName(UniqueItems[i].Name);
		ItemSlotWidget->SetItemImage(UniqueItems[i].Image);
		ItemSlotWidget->SetItemCount(PlayerController->GetItemCount(UniqueItems[i].ItemID));
		ItemSlotWidget->SetItemID(PlayerController->Inventory.Find(UniqueItems[i]));

		ItemSlotWidget->ItemClicked.AddDynamic(Hud, &AOverworldHUD::ShowItemShopInfo);

		ItemShopSlotDelegate.Broadcast(ItemSlotWidget);
	}
}

void AOverworldGameMode::BuyItem(int ItemID)
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	AOverworldHUD* Hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	if (ItemsToSell[ItemID].Value > PlayerController->Money) {
		Hud->OnScreenMessage("You don't have enough money!");
		return;
	}

	PlayerController->ObtainItem(ItemsToSell[ItemID]);
	PlayerController->Money -= ItemsToSell[ItemID].Value;

	Hud->ShowBuyShop();
}

void AOverworldGameMode::SellItem(int ItemID)
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	AOverworldHUD* Hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	if (PlayerController->Inventory[ItemID].Value + PlayerController->Money > 1000000) {
		Hud->OnScreenMessage("You can't obtain more money!");
		return;
	}

	PlayerController->Money += PlayerController->Inventory[ItemID].Value;
	PlayerController->LoseItem(PlayerController->Inventory[ItemID]);

	Hud->ShowSellShop();
}

void AOverworldGameMode::ShowPokemonInMenu()
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	AOverworldHUD* Hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

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
	if (!IsValid(PlayerController)) {
		return;
	}

	AOverworldHUD* Hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

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
	if (!IsValid(PlayerController)) {
		return;
	}

	AOverworldHUD* Hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

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

void AOverworldGameMode::PauseGame(EPause PauseType)
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	APokemonInceptionCharacter* PlayerPawn = Cast<APokemonInceptionCharacter>(PlayerController->GetPawn());
	if (!IsValid(PlayerPawn)) {
		return;
	}

	switch (PauseType) {
		case EPause::Pause: bIsPaused = true;
			break;
		case EPause::UnPause: bIsPaused = false;
			break;
		case EPause::Auto: bIsPaused = !bIsPaused;
			break;
	}

	if (bIsPaused) {
		PlayerPawn->CustomTimeDilation = 0;
		PlayerController->SetInputMode(FInputModeUIOnly());
	}
	else {
		PlayerPawn->CustomTimeDilation = 1;
		PlayerController->SetInputMode(FInputModeGameOnly());
	}

	if (!PokemonInLevel.IsEmpty()) {
		TArray<AActor*> Spawners;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), SpawnerClass, Spawners);

		for (AActor* Spawner : Spawners) {
			AWildPokemonSpawner* PokemonSpawner = Cast<AWildPokemonSpawner>(Spawner);
			AWildPokemon* PokemonRef = PokemonSpawner->SpawnedPokemon;

			if (PokemonRef == nullptr) {
				continue;
			}

			if (bIsPaused) {
				PokemonRef->CustomTimeDilation = 0;
			}
			else {
				PokemonRef->CustomTimeDilation = 1;
			}
		}
	}
}

bool AOverworldGameMode::bIsGamePaused()
{
	return bIsPaused;
}

void AOverworldGameMode::PlayEncounterSequence()
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	APokemonInceptionCharacter* PlayerPawn = Cast<APokemonInceptionCharacter>(PlayerController->GetPawn());
	if (!IsValid(PlayerPawn)) {
		return;
	}

	if (SequencePlayer && EncounterSequence){	
		SequencePlayer->Play();

		TArray<AActor*> CameraActorsToFind;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), CameraClass, CameraActorsToFind);

		for (AActor* CameraActor : CameraActorsToFind) {
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Camera change possition");
			CameraActor->SetActorLocation(PlayerPawn->GetCameraLocation());
			CameraActor->SetActorRotation(PlayerPawn->GetCameraRotation());
		}
	}
}

void AOverworldGameMode::ToggleMainMenu()
{
	PauseGame(EPause::Auto);
	OnGamePaused.Broadcast(bIsPaused);
}

void AOverworldGameMode::TogglePeacefulMode()
{
	AOverworldHUD* Hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	bIsPeacefulModeOn = !bIsPeacefulModeOn;

	if (bIsPeacefulModeOn) {
		Hud->OnScreenMessage("Peaceful Mode is enabled");
	}
	else {
		Hud->OnScreenMessage("Peaceful Mode is disabled");
	}
}

void AOverworldGameMode::UseItem()
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	AOverworldHUD* Hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

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
	if (!IsValid(PlayerController)) {
		return;
	}

	AOverworldHUD* Hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	PlayerController->PokemonParty.Swap(SelectedPokemonID, NewPositionId);
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
