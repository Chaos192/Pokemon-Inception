// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "../UI/BattleUI/BattleHUD.h"
#include "../UI/BattleUI/MoveButtonWidget.h"
#include "../Player/BattleController.h"
#include "../Player/PokemonInceptionCharacter.h"
#include "../Pokemon/StaticOverworldPokemon.h"
#include "../Pokemon/AttackMoveStruct.h"
#include "../Pokemon/StatusMoveStruct.h"
#include "../Pokemon/TypeStruct.h"


void ABattleGameMode::BeginPlay()
{
	Super::BeginPlay();

	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController == nullptr) {
		return;
	}

	UWorldSaveData* SaveData = Cast<UWorldSaveData>(UGameplayStatics::CreateSaveGameObject(UWorldSaveData::StaticClass()));

	if (UGameplayStatics::DoesSaveGameExist("SaveSlot", 0)) {
		SaveData = Cast<UWorldSaveData>(UGameplayStatics::LoadGameFromSlot("SaveSlot", 0));

		PlayerController->Inventory = SaveData->InventoryData;
		PlayerController->Pokedex = SaveData->PokedexData;
		PlayerController->Money = SaveData->MoneyData;
		PlayerController->PokemonParty = SaveData->PartyData;
		OpponentTeam = SaveData->OpponentData;

		SavedGameMapData = SaveData->GameMapData;
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), Camera, FoundActors);
	PlayerController->SetViewTargetWithBlend(FoundActors[0], 0, EViewTargetBlendFunction::VTBlend_Linear);

	PlayerPokemonId = PlayerController->GetLeadPokemon();
	OpponentPokemonId = GetCurrentOpponent();

	PlacePlayerPokemon(PlayerPokemonId);
	PlaceOpponentPokemon(OpponentPokemonId);

	BattleStart();
}

void ABattleGameMode::PlacePlayerPokemon(int PokemonId)
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController == nullptr) {
		return;
	}

	FRotator Rotation;
	FActorSpawnParameters SpawnInfo;
	FVector Position = FVector(-350, -270, 115);

	PlayerPokemonActor = GetWorld()->SpawnActor<AStaticOverworldPokemon>(PlayerController->PokemonParty[PokemonId].SpeciesData.PokemonActor, Position, Rotation, SpawnInfo);
}

void ABattleGameMode::PlaceOpponentPokemon(int PokemonId)
{
	FRotator Rotation;
	Rotation.Yaw = 180;
	FActorSpawnParameters SpawnInfo;
	FVector Position = FVector(-350, 440, 115);

	OpponentPokemonActor = GetWorld()->SpawnActor<AStaticOverworldPokemon>(OpponentTeam[PokemonId].SpeciesData.PokemonActor, Position, Rotation, SpawnInfo);
}

bool ABattleGameMode::bIsOpponentDefeated()
{
	for (FPokemonStruct Pokemon : OpponentTeam) {
		if (Pokemon.bIsFainted == false) {
			return false;
		}
	}
	return true;
}

int ABattleGameMode::GetCurrentOpponent()
{
	for (int i = 0; i < OpponentTeam.Num(); i++) {
		if (OpponentTeam[i].bIsFainted == false) {
			return i;
		}
	}

	return -1;
}

