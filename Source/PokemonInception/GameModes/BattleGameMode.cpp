// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "../UI/BattleUI/BattleHUD.h"
#include "../UI/BattleUI/MoveButtonWidget.h"
#include "../Player/PlayerCharacterController.h"
#include "../Player/PokemonInceptionCharacter.h"
#include "../Pokemon/StaticOverworldPokemon.h"


void ABattleGameMode::BeginPlay()
{
	Super::BeginPlay();

	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController == nullptr) {
		return;
	}

	UWorldSaveData* SaveData = Cast<UWorldSaveData>(UGameplayStatics::CreateSaveGameObject(UWorldSaveData::StaticClass()));

	if (UGameplayStatics::DoesSaveGameExist("SaveSlot", 0)) {
		SaveData = Cast<UWorldSaveData>(UGameplayStatics::LoadGameFromSlot("SaveSlot", 0));

		PlayerController->LoadInventory(SaveData->InventoryData);
		PlayerController->RecieveMoney(SaveData->MoneyData);
		PlayerController->LoadPokemonParty(SaveData->PartyData);
		OpponentTeam = SaveData->OpponentData;
	}

	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), Camera, FoundActors);

	PlayerController->SetViewTargetWithBlend(FoundActors[0], 0, EViewTargetBlendFunction::VTBlend_Linear);

	PlacePlayerPokemon(PlayerController->GetPokemonParty()[0]);
	PlaceOpponentPokemon(OpponentTeam[0]);

	BattleStart();
}

void ABattleGameMode::PlacePlayerPokemon(FPokemonStruct Pokemon)
{
	FRotator Rotation;
	FActorSpawnParameters SpawnInfo;
	FVector Position = FVector(-350, -270, 115);

	PlayerPokemonActor = GetWorld()->SpawnActor<AStaticOverworldPokemon>(Pokemon.SpeciesData.PokemonActor, Position, Rotation, SpawnInfo);
}

void ABattleGameMode::PlaceOpponentPokemon(FPokemonStruct Pokemon)
{
	FRotator Rotation;
	Rotation.Yaw += 180;
	FActorSpawnParameters SpawnInfo;
	FVector Position = FVector(-350, 440, 115);

	OpponentPokemonActor = GetWorld()->SpawnActor<AStaticOverworldPokemon>(Pokemon.SpeciesData.PokemonActor, Position, Rotation, SpawnInfo);
}

void ABattleGameMode::BattleStart()
{
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController == nullptr) {
		return;
	}

	FTimerDelegate OpponentMessageDelegate;
	FTimerDelegate PlayerMessageDelegate;

	FString OpponentMessage;

	if (OpponentTeam.Num() == 1) {
		OpponentMessage = "A wild " + OpponentTeam[0].SpeciesData.Name.ToString() + " appeared!";
	}
	else {
		OpponentMessage = "Trainer sent out " + OpponentTeam[0].SpeciesData.Name.ToString() + "!";
	}

	OpponentMessageDelegate.BindUFunction(Hud, FName("ShowText"), OpponentMessage);
	PlayerMessageDelegate.BindUFunction(Hud, FName("ShowText"), ("You sent out " + PlayerController->GetPokemonParty()[0].SpeciesData.Name.ToString() + "!"));

	GetWorldTimerManager().SetTimer(MessageTimer, OpponentMessageDelegate, 0.001, false);
	GetWorldTimerManager().SetTimer(MessageTimer2, PlayerMessageDelegate, 1.5, false);
	GetWorldTimerManager().SetTimer(WidgetDelay, Hud, &ABattleHUD::ShowBattleStartWidget, 3, false);
}

void ABattleGameMode::BattleEnd()
{
}

FString ABattleGameMode::ETypeToString(ETypes Type)
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

