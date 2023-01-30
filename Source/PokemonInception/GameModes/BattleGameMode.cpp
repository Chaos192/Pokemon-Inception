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

	BattleStart();
}

void ABattleGameMode::PlacePlayerPokemon(int PokemonId)
{
	if (IsValid(PlayerPokemonActor)) {
		PlayerPokemonActor->Destroy();
	}

	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController == nullptr) {
		return;
	}

	FRotator Rotation;
	FActorSpawnParameters SpawnInfo;
	FVector Position = FVector(-350, -270, 115);

	PlayerPokemonActor = GetWorld()->SpawnActor<AStaticOverworldPokemon>(PlayerController->PokemonParty[PokemonId].SpeciesData.PokemonActor, Position, Rotation, SpawnInfo);

	Hud->ShowText("You sent out " + PlayerController->PokemonParty[PokemonId].SpeciesData.Name.ToString() + "!");
}

void ABattleGameMode::PlaceOpponentPokemon(int PokemonId)
{
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	FRotator Rotation;
	Rotation.Yaw = 180;
	FActorSpawnParameters SpawnInfo;
	FVector Position = FVector(-350, 440, 115);

	OpponentPokemonActor = GetWorld()->SpawnActor<AStaticOverworldPokemon>(OpponentTeam[PokemonId].SpeciesData.PokemonActor, Position, Rotation, SpawnInfo);

	FString OpponentMessage;

	if (OpponentTeam.Num() == 1) {
		OpponentMessage = "A wild " + OpponentTeam[PokemonId].SpeciesData.Name.ToString() + " appeared!";
	}
	else {
		OpponentMessage = "Trainer sent out " + OpponentTeam[PokemonId].SpeciesData.Name.ToString() + "!";
	}

	Hud->ShowText(OpponentMessage);
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

void ABattleGameMode::UseMove(int MoveId, FString AttackerContextString)
{
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	FTimerDelegate MoveMessageDelegate;
	FString MoveMessage;

	FPokemonStruct Attacker;
	FPokemonStruct Opponent;

	if (AttackerContextString == "Player") {
		Attacker = PlayerController->PokemonParty[PlayerPokemonId];
		Opponent = OpponentTeam[OpponentPokemonId];
	}

	else if (AttackerContextString == "Opponent") {
		Attacker = OpponentTeam[OpponentPokemonId];
		Opponent = PlayerController->PokemonParty[PlayerPokemonId];
	}

	if (MoveId == -1) {
		int Damage = (((100 + Attacker.Attack + 15 * Attacker.Level) * 50) / (Opponent.Defence + 50)) / 5;

		if (AttackerContextString == "Player") {
			OpponentTeam[OpponentPokemonId].RecieveDamage(Damage);
			PlayerController->PokemonParty[PlayerPokemonId].RecieveDamage(Attacker.MaxHP/4);
		}

		else if (AttackerContextString == "Opponent") {
			PlayerController->PokemonParty[PlayerPokemonId].RecieveDamage(Damage);
			OpponentTeam[OpponentPokemonId].RecieveDamage(Attacker.MaxHP / 4);
		}

		MoveMessage = Attacker.SpeciesData.Name.ToString() + " used Struggle and dealt " + FString::FromInt(Damage) + " damage, but injured itself in the process!";
		Hud->ShowText(MoveMessage);

		return;
	}

	if (Attacker.Moves[MoveId].MoveStructType == "Attack") {
		FAttackMoveStruct* AttackMove = AttackMovesDT->FindRow<FAttackMoveStruct>(Attacker.Moves[MoveId].MoveID, "");

		//Base damage
		int Damage = (((100 + Attacker.Attack + 15 * Attacker.Level) * AttackMove->BaseDamage) / (Opponent.Defence + 50)) / 5;

		float EffectMultiplier = 0;

		//Adding effect multiplier
		if (Attacker.Effects.Contains(EEffect::AttackUp)) {
			EffectMultiplier = 1.5;
		}
		else if (Attacker.Effects.Contains(EEffect::AttackDown)) {
			EffectMultiplier = 0.5;
		}
		else EffectMultiplier = 1;

		if (Opponent.Effects.Contains(EEffect::DefenceUp)) {
			EffectMultiplier /= 1.5;
		}
		else if (Opponent.Effects.Contains(EEffect::DefenceDown)) {
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
		if ((Attacker.SpeciesData.Type1 == AttackMove->MoveType) || (Attacker.SpeciesData.Type2 == AttackMove->MoveType)) {
			Damage *= 1.25;
		}

		//Move effectiveness 
		FTypeStruct* MovetTypeStruct = TypesDT->FindRow<FTypeStruct>(FName(ETypeToString(AttackMove->MoveType)), "");
		
		if (MovetTypeStruct->SuperEffectiveAgainst.Contains(Opponent.SpeciesData.Type1)) {
			Damage *= 2;
		}
		else if (MovetTypeStruct->NotVeryEffectiveAgainst.Contains(Opponent.SpeciesData.Type1)) {
			Damage *= 0.5;
		}
		else if (MovetTypeStruct->NoEffectAgainst.Contains(Opponent.SpeciesData.Type1)) {
			Damage *= 0;
		}

		if (OpponentTeam[OpponentPokemonId].SpeciesData.Type2 != ETypes::None) {
			if(MovetTypeStruct->SuperEffectiveAgainst.Contains(Opponent.SpeciesData.Type2)) {
				Damage *= 2;
			}
			else if (MovetTypeStruct->NotVeryEffectiveAgainst.Contains(Opponent.SpeciesData.Type2)) {
				Damage *= 0.5;
			}
			else if (MovetTypeStruct->NoEffectAgainst.Contains(Opponent.SpeciesData.Type2)) {
				Damage *= 0;
			}
		}

		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Damage dealt: " + FString::FromInt(Damage)));
		if (AttackerContextString == "Player") {
			OpponentTeam[OpponentPokemonId].RecieveDamage(Damage);
			PlayerController->PokemonParty[PlayerPokemonId].Moves[MoveId].CurrPowerPoints--;
		}

		else if (AttackerContextString == "Opponent") {
			PlayerController->PokemonParty[PlayerPokemonId].RecieveDamage(Damage);
			OpponentTeam[OpponentPokemonId].Moves[MoveId].CurrPowerPoints--;
		}

		MoveMessage = Attacker.SpeciesData.Name.ToString() + " used " + AttackMove->Name.ToString() + " and dealt " + FString::FromInt(Damage) + " damage!";
	}

	
	if (Attacker.Moves[MoveId].MoveStructType == "Status") {
		FStatusMoveStruct* StatusMove = StatusMovesDT->FindRow<FStatusMoveStruct>(Attacker.Moves[MoveId].MoveID, "");

		MoveMessage = Attacker.SpeciesData.Name.ToString() + " used " + StatusMove->Name.ToString() + ", but it failed!";
		bool bIsMoveSuccessful = false;

		for (EEffect Effect : StatusMove->MoveEffects) {
			if (Effect == EEffect::RestoreHP) {
				switch (StatusMove->Target) {
					case ETarget::Self:
						if(Attacker.RestoreHP(Attacker.MaxHP / 2) == true) bIsMoveSuccessful = true;

						break;

					case ETarget::Opponent:
						if(Opponent.RestoreHP(Opponent.MaxHP / 2) == true) bIsMoveSuccessful = true;

						break;
				}
			}

			else {
				switch (StatusMove->Target) {
					case ETarget::Self:
						if (Attacker.AddEffect(Effect) == true) bIsMoveSuccessful = true;
				
						break;

					case ETarget::Opponent:
						if (Opponent.AddEffect(Effect) == true) bIsMoveSuccessful = true;
			
						break;
				}
			}
		}

		if(bIsMoveSuccessful == true) MoveMessage = Attacker.SpeciesData.Name.ToString() + " used " + StatusMove->Name.ToString() + "!";
		
		if (AttackerContextString == "Player") {
			PlayerController->PokemonParty[PlayerPokemonId] = Attacker;
			OpponentTeam[OpponentPokemonId] = Opponent;

			PlayerController->PokemonParty[PlayerPokemonId].Moves[MoveId].CurrPowerPoints--;
		}

		else if (AttackerContextString == "Opponent") {
			PlayerController->PokemonParty[PlayerPokemonId] = Attacker;
			OpponentTeam[OpponentPokemonId] = Opponent;

			OpponentTeam[OpponentPokemonId].Moves[MoveId].CurrPowerPoints--;
		}
	}

	Hud->ShowText(MoveMessage);
}

void ABattleGameMode::OpponentFaints()
{
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	OpponentPokemonActor->Destroy();

	int Exp = OpponentTeam[OpponentPokemonId].Level * OpponentTeam[OpponentPokemonId].SpeciesData.BaseExp / 7;

	if (PlayerController->PokemonParty[PlayerPokemonId].GainExp(Exp) == true) {
		TArray<UDataTable*> MoveTables;
		MoveTables.Add(AttackMovesDT);
		MoveTables.Add(StatusMovesDT);
		PlayerController->PokemonParty[PlayerPokemonId].CheckForNewMoves(MoveTables);
	}

	Hud->ShowText("The opponent " + OpponentTeam[OpponentPokemonId].SpeciesData.Name.ToString() + " was defeated, " + 
		PlayerController->PokemonParty[PlayerPokemonId].SpeciesData.Name.ToString() + " recieved " + FString::FromInt(Exp) + " Exp!");
	
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("OpponentFaints"));
}

void ABattleGameMode::PlayerFaints()
{
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	PlayerPokemonActor->Destroy();

	Hud->ShowText(PlayerController->PokemonParty[PlayerPokemonId].SpeciesData.Name.ToString() + " was defeated!");
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("player faints"));
}