void ABattleGameMode::UseMove(int AttackerId, int OpponentId, FMoveBaseStruct Move)
{
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController == nullptr) {
		return;
	}

	FTimerDelegate MoveMessageDelegate;
	FString MoveMessage;

	if (Move.CurrPowerPoints == 0) {
		return;
	}

	if (Move.MoveStructType == "Attack") {
		FAttackMoveStruct* AttackMove = AttackMovesDT->FindRow<FAttackMoveStruct>(Move.MoveID, "");

		//Base damage
		int Damage = (((100 + PlayerController->PokemonParty[AttackerId].Attack + 15 * PlayerController->PokemonParty[AttackerId].Level) *
			AttackMove->BaseDamage) / (OpponentTeam[OpponentId].Defence + 50)) / 5;

		float EffectMultiplier = 0;

		//Adding effect multiplier
		if (PlayerController->PokemonParty[AttackerId].Effects.Contains(EEffect::AttackUp)) {
			EffectMultiplier = 1.5;
		}
		else if (PlayerController->PokemonParty[AttackerId].Effects.Contains(EEffect::AttackDown)) {
			EffectMultiplier = 0.5;
		}
		else EffectMultiplier = 1;

		if (OpponentTeam[OpponentId].Effects.Contains(EEffect::DefenceUp)) {
			EffectMultiplier /= 1.5;
		}
		else if (OpponentTeam[OpponentId].Effects.Contains(EEffect::DefenceDown)) {
			EffectMultiplier /= 0.5;
		}

		Damage *= EffectMultiplier;

		//Is it a critical hit
		int CriticalHitChance = FMath::RandRange(1, 100);
		if (CriticalHitChance <= 5) {
			Damage *= 1.5;
		}

		//Multiplying by a random value
		float RandomMultiplier = FMath::RandRange(0.85, 1.00);
		Damage *= RandomMultiplier;

		//Same type attack bonus - STAB
		if ((PlayerController->PokemonParty[AttackerId].SpeciesData.Type1 == AttackMove->MoveType) ||
			(PlayerController->PokemonParty[AttackerId].SpeciesData.Type2 == AttackMove->MoveType)) {
			Damage *= 1.25;
		}

		//Move effectiveness 
		FTypeStruct* MovetTypeStruct = TypesDT->FindRow<FTypeStruct>(FName(ETypeToString(AttackMove->MoveType)), "");
		
		if (MovetTypeStruct->SuperEffectiveAgainst.Contains(OpponentTeam[OpponentId].SpeciesData.Type1)) {
			Damage *= 2;
		}
		else if (MovetTypeStruct->NotVeryEffectiveAgainst.Contains(OpponentTeam[OpponentId].SpeciesData.Type1)) {
			Damage *= 0.5;
		}
		else if (MovetTypeStruct->NoEffectAgainst.Contains(OpponentTeam[OpponentId].SpeciesData.Type1)) {
			Damage *= 0;
		}

		if (OpponentTeam[OpponentPokemonId].SpeciesData.Type2 != ETypes::None) {
			if(MovetTypeStruct->SuperEffectiveAgainst.Contains(OpponentTeam[OpponentId].SpeciesData.Type2)) {
				Damage *= 2;
			}
			else if (MovetTypeStruct->NotVeryEffectiveAgainst.Contains(OpponentTeam[OpponentId].SpeciesData.Type2)) {
				Damage *= 0.5;
			}
			else if (MovetTypeStruct->NoEffectAgainst.Contains(OpponentTeam[OpponentId].SpeciesData.Type2)) {
				Damage *= 0;
			}
		}

		OpponentTeam[OpponentId].RecieveDamage(Damage);
		/*UE_LOG(LogTemp, Error, TEXT("Damage dealt: %s"), *FString::FromInt(Damage));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Damage dealt: " + FString::FromInt(Damage)));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Opponent HP: " + FString::FromInt(GetCurrentOpponent().CurrHP) + "/" + FString::FromInt(GetCurrentOpponent().MaxHP)));*/

		MoveMessage = PlayerController->PokemonParty[AttackerId].SpeciesData.Name.ToString() + " used " + AttackMove->Name.ToString() + " and dealt " + FString::FromInt(Damage) + " damage!";
		MoveMessageDelegate.BindUFunction(Hud, FName("ShowText"), MoveMessage);
	}


	
	if (Move.MoveStructType == "Status") {
		FStatusMoveStruct* StatusMove = StatusMovesDT->FindRow<FStatusMoveStruct>(Move.MoveID, "");

		MoveMessage = PlayerController->PokemonParty[AttackerId].SpeciesData.Name.ToString() + " used " + StatusMove->Name.ToString() + "!";

		for (EEffect Effect : StatusMove->MoveEffects) {
			if (Effect == EEffect::RestoreHP) {
				switch (StatusMove->Target) {
					case ETarget::Self:
						PlayerController->PokemonParty[AttackerId].RestoreHP(PlayerController->PokemonParty[AttackerId].MaxHP / 2);
						MoveMessageDelegate.BindUFunction(Hud, FName("ShowText"), MoveMessage);

						break;

					case ETarget::Opponent:
						OpponentTeam[OpponentId].RestoreHP(OpponentTeam[OpponentId].MaxHP / 2);
						MoveMessageDelegate.BindUFunction(Hud, FName("ShowText"), MoveMessage);

						break;
					}
			}

			else {
				switch (StatusMove->Target) {
					case ETarget::Self:
						if (PlayerController->PokemonParty[AttackerId].AddEffect(Effect) == true) {
							MoveMessageDelegate.BindUFunction(Hud, FName("ShowText"), MoveMessage);

						}
						else {
							MoveMessage = PlayerController->PokemonParty[AttackerId].SpeciesData.Name.ToString() + " used " + StatusMove->Name.ToString() + ", but it failed!";
							MoveMessageDelegate.BindUFunction(Hud, FName("ShowText"), MoveMessage);

						}
						break;

					case ETarget::Opponent:
						if (OpponentTeam[OpponentId].AddEffect(Effect) == true) {
							MoveMessageDelegate.BindUFunction(Hud, FName("ShowText"), MoveMessage);

						}
						else {
							MoveMessage = PlayerController->PokemonParty[AttackerId].SpeciesData.Name.ToString() + " used " + StatusMove->Name.ToString() + ", but it failed!";
							MoveMessageDelegate.BindUFunction(Hud, FName("ShowText"), MoveMessage);

						}
						break;
				}
			}
		}
	}

	GetWorldTimerManager().SetTimer(MessageTimer, MoveMessageDelegate, 0.001, false);
	

	Move.CurrPowerPoints--;
}

