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
}

void ABattleGameMode::PlacePlayerPokemon(FPokemonStruct Pokemon)
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController == nullptr) {
		return;
	}

	APokemonInceptionCharacter* Player = Cast<APokemonInceptionCharacter>(PlayerController->GetPawn());
	if (Player == nullptr) {
		return;
	}

	FRotator Rotation;
	FActorSpawnParameters SpawnInfo;
	FVector Position = FVector(-350, -270, 115);

	PlayerPokemonActor = GetWorld()->SpawnActor<AStaticOverworldPokemon>(Pokemon.SpeciesData.PokemonActor, Position, Rotation, SpawnInfo);
}

void ABattleGameMode::PlaceOpponentPokemon(FPokemonStruct Pokemon)
{
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController == nullptr) {
		return;
	}

	APokemonInceptionCharacter* Player = Cast<APokemonInceptionCharacter>(PlayerController->GetPawn());
	if (Player == nullptr) {
		return;
	}
	
	FRotator Rotation;
	Rotation.Yaw += 180;
	FActorSpawnParameters SpawnInfo;
	FVector Position = FVector(-350, 440, 115);

	OpponentPokemonActor = GetWorld()->SpawnActor<AStaticOverworldPokemon>(Pokemon.SpeciesData.PokemonActor, Position, Rotation, SpawnInfo);
}

void ABattleGameMode::BattleStart()
{
	/*ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	if (OpponentTeam.Num() == 1) {
		Hud->ShowText("A wild " + OpponentTeam[0].SpeciesData.Name.ToString() + "Pokemon appeared!");
	}*/
}

void ABattleGameMode::BattleEnd()
{
}

void ABattleGameMode::DisplayMessage(FString MessageToDisplay, UUserWidget* Widget)
{
	TempMessage = "";
	Message = MessageToDisplay;
	Iterator = 0;
	NextWidget = Widget;

	GetWorldTimerManager().SetTimer(MessageTimer, this, &ABattleGameMode::IterateMessage, 0.05, true);
}

void ABattleGameMode::IterateMessage()
{
	TempMessage = Message.Mid(0, Iterator);
	Iterator++;
	MessageUpdate.Broadcast(TempMessage);

	if (TempMessage == Message) {
		GetWorldTimerManager().ClearTimer(MessageTimer);
		GetWorldTimerManager().SetTimer(WidgetDelay, this, &ABattleGameMode::DisplayNextWidget, 1, false);
	}
}

void ABattleGameMode::DisplayNextWidget()
{
	WidgetUpdate.Broadcast(NextWidget);
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

		//Button clicked

		PokemonSlotDelegate.Broadcast(PokemonSlotWidget);
	}
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

void ABattleGameMode::ShowItemInfo(FItemBaseStruct InventoryItem)
{
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	UItemInfoWidget* ItemInfo = CreateWidget<UItemInfoWidget>(UGameplayStatics::GetGameInstance(GetWorld()), Hud->GetItemInfoWidgetClass());

	ItemInfo->SetDescription(InventoryItem.Description);

	ItemInfoDelegate.Broadcast(ItemInfo);
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