void ABattleGameMode::ShowPokemonInMenu()
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	TArray<FPokemonStruct> Party = PlayerController->GetPokemonParty();

	for (FPokemonStruct Pokemon : Party) {
		ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
		UPokemonSlotWidget* PokemonSlotWidget = CreateWidget<UPokemonSlotWidget>(UGameplayStatics::GetGameInstance(GetWorld()), Hud->GetPokemonSlotWidgetClass());

		PokemonSlotWidget->SetPokemonName(Pokemon.SpeciesData.Name);
		PokemonSlotWidget->SetPokemonLevel(Pokemon.Level);
		PokemonSlotWidget->SetPokemonImage(Pokemon.SpeciesData.Image);
		PokemonSlotWidget->SetPokemonHP(Pokemon.CurrHP, Pokemon.MaxHP);
		PokemonSlotWidget->SetPokemon(Pokemon);

		PokemonSlotWidget->PokemonClick.AddDynamic(this, &ABattleGameMode::ShowPokemonSummary);

		PokemonSlotDelegate.Broadcast(PokemonSlotWidget);
	}
}

void ABattleGameMode::ShowPokemonSummary(FPokemonStruct Pokemon)
{
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	UPokemonSummaryWidget* PokemonSummary = CreateWidget<UPokemonSummaryWidget>(UGameplayStatics::GetGameInstance(GetWorld()), Hud->GetPokemonSummaryWidgetClass());

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

	for (FMoveBaseStruct Move : Pokemon.CurrentMoves) {
		UMoveButtonWidget* MoveButton = CreateWidget<UMoveButtonWidget>(UGameplayStatics::GetGameInstance(GetWorld()), Hud->GetMoveButtonWidgetClass());

		MoveButton->InitButton(Move.Name, Move.CurrPowerPoints, Move.PowerPoints, Move.MoveType);
		PokemonSummary->AddMove(MoveButton);
	}

	PokemonSummaryDelegate.Broadcast(PokemonSummary);
}

void ABattleGameMode::FillBagWidget()
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
		ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
		UItemSlotWidget* ItemSlotWidget = CreateWidget<UItemSlotWidget>(UGameplayStatics::GetGameInstance(GetWorld()), Hud->GetItemSlotWidgetClass());

		ItemSlotWidget->SetItemName(UniqueItems[i].Name);
		ItemSlotWidget->SetItemImage(UniqueItems[i].Image);
		ItemSlotWidget->SetItemCount(ItemCount[i]);
		ItemSlotWidget->SetItem(UniqueItems[i]);

		ItemSlotWidget->ItemClicked.AddDynamic(this, &ABattleGameMode::ShowItemInfo);

		ItemSlotDelegate.Broadcast(ItemSlotWidget);
	}
}

void ABattleGameMode::ShowItemInfo(FItemBaseStruct InventoryItem)
{
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	UItemInfoWidget* ItemInfo = CreateWidget<UItemInfoWidget>(UGameplayStatics::GetGameInstance(GetWorld()), Hud->GetItemInfoWidgetClass());

	ItemInfo->SetDescription(InventoryItem.Description);

	ItemInfoDelegate.Broadcast(ItemInfo);
}

void ABattleGameMode::ShowPokemonMoves()
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	FPokemonStruct Attacker = (PlayerController->GetPokemonParty())[0];

	for (FMoveBaseStruct Move : Attacker.CurrentMoves) {
		ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
		UMoveButtonWidget* MoveButton = CreateWidget<UMoveButtonWidget>(UGameplayStatics::GetGameInstance(GetWorld()), Hud->GetMoveButtonWidgetClass());

		MoveButton->InitButton(Move.Name, Move.CurrPowerPoints, Move.PowerPoints, Move.MoveType);
		MoveDelegate.Broadcast(MoveButton);
	}
}

FPokemonStruct ABattleGameMode::GetCurrentOpponent()
{
	return OpponentTeam[0];
}

void ABattleGameMode::Run()
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController == nullptr) {
		return;
	}

	UWorldSaveData* SaveData = Cast<UWorldSaveData>(UGameplayStatics::CreateSaveGameObject(UWorldSaveData::StaticClass()));

	SaveData->InventoryData = PlayerController->GetInventory();
	SaveData->MoneyData = PlayerController->GetMoney();

	ABattleHUD* Hud = Cast<ABattleHUD>(PlayerController->GetHUD());

	UGameplayStatics::OpenLevel(GetWorld(), "TestingMap");

	Hud->PlayerOwner->SetInputMode(FInputModeGameOnly());
	Hud->PlayerOwner->bShowMouseCursor = false;
}