void ABattleGameMode::BattleStart()
{
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController == nullptr) {
		return;
	}

	FTimerDelegate OpponentMessageDelegate;
	FTimerDelegate PlayerMessageDelegate;

	FString OpponentMessage;

	if (OpponentTeam.Num() == 1) {
		OpponentMessage = "A wild " + OpponentTeam[OpponentPokemonId].SpeciesData.Name.ToString() + " appeared!";
	}
	else {
		OpponentMessage = "Trainer sent out " + OpponentTeam[OpponentPokemonId].SpeciesData.Name.ToString() + "!";
	}

	OpponentMessageDelegate.BindUFunction(Hud, FName("ShowText"), OpponentMessage);
	PlayerMessageDelegate.BindUFunction(Hud, FName("ShowText"), ("You sent out " + PlayerController->PokemonParty[PlayerPokemonId].SpeciesData.Name.ToString() + "!"));

	GetWorldTimerManager().SetTimer(MessageTimer, OpponentMessageDelegate, 0.001, false);
	GetWorldTimerManager().SetTimer(MessageTimer2, PlayerMessageDelegate, 1.5, false);
	GetWorldTimerManager().SetTimer(WidgetDelay, Hud, &ABattleHUD::ShowBattleStartWidget, 3, false);
}

void ABattleGameMode::BattleEnd(FString BattleOutcome)
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (BattleOutcome == "Win") {
		int Money = 0;

		for (FPokemonStruct Pokemon : OpponentTeam) {
			Money += Pokemon.Level * 100;
		}

		PlayerController->Money += Money;
		if (PlayerController->Money > 1000000) PlayerController->Money = 1000000;
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("You won and got " + FString::FromInt(Money) + " money!"));
	}

	else if(BattleOutcome == "Loss") {
		int Money = 0;

		for (FPokemonStruct Pokemon : OpponentTeam) {
			Money += Pokemon.Level * 100;
		}

		PlayerController->Money -= Money;
		if (PlayerController->Money < 0) PlayerController->Money = 0;
	}

	ExitBattleMap();
}

void ABattleGameMode::BattleTurn(EAction PlayerAction)
{
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (PlayerAction == EAction::UseMove) {
		UseMove(PlayerPokemonId, OpponentPokemonId, SelectedMove);

		if (OpponentTeam[OpponentPokemonId].bIsFainted == true) {
			OpponentPokemonActor->Destroy();

			int Exp = OpponentTeam[OpponentPokemonId].Level * OpponentTeam[OpponentPokemonId].SpeciesData.BaseExp / 7;
			PlayerController->PokemonParty[PlayerPokemonId].GainExp(Exp);
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Exp Gained: " + FString::FromInt(Exp)));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Exp: " + FString::FromInt(PlayerController->PokemonParty[PlayerPokemonId].CurrExp) + "/" + FString::FromInt(PlayerController->PokemonParty[PlayerPokemonId].RequiredExp)));
			
			/*TArray<UDataTable*> MoveTables;
			MoveTables.Add(AttackMovesDT);
			MoveTables.Add(StatusMovesDT);
			PlayerController->PokemonParty[PlayerPokemonId].CheckForNewMoves(MoveTables);*/
			
			if (bIsOpponentDefeated() == true) {
				BattleEnd("Win");
			}
			else {
				OpponentPokemonId = GetCurrentOpponent();
				PlaceOpponentPokemon(OpponentPokemonId);
			}
		}
	}
	GetWorldTimerManager().SetTimer(WidgetDelay, Hud, &ABattleHUD::ShowBattleStartWidget, 2, false);
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