void ABattleGameMode::BattleStart()
{
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController == nullptr) {
		return;
	}

	PlayerPokemonId = PlayerController->GetLeadPokemon();
	OpponentPokemonId = GetCurrentOpponent();

	FTimerDelegate SendOutOpponent;
	FTimerDelegate SendOutPlayer;

	SendOutOpponent.BindUFunction(this, FName("PlaceOpponentPokemon"), OpponentPokemonId);
	SendOutPlayer.BindUFunction(this, FName("PlacePlayerPokemon"), PlayerPokemonId);

	GetWorldTimerManager().SetTimer(MessageTimer, SendOutOpponent, 0.001, false);
	GetWorldTimerManager().SetTimer(MessageTimer2, SendOutPlayer, 1.5, false);
	GetWorldTimerManager().SetTimer(WidgetDelay, Hud, &ABattleHUD::ShowBattleStartWidget, 3, false);
}

void ABattleGameMode::BattleEnd()
{
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));


	if (bIsBattleVictory == true) {
		int Money = 0;

		for (FPokemonStruct Pokemon : OpponentTeam) {
			Money += Pokemon.Level * 100;
		}

		if ((PlayerController->Money + Money) > 1000000) {
			Money = 1000000 - PlayerController->Money;
		}

		PlayerController->Money += Money;

		Hud->ShowText("You won! You recieved " + FString::FromInt(Money) + " money!");
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("win"));
	}

	else if(bIsBattleVictory == false) {
		int Money = 0;

		for (FPokemonStruct Pokemon : OpponentTeam) {
			Money += Pokemon.Level * 100;
		}

		if ((PlayerController->Money - Money) < 0) {
			Money = 0 + PlayerController->Money;
		}

		PlayerController->Money -= Money;

		Hud->ShowText("You don't have any Pokemon that can fight! You lost " + FString::FromInt(Money) + " money!");
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("loss"));
	}

	GetWorldTimerManager().SetTimer(MessageTimer4, this, &ABattleGameMode::ExitBattleMap, CurrentAction, false);
}