void ABattleGameMode::SelectMove(FMoveBaseStruct Move)
{
	SelectedMove = Move;
	BattleTurn(EAction::UseMove);
}

void ABattleGameMode::ShowPokemonInMenu()
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	for (FPokemonStruct Pokemon : PlayerController->PokemonParty) {
		ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
		UPokemonSlotWidget* PokemonSlotWidget = CreateWidget<UPokemonSlotWidget>(UGameplayStatics::GetGameInstance(GetWorld()), Hud->GetPokemonSlotWidgetClass());

		PokemonSlotWidget->SetPokemonName(Pokemon.SpeciesData.Name);
		PokemonSlotWidget->SetPokemonLevel(Pokemon.Level);
		PokemonSlotWidget->SetPokemonImage(Pokemon.SpeciesData.Image);
		PokemonSlotWidget->SetPokemonHP(Pokemon.CurrHP, Pokemon.MaxHP);
		PokemonSlotWidget->SetPokemonEXP(Pokemon.CurrExp, Pokemon.RequiredExp);
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
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	TArray<FItemBaseStruct> UniqueItems;
	TArray<int> ItemCount;

	for (FItemBaseStruct Item : PlayerController->Inventory) {
		if (UniqueItems.Contains(Item) == false) {
			UniqueItems.Add(Item);
		}
	}

	for (FItemBaseStruct Item : UniqueItems) {
		int Count = 0;

		for (FItemBaseStruct ItemToSearch : PlayerController->Inventory) {
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

FPokemonStruct ABattleGameMode::GetCurrentOpponentStruct() {
	return OpponentTeam[OpponentPokemonId];
}

void ABattleGameMode::ShowPokemonMoves()
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	FPokemonStruct Attacker = PlayerController->PokemonParty[PlayerPokemonId];

	for (FMoveBaseStruct Move : Attacker.CurrentMoves) {
		ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
		UMoveButtonWidget* MoveButton = CreateWidget<UMoveButtonWidget>(UGameplayStatics::GetGameInstance(GetWorld()), Hud->GetMoveButtonWidgetClass());

		MoveButton->InitButton(Move.Name, Move.CurrPowerPoints, Move.PowerPoints, Move.MoveType);
		MoveButton->SetMove(Move);
		MoveButton->ButtonClicked.AddDynamic(this, &ABattleGameMode::SelectMove);

		MoveDelegate.Broadcast(MoveButton);
	}
}

void ABattleGameMode::ExitBattleMap()
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController == nullptr) {
		return;
	}

	UWorldSaveData* SaveData = Cast<UWorldSaveData>(UGameplayStatics::CreateSaveGameObject(UWorldSaveData::StaticClass()));

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("MoneyBefore: " + FString::FromInt(SaveData->MoneyData)));

	SaveData->InventoryData = PlayerController->Inventory;
	SaveData->PokedexData = PlayerController->Pokedex;
	SaveData->MoneyData = PlayerController->Money;
	SaveData->PartyData = PlayerController->PokemonParty;
	SaveData->StorageData = PlayerController->PokemonStorage;

	SaveData->GameMapData = SavedGameMapData;

	UGameplayStatics::SaveGameToSlot(SaveData, "SaveSlot", 0);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("MoneyNow: " + FString::FromInt(SaveData->MoneyData)));

	ABattleHUD* Hud = Cast<ABattleHUD>(PlayerController->GetHUD());

	UGameplayStatics::OpenLevel(GetWorld(), "TestingMap");

	Hud->PlayerOwner->SetInputMode(FInputModeGameOnly());
	Hud->PlayerOwner->bShowMouseCursor = false;
}

void ABattleGameMode::Run()
{
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	Hud->ShowText("Got away safely...");
	GetWorldTimerManager().SetTimer(WidgetDelay, this, &ABattleGameMode::ExitBattleMap, 2, false);
}