void ABattleGameMode::BattleTurn(EAction PlayerAction)
{
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	bool bHasBattleEnded = false;
	CurrentAction = 0;

	if (PlayerAction == EAction::UseMove) {
		UseMove(SelectedMoveID, "Player");
		CurrentAction++;

		if (PlayerController->PokemonParty[PlayerPokemonId].bIsFainted == true) {
			GetWorldTimerManager().SetTimer(MessageTimer, this, &ABattleGameMode::PlayerFaints, CurrentAction, false);
			CurrentAction++;

			if (PlayerController->bIsPartyDefeated() == true) {
				bHasBattleEnded = true;
				bIsBattleVictory = false;
				GetWorldTimerManager().SetTimer(MessageTimer5, this, &ABattleGameMode::BattleEnd, CurrentAction, false);
			}
			else {
				bSkipTurn = true;
			}
		}

		if (OpponentTeam[OpponentPokemonId].bIsFainted == true && PlayerController->bIsPartyDefeated() == false) {
			GetWorldTimerManager().SetTimer(MessageTimer2, this, &ABattleGameMode::OpponentFaints, CurrentAction, false);
			CurrentAction++;

			if (bIsOpponentDefeated() == true) {
				bHasBattleEnded = true;
				bIsBattleVictory = true;
				GetWorldTimerManager().SetTimer(MessageTimer5, this, &ABattleGameMode::BattleEnd, CurrentAction, false);
			}
			else {
				OpponentPokemonId = GetCurrentOpponent();

				FTimerDelegate SwitchOpponent;
				SwitchOpponent.BindUFunction(this, FName("PlaceOpponentPokemon"), OpponentPokemonId);

				GetWorldTimerManager().SetTimer(MessageTimer3, SwitchOpponent, CurrentAction, false);
				CurrentAction++;
			}
		}

		if (bSkipTurn == true && bHasBattleEnded == false) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("switching pokemon"));
			GetWorldTimerManager().SetTimer(MessageTimer4, Hud, &ABattleHUD::ShowPokemon, CurrentAction, false);
		}
	}

	if (PlayerAction == EAction::SwitchOut) {
		PlayerController->PokemonParty[PlayerPokemonId].ClearEffects();
		PlayerPokemonId = SwitchedPokemonID;
		PlacePlayerPokemon(PlayerPokemonId);
		CurrentAction++;

		if (bSkipTurn == false) {
			//something
		}
		bSkipTurn = false;
	}

	if (bHasBattleEnded == false && PlayerController->PokemonParty[PlayerPokemonId].bIsFainted == false) {
		GetWorldTimerManager().SetTimer(WidgetDelay, Hud, &ABattleHUD::ShowBattleStartWidget, CurrentAction, false);
	}
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

void ABattleGameMode::SelectMove(int MoveId)
{
	SelectedMoveID = MoveId;
	BattleTurn(EAction::UseMove);
}

void ABattleGameMode::SelectPokemon(int InId)
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (PlayerPokemonId == InId || PlayerController->PokemonParty[InId].bIsFainted == true) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("This pokemon can't enter"));
		return;
	}

	SwitchedPokemonID = InId;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("switching pokemon"));
	BattleTurn(EAction::SwitchOut);
}

void ABattleGameMode::ShowPokemonInMenu()
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	TArray<FPokemonStruct> Party = PlayerController->PokemonParty;

	for (int i = 0; i < Party.Num(); i++) {
		ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
		UPokemonSlotWidget* PokemonSlotWidget = CreateWidget<UPokemonSlotWidget>(UGameplayStatics::GetGameInstance(GetWorld()), Hud->GetPokemonSlotWidgetClass());

		PokemonSlotWidget->SetPokemonName(Party[i].SpeciesData.Name);
		PokemonSlotWidget->SetPokemonLevel(Party[i].Level);
		PokemonSlotWidget->SetPokemonImage(Party[i].SpeciesData.Image);
		PokemonSlotWidget->SetPokemonHP(Party[i].CurrHP, Party[i].MaxHP);
		PokemonSlotWidget->SetPokemonEXP(Party[i].CurrExp, Party[i].RequiredExp);
		PokemonSlotWidget->SetPokemon(i);

		PokemonSlotWidget->PokemonClick.AddDynamic(Hud, &ABattleHUD::ShowPokemonSummary);
		PokemonSlotWidget->PokemonClick.AddDynamic(Hud, &ABattleHUD::ShowSwitchOutPopup);

		PokemonSlotDelegate.Broadcast(PokemonSlotWidget);
	}
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

void ABattleGameMode::ShowPokemonMoves()
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	FPokemonStruct Attacker = PlayerController->PokemonParty[PlayerPokemonId];

	for (int i = 0; i < Attacker.CurrentMoves.Num(); i++) {
		ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
		UMoveButtonWidget* MoveButton = CreateWidget<UMoveButtonWidget>(UGameplayStatics::GetGameInstance(GetWorld()), Hud->GetMoveButtonWidgetClass());

		MoveButton->InitButton(Attacker.Moves[Attacker.CurrentMoves[i]].Name, Attacker.Moves[Attacker.CurrentMoves[i]].CurrPowerPoints,
			Attacker.Moves[Attacker.CurrentMoves[i]].PowerPoints, Attacker.Moves[Attacker.CurrentMoves[i]].MoveType);
		MoveButton->SetMove(Attacker.CurrentMoves[i]);

		if (Attacker.Moves[Attacker.CurrentMoves[i]].CurrPowerPoints > 0) {
			MoveButton->ButtonClicked.AddDynamic(this, &ABattleGameMode::SelectMove);
		}

		MoveDelegate.Broadcast(MoveButton);
	}
}

FPokemonStruct ABattleGameMode::GetCurrentOpponentStruct() {
	return OpponentTeam[OpponentPokemonId];
}

int ABattleGameMode::GetPlayerPokemonId() {
	return PlayerPokemonId;
}

bool ABattleGameMode::HasPlayerRanOutOfPP()
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	return PlayerController->PokemonParty[PlayerPokemonId].bHasRanOutOfPP();
}

void ABattleGameMode::ExitBattleMap()
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController == nullptr) {
		return;
	}

	for (int i = 0; i < PlayerController->PokemonParty.Num(); i++) {
		PlayerController->PokemonParty[i].ClearEffects();
	}

	UWorldSaveData* SaveData = Cast<UWorldSaveData>(UGameplayStatics::CreateSaveGameObject(UWorldSaveData::StaticClass()));

	SaveData->InventoryData = PlayerController->Inventory;
	SaveData->PokedexData = PlayerController->Pokedex;
	SaveData->MoneyData = PlayerController->Money;
	SaveData->PartyData = PlayerController->PokemonParty;
	SaveData->StorageData = PlayerController->PokemonStorage;

	SaveData->GameMapData = SavedGameMapData;

	UGameplayStatics::SaveGameToSlot(SaveData, "SaveSlot", 0);

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

